LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;
USE work.ALL;
USE work.budpack.ALL;

ENTITY check_blocking IS
  PORT(
    clk               : IN  std_logic;
    reset             : IN  std_logic;
    start             : IN  std_logic;
    flag_blocking_out : OUT std_logic;
    probe_in          : IN  tree_probe;
    probe_out         : OUT tree_probe;
    done_bit          : OUT std_logic;
    -- ram related
    ram_addr          : OUT std_logic_vector(31 DOWNTO 0);
    ram_data_out      : IN  std_logic_vector(31 DOWNTO 0);
    read_done         : IN  std_logic;
    read_start        : OUT std_logic
    );
END ENTITY check_blocking;

ARCHITECTURE synthe_cblock OF check_blocking IS

  ALIAS slv IS std_logic_vector;
  ALIAS usgn IS unsigned;
  TYPE StateType IS (idle, prep, s0, s1, s2, done, read_wait);
  SIGNAL state, nstate     : StateType;
  SIGNAL cur, gen          : tree_probe;
  SIGNAL group_addr        : usgn(31 DOWNTO 0);
  SIGNAL log2top_node_size : usgn(6 DOWNTO 0);
  SIGNAL flag_blocking     : std_logic;
  SIGNAL data_read         : std_logic_vector(31 DOWNTO 0);

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
      WHEN prep      => nstate <= s0;
      WHEN s0        => nstate <= read_wait;
      WHEN read_wait => nstate <= read_wait;
      WHEN s1        => nstate <= s2;
      WHEN s2        => nstate <= s0;
      WHEN done =>
        nstate   <= idle;
        done_bit <= '1';
      WHEN OTHERS => NULL;
    END CASE;
    
  END PROCESS;

  p1 : PROCESS
    VARIABLE rowbase_var           : slv(31 DOWNTO 0);
    VARIABLE log2top_node_size_var : integer RANGE 0 TO MAX_TREE_DEPTH;
  BEGIN
    WAIT UNTIL clk'event AND clk = '1';

    state      <= nstate;
    read_start <= '0';

    IF reset = '0' THEN                 -- active low
      state <= idle;
    ELSE
      
      IF state = prep THEN
        
        log2top_node_size_var := to_integer(resize(usgn(LOG2TMB) - 3* (usgn(probe_in.verti)), log2top_node_size'length));
        cur.verti             <= slv(usgn(probe_in.verti) - 1);
        cur.horiz             <= slv(usgn(probe_in.horiz) SRL 3);
        cur.nodesel           <= probe_in.horiz(2 DOWNTO 0);
        cur.saddr             <= slv(usgn(probe_in.saddr) - (usgn(probe_in.nodesel) SLL log2top_node_size_var));
        cur.rowbase           <= slv(usgn(probe_in.rowbase) + (to_unsigned(1, rowbase_var'length) SLL (to_integer(3*(usgn(probe_in.verti) - 1)))));
        cur.alvec             <= '0';

        flag_blocking <= '0';
      END IF;  -- end prep

      IF state = s0 THEN

        log2top_node_size <= resize(usgn(LOG2TMB) - 3* (usgn(cur.verti)), log2top_node_size'length);
        rowbase_var       := slv(usgn(cur.rowbase) - (to_unsigned(1, rowbase_var'length) SLL (to_integer(3*(usgn(cur.verti))))));
        gen.rowbase       <= rowbase_var;
        group_addr        <= usgn(rowbase_var) + usgn(cur.horiz);

        read_start <= '1';
        
      END IF;  -- end s0

      IF state = read_wait THEN
        IF read_done = '1' THEN
          data_read <= ram_data_out;
          state     <= s1;
        END IF;
      END IF;

      IF state = s1 THEN                -- check bits & compute gen 
        gen.alvec <= cur.alvec;
        -- selected node's AND bit = 1 ==> found blocking
        IF ram_data_out(15 + to_integer(resize(usgn(cur.nodesel), 32) SLL 1)) = '1' THEN
          flag_blocking <= '1';

          gen.verti   <= cur.verti;
          gen.horiz   <= cur.horiz;
          gen.nodesel <= cur.nodesel;
          --gen.saddr   <= slv((usgn(cur.horiz)) SLL (to_integer(log2top_node_size) -2));--cur.saddr;
          gen.saddr   <= slv((usgn(cur.horiz)) SLL (to_integer(log2top_node_size)));
          gen.rowbase <= slv(usgn(rowbase_var) - (to_unsigned(1, rowbase_var'length) SLL to_integer(3*(usgn(cur.verti)-1))));
          
          
        ELSE
          
          gen.verti   <= slv(usgn(cur.verti) - 1);
          gen.horiz   <= slv(usgn(cur.horiz) SRL 3);
          gen.nodesel <= cur.horiz(2 DOWNTO 0);  -- up nodesel = cur horiz % 8
          gen.saddr   <= slv(usgn(cur.saddr) - (resize(usgn(cur.nodesel), gen.saddr'length) SLL to_integer(log2top_node_size)));
          
          
        END IF;
      END IF;  -- end s1

      IF state = s2 THEN
        
        IF to_integer(usgn(cur.verti)) = 0 OR flag_blocking = '1' THEN
          state     <= done;
          probe_out <= gen;
        END IF;

        cur <= gen;
      END IF;  -- end s2    
      
    END IF;  -- reset
  END PROCESS;

  ram_addr          <= slv(group_addr);
  flag_blocking_out <= flag_blocking;

END ARCHITECTURE;
