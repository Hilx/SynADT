LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;
USE work.ALL;
USE work.budpack.ALL;

ENTITY free_info IS
  PORT(
    clk                   : IN  std_logic;
    reset                 : IN  std_logic;
    start                 : IN  std_logic;
    address               : IN  std_logic_vector(31 DOWNTO 0);
    size                  : IN  std_logic_vector(31 DOWNTO 0);
    probe_out             : OUT tree_probe;
    done_bit              : OUT std_logic;
    top_node_size_out     : OUT std_logic_vector(31 DOWNTO 0);
    log2top_node_size_out : OUT std_logic_vector(6 DOWNTO 0);
    group_addr_out        : OUT std_logic_vector(31 DOWNTO 0);
    vg_addr               : OUT std_logic_vector(31 DOWNTO 0)
    );
END ENTITY free_info;

ARCHITECTURE synth_free_info OF free_info IS

  ALIAS slv IS std_logic_vector;
  ALIAS usgn IS unsigned;

  TYPE StateType IS (idle, s0, s1, capture, done);
  SIGNAL state, nstate : StateType;

  SIGNAL top_node_size     : usgn(31 DOWNTO 0);
  SIGNAL log2top_node_size : usgn(6 DOWNTO 0);
  SIGNAL verti             : usgn(31 DOWNTO 0);
  SIGNAL horiz             : usgn(31 DOWNTO 0);
  SIGNAL rowbase           : usgn(31 DOWNTO 0);
  SIGNAL nodesel           : usgn(2 DOWNTO 0);
  SIGNAL alvec             : std_logic;
  SIGNAL group_addr        : usgn(31 DOWNTO 0);

BEGIN

  p0 : PROCESS(state, start)
  BEGIN

    nstate   <= idle;
    done_bit <= '0';

    CASE state IS
      WHEN idle =>
        nstate <= idle;
        IF start = '1' THEN
          nstate <= s0;
        END IF;
      WHEN s0      => nstate <= s0;
      WHEN s1      => nstate <= capture;
      WHEN capture => nstate <= done;
      WHEN done =>
        nstate   <= idle;
        done_bit <= '1';
      WHEN OTHERS => NULL;
    END CASE;
    
  END PROCESS;

  p1 : PROCESS
    VARIABLE rowbase_var, horiz_var         : usgn(31 DOWNTO 0);
    VARIABLE alvec_var                      : std_logic;
    VARIABLE nodesel_inter0, nodesel_inter1 : usgn(31 DOWNTO 0) := (OTHERS => '0');
    VARIABLE rang1                          : integer;

  BEGIN
    WAIT UNTIL clk'event AND clk = '1';

    state <= nstate;

    IF reset = '0' THEN                 -- active low
      state <= idle;
    ELSE

      IF state = idle THEN              -- initialise
        
        top_node_size     <= usgn(TOTAL_MEM_BLOCKS);
        log2top_node_size <= usgn(LOG2TMB);
        verti             <= (OTHERS => '0');
        horiz             <= (OTHERS => '0');
        nodesel           <= (OTHERS => '0');
        rowbase           <= (OTHERS => '0');
        alvec             <= '0';
        alvec_var         := '0';

      END IF;

      IF state = s0 THEN
        
        state                     <= s1;
        IF to_integer(usgn(size)) <= to_integer(top_node_size SRL 4) THEN
          state             <= nstate;
          verti             <= verti + 1;
          top_node_size     <= top_node_size SRL 3;
          log2top_node_size <= log2top_node_size - 3;
          rowbase           <= rowbase + (to_unsigned(1, rowbase'length) SLL (to_integer(3 * (verti - 1))));
        END IF;
        
        
      END IF;  -- end state = s0

      IF state = s1 THEN
        horiz     <= usgn(address) SRL to_integer(log2top_node_size);
        horiz_var := usgn(address) SRL to_integer(log2top_node_size);

        --        -- to_integer(log2top_node_size - 1)
        nodesel_inter0 := (usgn(address) SRL to_integer(log2top_node_size));
        nodesel_inter1 := usgn(address) - (nodesel_inter0 SLL to_integer(log2top_node_size));
        nodesel        <= resize(nodesel_inter1 SRL to_integer(log2top_node_size - 3), nodesel'length);

        IF to_integer(top_node_size) = 2 THEN
          nodesel   <= resize(usgn(address(1 DOWNTO 1)), nodesel'length);
          alvec     <= '1';
          alvec_var := '1';
        ELSIF to_integer(top_node_size) = 4 THEN
          nodesel <= resize(usgn(address(1 DOWNTO 0)), nodesel'length) SLL 1;
        END IF;

        rowbase_var := rowbase + (to_unsigned(1, rowbase'length) SLL (to_integer(3 * (verti - 1))));

        IF alvec_var = '0' THEN
          group_addr <= rowbase_var + horiz;
        ELSE
          group_addr <= resize(horiz(3 DOWNTO 0), 32);  -- group addr = horiz % 16
        END IF;

        vg_addr <= slv(rowbase_var + horiz);
        
      END IF;  -- end state = s1

      IF state = capture THEN

        probe_out.verti       <= slv(verti);
        probe_out.horiz       <= slv(horiz);
        probe_out.nodesel     <= slv(nodesel);
        probe_out.rowbase     <= slv(rowbase);
        probe_out.saddr       <= address;
        probe_out.alvec       <= alvec;
        top_node_size_out     <= slv(top_node_size);
        log2top_node_size_out <= slv(log2top_node_size);
        group_addr_out        <= slv(group_addr);
        
      END IF;

    END IF;  -- end reset
    
  END PROCESS;



END ARCHITECTURE;
