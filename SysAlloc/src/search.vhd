LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;
USE work.ALL;
USE work.budpack.ALL;

ENTITY locator IS
  PORT(
    clk             : IN  std_logic;
    reset           : IN  std_logic;
    start           : IN  std_logic;
    probe_in        : IN  tree_probe;
    size            : IN  std_logic_vector(31 DOWNTO 0);
    direction_in    : IN  std_logic;    -- DOWN = 0, UP = 1
    probe_out       : OUT tree_probe;
    done_bit        : OUT std_logic;
    ram_addr        : OUT std_logic_vector(31 DOWNTO 0);
    ram_data_out    : IN  std_logic_vector(31 DOWNTO 0);
    flag_failed_out : OUT std_logic;
    vg_addr         : OUT std_logic_vector(31 DOWNTO 0);
    read_done       : IN  std_logic;
    read_start      : OUT std_logic
    );
END ENTITY locator;

ARCHITECTURE synth_locator OF locator IS
  ALIAS slv IS std_logic_vector;
  ALIAS usgn IS unsigned;
  TYPE StateType IS (idle, prep, s0, s1, s2, s3, done);
  SIGNAL state, nstate            : StateType;
  SIGNAL search_status            : std_logic;
  SIGNAL top_node_size            : std_logic_vector(31 DOWNTO 0);
  SIGNAL log2top_node_size        : std_logic_vector(6 DOWNTO 0);  -- range > 32
  SIGNAL group_addr               : std_logic_vector(31 DOWNTO 0);
  SIGNAL mtree, utree             : std_logic_vector(31 DOWNTO 0);
  SIGNAL flag_found               : std_logic;
  SIGNAL cur                      : tree_probe;
  SIGNAL gen                      : tree_probe;
  SIGNAL gen_direction, direction : std_logic;  -- DOWN = 0, UP = 1 
  SIGNAL local_and_tree           : std_logic_vector(14 DOWNTO 0);
  SIGNAL flag_failed              : std_logic;

BEGIN

  P0 : PROCESS(state, start, search_status, flag_failed)
  BEGIN
    
    nstate   <= idle;
    done_bit <= '0';
    CASE state IS
      WHEN idle =>

        nstate <= idle;
        IF start = '1' THEN
          nstate <= prep;
        END IF;
      WHEN prep => nstate <= s0;
      WHEN s0   => nstate <= s1;
      WHEN s1   => nstate <= s1;
      WHEN s2   => nstate <= s3;
      WHEN s3 =>
        
        nstate <= s0;
        IF search_status = '1' OR flag_failed = '1' THEN
          nstate   <= done;
          done_bit <= '1';
        END IF;
        
      WHEN done =>
        nstate   <= idle;
        done_bit <= '1';
      WHEN OTHERS => NULL;
    END CASE;

  END PROCESS;

  P1 : PROCESS
    VARIABLE rowbase_var    : slv(31 DOWNTO 0);
    VARIABLE mtree_var      : slv(31 DOWNTO 0);
    VARIABLE ns_var         : std_logic;
    VARIABLE ns_var2        : slv(1 DOWNTO 0);
    VARIABLE flag_found_var : std_logic;
    VARIABLE nodesel_var    : slv(2 DOWNTO 0);
  BEGIN
    WAIT UNTIL clk'event AND clk = '1';

    IF reset = '0' THEN                 -- active low
      state <= idle;
    ELSE
      state      <= nstate;
      read_start <= '0';

      IF state = prep THEN
        search_status <= '0';
        flag_failed   <= '0';
        cur           <= probe_in;
        direction     <= direction_in;
      END IF;

      IF state = s0 THEN
        
        flag_found     <= '0';
        flag_found_var := '0';
        gen.alvec      <= '0';


        top_node_size     <= slv(usgn(TOTAL_MEM_BLOCKS) SRL (to_integer(3*(usgn(cur.verti)))));
        log2top_node_size <= slv(resize(usgn(LOG2TMB) - 3* (usgn(cur.verti)), log2top_node_size'length));

        IF to_integer(usgn(cur.verti)) = 0 THEN
          rowbase_var := (OTHERS => '0');
        ELSE
          IF direction = '0' THEN       -- DOWN          
            rowbase_var := slv(usgn(cur.rowbase) + (to_unsigned(1, rowbase_var'length) SLL (to_integer(3*(usgn(cur.verti)-1)))));  -- calculating the current row base       
          ELSE                          -- UP                  
            rowbase_var := slv(usgn(cur.rowbase) - (to_unsigned(1, rowbase_var'length) SLL (to_integer(3*(usgn(cur.verti))))));
          END IF;
        END IF;
        gen.rowbase <= rowbase_var;

        IF cur.alvec = '0' THEN
          group_addr <= slv(usgn(rowbase_var) + usgn(cur.horiz));  -- rowbase_var before!      
        ELSE                            -- in allocation vector
          group_addr <= slv(usgn(ALVEC_SHIFT) +(usgn(cur.horiz) SRL 4));
          vg_addr    <= slv(usgn(rowbase_var) + usgn(cur.horiz));
        END IF;

        read_start <= '1';

      END IF;

      IF state = s1 THEN
        IF read_done = '1' THEN
          mtree <= ram_data_out;
          state <= s2;
        END IF;
      END IF;

      IF state = s2 THEN

        IF size <= slv(usgn(top_node_size) SRL 4) THEN  --topsize/16
          
          IF direction = '1' THEN       -- UP
            mtree_var := utree;
          ELSE                          -- DOWN
            mtree_var := mtree;
          END IF;


          flag_found_var := '0';
          IF(mtree_var(15) AND mtree_var(17)
             AND mtree_var(19) AND mtree_var(21)
             AND mtree_var(23) AND mtree_var(25)
             AND mtree_var(27) AND mtree_var(29)) = '0' THEN
            flag_found_var := '1';
          END IF;
          flag_found <= flag_found_var;

          IF flag_found_var = '1' THEN

            ------------------------ find starting address
            nodesel_var(2) := mtree_var(3);
            ns_var         := nodesel_var(2);
            IF ns_var = '0' THEN
              nodesel_var(1) := mtree_var(7);
            ELSE
              nodesel_var(1) := mtree_var(11);
            END IF;
            ns_var2 := nodesel_var(2 DOWNTO 1);
            CASE ns_var2 IS
              WHEN "00" =>
                nodesel_var(0) := mtree_var(15);
              WHEN "01" =>
                nodesel_var(0) := mtree_var(19);
              WHEN "10" =>
                nodesel_var(0) := mtree_var(23);
              WHEN "11" =>
                nodesel_var(0) := mtree_var(27);
              WHEN OTHERS => NULL;
            END CASE;
            ----------------------------------------------
            -- flag_found = 1
            gen.verti <= slv(usgn(cur.verti) + 1);
            gen.horiz <= slv((usgn(cur.horiz) SLL 3) + usgn(nodesel_var));
            gen.saddr <= slv(usgn(cur.saddr) + (resize(usgn(nodesel_var), 32) SLL to_integer(usgn(log2top_node_size) - 3)));

            IF to_integer(usgn(top_node_size) SRL 4) = 1 THEN
              gen.alvec <= '1';
            END IF;

            gen_direction <= '0';       -- go down
            
          ELSE

            IF to_integer(usgn(cur.verti)) = 0 THEN
              flag_failed <= '1';
            ELSE
              gen.verti     <= slv(usgn(cur.verti) - 1);
              gen.horiz     <= slv(usgn(cur.horiz) SRL 3);
              gen_direction <= '1';     -- go up
              nodesel_var   := slv(resize(usgn(cur.horiz(2 DOWNTO 0)), gen.nodesel'length));
              gen.saddr     <= slv(usgn(cur.saddr) - (resize(usgn(nodesel_var), 32) SLL to_integer(usgn(log2top_node_size))));  -- which nodesel to use?
            END IF;

          END IF;

        ELSE
          
          IF cur.alvec = '1' THEN       -- using allocation vector
            
            
            gen.alvec      <= '1';
            search_status  <= '1';
            flag_found     <= '1';
            flag_found_var := '1';

            IF mtree(to_integer(resize(usgn(cur.horiz(3 DOWNTO 0)), 6) SLL 1)) = '0' THEN
              nodesel_var := "000";
              gen.rowbase <= cur.rowbase;  -- keep that input rowbase if search done
            ELSIF mtree(to_integer((resize(usgn(cur.horiz(3 DOWNTO 0)), 6) SLL 1)+ 1)) = '0' THEN
              nodesel_var := "001";
              gen.rowbase <= cur.rowbase;  -- keep that input rowbase if search done

            ELSE
              flag_found     <= '0';
              flag_found_var := '0';
              search_status  <= '0';
            END IF;
            
          END IF;

          IF cur.alvec = '0' THEN  -- cur.alvec= 0, not using allocation vector
            -- allocation be decided in this level
            -- can only go up if no available node
            IF size <= slv(usgn(top_node_size) SRL 3) THEN  -- topsize/8

              flag_found     <= '0';
              flag_found_var := '0';
              IF(mtree(14) AND mtree(16) AND mtree(18) AND mtree(20)
                 AND mtree(22) AND mtree(24) AND mtree(26) AND mtree(28)) = '0' THEN
                flag_found     <= '1';
                flag_found_var := '1';
              END IF;

              IF flag_found_var = '1' THEN

                ------------------------ find starting address
                nodesel_var(2) := mtree(14) AND mtree(16) AND mtree(18) AND mtree(20);
                ns_var         := nodesel_var(2);
                IF ns_var = '0' THEN
                  nodesel_var(1) := mtree(14) AND mtree(16);
                ELSE
                  nodesel_var(1) := mtree(22) AND mtree(24);
                END IF;
                ns_var2 := nodesel_var(2 DOWNTO 1);
                CASE ns_var2 IS
                  WHEN "00" =>
                    nodesel_var(0) := mtree(14);
                  WHEN "01" =>
                    nodesel_var(0) := mtree(18);
                  WHEN "10" =>
                    nodesel_var(0) := mtree(22);
                  WHEN "11" =>
                    nodesel_var(0) := mtree(26);
                  WHEN OTHERS => NULL;
                END CASE;
              ----------------------------------------------
              END IF;
              
              
            ELSIF size <= slv(usgn(top_node_size) SRL 2) THEN  -- topsize /4     
              -- find the first OR bit in local depth = 2

              flag_found     <= '0';
              flag_found_var := '0';
              IF(mtree(6) AND mtree(8) AND mtree(10) AND mtree(12)) = '0' THEN
                
                flag_found     <= '1';
                flag_found_var := '1';
              END IF;

              IF flag_found_var = '1' THEN

                ------------------------ find starting address
                nodesel_var(2) := mtree(6) AND mtree(8);
                ns_var         := nodesel_var(2);
                IF ns_var = '0' THEN
                  nodesel_var(1) := mtree(6);
                ELSE
                  nodesel_var(1) := mtree(10);
                END IF;
                nodesel_var(0) := '0';
                ----------------------------------------------

              END IF;
              
            ELSIF size <= slv(usgn(top_node_size) SRL 1) THEN  -- topsize/2 
              -- find the first OR bit in local depth = 1
              
              flag_found     <= '0';
              flag_found_var := '0';
              IF(mtree(2) AND mtree(4)) = '0' THEN
                flag_found     <= '1';
                flag_found_var := '1';
              END IF;

              IF flag_found_var = '1' THEN

                ------------------------ find starting address
                nodesel_var(2) := mtree(2);
                nodesel_var(1) := '0';
                nodesel_var(0) := '0';
                ----------------------------------------------                 
                
              END IF;
              
            ELSE                        -- final else
                                        -- 
              flag_found     <= NOT mtree(0);
              flag_found_var := NOT mtree(0);

              IF flag_found_var = '1' THEN
                nodesel_var := "000";
              END IF;
              
            END IF;

          END IF;

          IF flag_found_var = '1' THEN
            
            search_status <= '1';
            gen.rowbase   <= cur.rowbase;  -- keep that input rowbase if search done

            gen.verti     <= cur.verti;
            gen.horiz     <= cur.horiz;
            gen_direction <= '0';       -- go down

            IF cur.alvec = '0' THEN     -- not use alvec

              IF to_integer(usgn(top_node_size)) = 4 THEN
                gen.saddr <= slv(usgn(cur.saddr) + (usgn(nodesel_var) SRL 1));  -- gen.nodesel?
                
              ELSE
                gen.saddr <= slv(usgn(cur.saddr) + (resize(usgn(nodesel_var), gen.saddr'length) SLL to_integer(usgn(log2top_node_size) - 3)));  -- gen.nodesel?
                
              END IF;

            ELSE                        -- using alvec              
              gen.saddr <= slv(usgn(cur.saddr) + usgn(nodesel_var));
            END IF;
            
          ELSE                          -- not found
            
            IF to_integer(usgn(cur.verti)) = 0 THEN
              flag_failed <= '1';
            ELSE                        -- GO UP
              gen.verti     <= slv(usgn(cur.verti) - 1);
              gen.horiz     <= slv(usgn(cur.horiz) SRL 3);
              gen_direction <= '1';     -- UP = 1
              nodesel_var   := slv(resize(usgn(cur.horiz(2 DOWNTO 0)), gen.nodesel'length));

              gen.saddr <= slv(usgn(cur.saddr) - (resize(usgn(cur.nodesel), 32) SLL to_integer(usgn(log2top_node_size))));
            END IF;  -- verti = or != 0
          END IF;  -- flag found or not
        END IF;

        gen.nodesel <= nodesel_var;
      END IF;

      IF state = s3 THEN
        
        IF search_status = '0' THEN     -- continue the search          
          cur       <= gen;
          direction <= gen_direction;
        END IF;
        
      END IF;

    END IF;
    
  END PROCESS;

  P2 : PROCESS(mtree, cur.nodesel)
    VARIABLE mtree_var : slv(31 DOWNTO 0);
  BEGIN

    -- update mtree based on depth 3
    -- 0, 1, 2,3
    mtree_var := mtree;

    FOR i IN 15 TO 29 LOOP
      IF i <= to_integer(15 + (resize(usgn(cur.nodesel), 5) SLL 1))THEN
        mtree_var(i) := '1';
      ELSE
        mtree_var(i) := mtree(i);
      END IF;
    END LOOP;

    FOR i IN 0 TO 3 LOOP
      mtree_var(7 + 2*i) := mtree_var(15 + 4*i) AND mtree_var(17 + 4*i);
    END LOOP;

    FOR i IN 0 TO 1 LOOP
      mtree_var(3 + 2*i) := mtree_var(7 + 4*i) AND mtree_var(9 + 4*i);
    END LOOP;

    mtree_var(1) := mtree_var(3) AND mtree_var(5);

    utree <= mtree_var;
    
  END PROCESS;

  p3 : PROCESS(mtree)
    VARIABLE local_var : slv(14 DOWNTO 0);
  BEGIN
    
    FOR i IN 0 TO 7 LOOP
      local_var(i + 7) := mtree(2*i + 14);
    END LOOP;

    FOR i IN 0 TO 3 LOOP
      local_var(i + 3) := local_var(7 + 2*i) AND local_var(8 + 2*i);
    END LOOP;

    FOR i IN 0 TO 1 LOOP
      local_var(i + 1) := local_var(3 + 2*i) AND local_var(4 + 2*i);
    END LOOP;

    local_var(0) := local_var(1) AND local_var(2);

    local_and_tree <= local_var;
  END PROCESS;


  ram_addr        <= group_addr;
  probe_out       <= gen;
  flag_failed_out <= flag_failed;
  
END ARCHITECTURE;
