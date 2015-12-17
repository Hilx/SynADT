LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;
USE work.ALL;
USE work.budpack.ALL;

ENTITY up_marker IS
  PORT(
    clk          : IN  std_logic;
    reset        : IN  std_logic;
    start        : IN  std_logic;
    probe_in     : IN  tree_probe;
    node_in      : IN  std_logic_vector(1 DOWNTO 0);
    done_bit     : OUT std_logic;
    ram_we       : OUT std_logic;       -- write_start
    ram_addr     : OUT std_logic_vector(31 DOWNTO 0);
    ram_data_in  : OUT std_logic_vector(31 DOWNTO 0);
    ram_data_out : IN  std_logic_vector(31 DOWNTO 0);
    read_done    : IN  std_logic;
    write_done   : IN  std_logic;
    read_start   : OUT std_logic
    );
END ENTITY up_marker;

ARCHITECTURE synth_umark OF up_marker IS

  ALIAS slv IS std_logic_vector;
  ALIAS usgn IS unsigned;

  TYPE StateType IS (idle, prep, s0, s_read, s_w0, s_w1, done, read_wait, write_wait);
  SIGNAL state, nstate     : StateType;
  SIGNAL mtree, utree      : std_logic_vector(31 DOWNTO 0);
  SIGNAL cur, gen          : tree_probe;
  SIGNAL group_addr        : std_logic_vector(31 DOWNTO 0);
  SIGNAL node_propa        : std_logic_vector(1 DOWNTO 0);
  SIGNAL original_top_node : std_logic_vector(1 DOWNTO 0);
  SIGNAL index             : integer RANGE 0 TO 15;

  SIGNAL read_data : std_logic_vector(31 DOWNTO 0);
BEGIN

  p0 : PROCESS(state, start)
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
      WHEN s0         => nstate <= read_wait;
      WHEN read_wait  => nstate <= read_wait;
      WHEN s_read     => nstate <= s_w0;
      WHEN s_w0       => nstate <= write_wait;
      WHEN write_wait => nstate <= write_wait;
      WHEN s_w1       => nstate <= s0;
      WHEN done       => nstate <= idle;
                         done_bit <= '1';
      WHEN OTHERS => NULL;
    END CASE;
    
  END PROCESS;

  p1 : PROCESS

  BEGIN
    WAIT UNTIL clk'event AND clk = '1';

    state      <= nstate;
    read_start <= '0';

    IF reset = '0' THEN                 -- active low
      state <= idle;
    ELSE

      IF state = prep THEN

        cur        <= probe_in;
        node_propa <= node_in;
      END IF;

      IF state = s0 THEN
        
        gen.verti   <= slv(usgn(cur.verti) - 1);
        gen.horiz   <= slv(usgn(cur.horiz) SRL 3);  -- input.horiz/8
        -- output row base = input row base - 2^(output verti- 1)
        -- output row base = input row base - 2^(input verti - 2)               
        gen.rowbase <= slv(usgn(cur.rowbase) - (to_unsigned(1, gen.rowbase'length) SLL to_integer(3 * (usgn(cur.verti) - 2))));
        group_addr  <= slv(usgn(cur.rowbase) + (usgn(cur.horiz) SRL 3));
        -- index = 14 + (input horiz % 8) * 2
        index       <= to_integer((resize(usgn(cur.horiz(2 DOWNTO 0)), 32) SLL 1));

        read_start <= '1';
      END IF;  -- finish state = s0

      IF state = read_wait THEN
        
        IF read_done = '1' THEN
          
          read_data <= ram_data_out;
          state     <= s_read;
          
        END IF;
        
      END IF;

      IF state = s_read THEN
        original_top_node <= read_data(1 DOWNTO 0);  -- keep a copy of the original top node
        FOR i IN 0 TO 31 LOOP
          mtree(i) <= read_data(i);
          IF i = index + 14 THEN
            mtree(i) <= node_propa(0);
          END IF;
          IF i = index + 15 THEN
            mtree(i) <= node_propa(1);
          END IF;
        END LOOP;
      END IF;  -- finish state - s_read

      ram_we <= '0';
      IF state = s_w0 THEN
        node_propa  <= utree(1 DOWNTO 0);
        ram_data_in <= utree;

        ram_we <= '1';
      END IF;

      IF state = write_wait THEN
        
        IF write_done = '1' THEN
          state <= s_w1;
        END IF;
        
      END IF;


      IF state = s_w1 THEN


        IF to_integer(usgn(gen.verti)) = 0 OR original_top_node = node_propa THEN
          -- no more upward propagation
          state <= done;
        END IF;

        cur <= gen;
      END IF;
      
      
      
    END IF;
    
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
