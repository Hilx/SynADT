LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;
USE work.ALL;
USE work.budpack.ALL;

ENTITY down_marker IS
  PORT(
    clk          : IN  std_logic;
    reset        : IN  std_logic;
    start        : IN  std_logic;
    flag_alloc   : IN  std_logic;       -- 1 = alloc, 0 = free
    probe_in     : IN  tree_probe;
    reqsize      : IN  std_logic_vector(31 DOWNTO 0);
    done_bit     : OUT std_logic;
    node_out     : OUT std_logic_vector(1 DOWNTO 0);
    flag_markup  : OUT std_logic;
    ram_we       : OUT std_logic;       -- write_start
    ram_addr     : OUT std_logic_vector(31 DOWNTO 0);
    ram_data_in  : OUT std_logic_vector(31 DOWNTO 0);
    ram_data_out : IN  std_logic_vector(31 DOWNTO 0);
    read_done    : IN  std_logic;
    write_done   : IN  std_logic;
    read_start   : OUT std_logic
    );
END ENTITY down_marker;

ARCHITECTURE synth_dmark OF down_marker IS

  ALIAS slv IS std_logic_vector;
  ALIAS usgn IS unsigned;

  TYPE StateType IS (idle, prep, s0, s_read, s_mark, s_w0, s_w1, done, store, update, w_wait, read_wait0, write_wait);
  SIGNAL state, nstate : StateType;

  SIGNAL top_node_size     : std_logic_vector(31 DOWNTO 0);
  SIGNAL log2top_node_size : std_logic_vector(6 DOWNTO 0);
  SIGNAL group_addr        : std_logic_vector(31 DOWNTO 0);
  SIGNAL mtree             : std_logic_vector(31 DOWNTO 0);
  SIGNAL cur               : tree_probe;
  SIGNAL gen               : tree_probe;
  SIGNAL size_left         : std_logic_vector(31 DOWNTO 0);
  SIGNAL flag_first        : std_logic;
  SIGNAL original_top_node : std_logic_vector(1 DOWNTO 0);
  SIGNAL alvec_sel         : integer RANGE 0 TO 31;
  SIGNAL shift             : std_logic_vector(5 DOWNTO 0);  -- double the first cur.nodesel
  SIGNAL utree             : std_logic_vector(31 DOWNTO 0);

  -- free related
  TYPE holder_array IS ARRAY (0 TO MAX_TREE_DEPTH) OF holder_type;
  SIGNAL holder         : holder_array;
  SIGNAL index          : std_logic_vector(5 DOWNTO 0);
  SIGNAL effective_node : std_logic_vector(1 DOWNTO 0);

  -- MEMORY ACCESS
  SIGNAL read_data : std_logic_vector(31 DOWNTO 0);
  
BEGIN

  p0 : PROCESS(state, start, size_left, flag_alloc)
  BEGIN

    nstate   <= idle;
    done_bit <= '0';

    CASE state IS
      WHEN idle =>
        nstate <= idle;
        IF start = '1' THEN
          nstate <= prep;
        END IF;
      WHEN prep       => nstate <= s0;
      WHEN s0         => nstate <= read_wait0;
      WHEN read_wait0 => nstate <= read_wait0;
      WHEN s_read     => nstate <= s_mark;
      WHEN s_mark =>
        
        nstate <= s_w0;
        IF flag_alloc = '0' THEN
          nstate <= store;
        END IF;
        
      WHEN s_w0       => nstate <= write_wait;
      WHEN write_wait => nstate <= write_wait;
      WHEN s_w1 =>
        
        IF flag_alloc = '1' THEN        -- when allocating
          nstate <= w_wait;
          IF to_integer(usgn(size_left)) = 0 THEN
            nstate <= done;
          END IF;
        ELSE                            -- when free
          nstate <= update;
        END IF;
        
      WHEN w_wait => nstate <= s0;

      WHEN done =>
        nstate   <= idle;
        done_bit <= '1';
        
      WHEN store  => nstate <= s0;
      WHEN update => nstate <= s_w0;

      WHEN OTHERS => NULL;
    END CASE;
    
  END PROCESS;

  p1 : PROCESS
    VARIABLE rowbase_var         : slv(31 DOWNTO 0);
    VARIABLE step                : std_logic_vector(4 DOWNTO 0);  -- 8 possible nodes to mark
    VARIABLE size_left_var       : slv(31 DOWNTO 0);
    VARIABLE offset              : std_logic_vector(2 DOWNTO 0);
    VARIABLE index_var           : integer RANGE 0 TO MAX_TREE_DEPTH;
    VARIABLE update_start_bit    : integer RANGE 0 TO 15;
    VARIABLE mtree_var           : slv(31 DOWNTO 0);
    VARIABLE last_write_ever_var : std_logic;

  BEGIN
    WAIT UNTIL clk'event AND clk = '1';

    IF reset = '0' THEN                 -- active low
      state <= idle;
    ELSE
      state      <= nstate;
      read_start <= '0';
      ram_we     <= '0';

      IF state = prep THEN
        cur         <= probe_in;
        size_left   <= reqsize;
        flag_first  <= '1';
        flag_markup <= '0';
      END IF;

      IF state = s0 THEN
        gen.alvec <= '0';

        top_node_size     <= slv(usgn(TOTAL_MEM_BLOCKS) SRL (to_integer(3*(usgn(cur.verti)))));
        log2top_node_size <= slv(resize(usgn(LOG2TMB) - 3* (usgn(cur.verti)), log2top_node_size'length));

        rowbase_var := slv(usgn(cur.rowbase) + (to_unsigned(1, rowbase_var'length) SLL (to_integer(3*(usgn(cur.verti)-1)))));

        gen.rowbase <= rowbase_var;

        IF cur.alvec = '0' THEN
          group_addr <= slv(usgn(rowbase_var) + usgn(cur.horiz));
        ELSE                            -- in allocation vector
          group_addr <= slv(usgn(ALVEC_SHIFT) +(usgn(cur.horiz) SRL 4));
        END IF;

        -- ( horiz % 16 ) * 2
        alvec_sel <= to_integer(resize(usgn(cur.horiz(3 DOWNTO 0)), 5) SLL 1);

        offset := (OTHERS => '0');

        read_start <= '1';
        --ram_addr <= group_addr;

      END IF;  -- finished case of s0

      IF state = read_wait0 THEN
        IF read_done = '1' THEN
          read_data <= ram_data_out;
          state     <= s_read;
        END IF;
      END IF;

      IF state = s_read THEN
        mtree <= read_data;

        shift <= (OTHERS => '0');
        IF flag_first = '1' THEN  -- keep a copy of the top node of chosen group
          flag_first <= '0';
          shift      <= slv(resize(usgn(cur.nodesel), shift'length) SLL 1);  -- set shift value = nodesel * 2
          -- probably doesn't need special case, as the nodesel is always in range 0 to 7.
          --    if to_integer(usgn(top_node_size)) = 4 then -- special case for shift
          --    shift      <= slv(resize(usgn(cur.nodesel), shift'length) SLL 2);  -- set shift value = nodesel * 4
          --    end if;

          IF cur.alvec = '0' THEN
            original_top_node <= read_data(1 DOWNTO 0);
          ELSE                          -- in allocation vector            
            original_top_node <= read_data(alvec_sel + 1 DOWNTO alvec_sel);
          END IF;
        END IF;
        
      END IF;  -- finished case of s_read

      IF state = s_mark THEN

        IF cur.alvec = '1' THEN
          IF to_integer(usgn(reqsize)) = 1 AND probe_in.saddr(0) = '1' THEN
            mtree(alvec_sel + 1) <= flag_alloc;
          ELSE
            mtree(alvec_sel) <= flag_alloc;
          END IF;

          size_left_var := (OTHERS => '0');
          size_left     <= size_left_var;
          
        ELSIF size_left < slv(usgn(top_node_size) SRL 3) THEN  -- reqsize < topsize/8
          
          mtree(14 + to_integer(usgn(shift))) <= flag_alloc;
          size_left_var                       := size_left;
          offset                              := slv(resize(usgn(shift) SRL 1, offset'length));
        ELSIF to_integer(usgn(top_node_size)) = 4 THEN  -- topsize = 4

          step := slv(resize(usgn(size_left), step'length));  -- step = size_left

          FOR i IN 0 TO 15 LOOP
            -- if shift =< i < shift +step
            IF i >= to_integer(usgn(shift)) AND i < to_integer(usgn(shift) + (usgn(step) SLL 2)) THEN
              mtree(i + 14) <= flag_alloc;
            END IF;
          END LOOP;

          size_left_var := (OTHERS => '0');
          size_left     <= size_left_var;

        ELSE  -- other topsize                        
          
          step := slv(resize(usgn(size_left) SRL to_integer(usgn(log2top_node_size) - 3), step'length));
          FOR i IN 0 TO 15 LOOP

            -- if shift =< i < shift +step
            IF i >= to_integer(usgn(shift)) AND i < to_integer(usgn(shift) + (usgn(step) SLL 1)) THEN
              mtree(i + 14) <= flag_alloc;
            END IF;
          END LOOP;

          -- size left = size left - step * base node size
          size_left_var := slv(usgn(size_left) -(resize(usgn(step), size_left'length) SLL to_integer(usgn(log2top_node_size) - 3)));
          size_left     <= size_left_var;

          IF to_integer(usgn(size_left_var)) /= 0 THEN

            mtree(14 + to_integer(usgn(shift)) + to_integer(usgn(step) SLL 1)) <= flag_alloc;
          END IF;
          -- offset = shift/2 + step
          offset := slv(resize((usgn(shift) SRL 1) + usgn(step), offset'length));
          
        END IF;  -- finished discussing 4 cases

        IF to_integer(usgn(size_left_var)) = 0 THEN
          
          IF flag_alloc = '0' THEN
            state <= s_w0;
          END IF;

          gen.alvec <= '0';
          IF USE_ALVEC = '1' THEN       -- used to be 16
            gen.alvec <= '1';
          END IF;
          
        ELSE
          
          gen.saddr   <= cur.saddr;
          gen.nodesel <= offset;
          gen.verti   <= slv(usgn(cur.verti) + 1);
          gen.horiz   <= slv((usgn(cur.horiz) SLL 3) + usgn(offset));
          gen.alvec   <= '0';
          IF to_integer(usgn(size_left_var)) = 1 AND USE_ALVEC = '1' THEN  -- used to be 16
            gen.alvec <= '1';
          END IF;

          IF flag_alloc = '0' THEN
            state <= store;

          END IF;
          
        END IF;

        IF flag_alloc = '0' THEN
          index <= slv(resize(usgn(cur.verti), index'length));
        END IF;

      END IF;  -- finished case of s_mark

      IF state = s_w0 THEN

        -- write data
        IF cur.alvec = '0' THEN         -- used to check gen.alvec
          ram_data_in <= utree;
        ELSE
          ram_data_in <= mtree;
        END IF;

        IF flag_alloc = '1' THEN  -- in case of malloc, decide if i need to mark upwards
          
          IF cur.verti = probe_in.verti AND to_integer(usgn(probe_in.verti)) > 0 THEN
            flag_markup <= '1';
            IF gen.alvec = '0' THEN
              node_out <= utree(1 DOWNTO 0);
              IF utree(1 DOWNTO 0) = original_top_node THEN
                flag_markup <= '0';
              END IF;
            ELSE
              node_out <= mtree(alvec_sel + 1 DOWNTO alvec_sel);
              IF mtree(alvec_sel + 1 DOWNTO alvec_sel) = original_top_node THEN
                flag_markup <= '0';
              END IF;
            END IF;
          END IF;
          
        ELSE                            -- free
          
          IF gen.alvec = '1' THEN
            effective_node <= mtree(alvec_sel+1 DOWNTO alvec_sel);
            node_out       <= mtree(alvec_sel+1 DOWNTO alvec_sel);
          ELSE
            effective_node <= utree(1 DOWNTO 0);
            node_out       <= utree(1 DOWNTO 0);
          END IF;
          
        END IF;


        ram_we <= '1';
        --ram_addr <= group_addr;
        
      END IF;  -- finished case of s_w0

      IF state = write_wait THEN
        
        IF write_done = '1' THEN
          state <= s_w1;
        END IF;
        
      END IF;

      IF state = s_w1 THEN
        

        cur <= gen;

        -- free
        IF flag_alloc = '0' THEN

          IF (slv(resize(usgn(index), 32)) = slv(usgn(probe_in.verti))) THEN
            state <= done;

            flag_markup <= '1';
            IF effective_node = original_top_node THEN
              flag_markup <= '0';
            END IF;
            
          END IF;
          
        END IF;
        
      END IF;  -- finish case of s_w1

      IF state = update THEN
        
        index_var := to_integer(usgn(index) - 1);
        index     <= slv(to_unsigned(index_var, index'length));

        update_start_bit := to_integer(usgn(holder(index_var).nodesel) SLL 1);

        mtree_var                                              := holder(index_var).mtree;
        mtree_var(update_start_bit +1 DOWNTO update_start_bit) := effective_node;

        mtree <= mtree_var;

        group_addr <= holder(index_var).gaddr;
        
      END IF;


      IF state = store THEN
        holder(to_integer(usgn(cur.verti))).mtree   <= utree;
        holder(to_integer(usgn(cur.verti))).nodesel <= gen.nodesel;
        holder(to_integer(usgn(cur.verti))).gaddr   <= group_addr;

        cur <= gen;
      END IF;  -- finish case of store
      
    END IF;  -- not resetting

  END PROCESS;


  p3 : PROCESS(mtree)                   --comb process, updates the tree
    VARIABLE vartree : std_logic_vector(31 DOWNTO 0);
  BEGIN

    FOR i IN 0 TO 3 LOOP
      vartree(6 + 2*i) := mtree(14 + 4*i) OR mtree(16 + 4*i);   -- or tree
      vartree(7 + 2*i) := mtree(15 + 4*i) AND mtree(17 + 4*i);  -- and tree`
    END LOOP;

    FOR i IN 0 TO 1 LOOP
      vartree(2 + 2*i) := vartree(6 + 4*i) OR vartree(8 + 4*i);
      vartree(3 + 2*i) := vartree(7 + 4*i) AND vartree(9 + 4*i);
    END LOOP;

    vartree(0) := vartree(2) OR vartree(4);
    vartree(1) := vartree(3) AND vartree(5);

    FOR i IN 14 TO 31 LOOP
      vartree(i) := mtree(i);
    END LOOP;

    utree <= vartree;
    
  END PROCESS;

  ram_addr <= group_addr;
  
END ARCHITECTURE;
