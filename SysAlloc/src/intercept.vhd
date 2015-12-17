LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;
USE work.ALL;
USE work.budpack.ALL;

ENTITY intercept IS
  PORT (
    clk       : IN  std_logic;
    reset     : IN  std_logic;
    ddr_sel   : OUT std_logic;
    -- slave
    command   : IN  std_logic;          --  1 malloc, 0 free
    request   : IN  std_logic_vector(31 DOWNTO 0);
    req_valid : IN  std_logic;
    result    : OUT std_logic_vector(31 DOWNTO 0);
    res_valid : OUT std_logic;
    done_free : OUT std_logic;


    -- buddy
    buddy_start         : OUT std_logic;
    buddy_command       : OUT std_logic;
    buddy_size          : OUT std_logic_vector(31 DOWNTO 0);
    buddy_free_addr     : OUT std_logic_vector(31 DOWNTO 0);
    buddy_done          : IN  std_logic;
    buddy_malloc_addr   : IN  std_logic_vector(31 DOWNTO 0);
    buddy_malloc_failed : IN  std_logic;

    write_counter_out : OUT integer;

    -- ddr
    ddr_command    : OUT std_logic;     -- 0 = write, 1 = read
    ddr_start      : OUT std_logic;
    ddr_addr       : OUT std_logic_vector(31 DOWNTO 0);
    ddr_write_data : OUT std_logic_vector(31 DOWNTO 0);
    ddr_read_data  : IN  std_logic_vector(31 DOWNTO 0);
    ddr_done       : IN  std_logic
    );
END ENTITY intercept;

ARCHITECTURE synth_inter OF intercept IS

  ALIAS slv IS std_logic_vector;
  ALIAS usgn IS unsigned;
  
  TYPE StateType IS (idle,
                     malloc, free,
                     busy,
                     write_wait, read_wait,
                     done_state);

  SIGNAL state, nstate : StateType;

  SIGNAL buddy_size_i, buddy_free_addr_i : slv(31 DOWNTO 0);

  SIGNAL write_counter : integer;

BEGIN

  p0 : PROCESS(state)
  BEGIN

    nstate  <= idle;
    ddr_sel <= '0';

    CASE state IS
      WHEN idle =>
        nstate <= idle;
      WHEN malloc => nstate <= busy;
      WHEN free   => nstate <= read_wait;
      WHEN busy =>
        nstate  <= busy;
        ddr_sel <= '1';
      WHEN write_wait => nstate <= write_wait;
      WHEN read_wait  => nstate <= read_wait;
      WHEN done_state => nstate <= idle;
      WHEN OTHERS     => nstate <= idle;
    END CASE;


  END PROCESS;

  p1 : PROCESS
    VARIABLE saddr : usgn(31 DOWNTO 0);
  BEGIN
    WAIT UNTIL clk'event AND clk = '1';
    write_counter <= write_counter + 1;
    state         <= nstate;
    buddy_start   <= '0';
    done_free     <= '0';
    res_valid     <= '0';
    ddr_start     <= '0';

    IF reset = '0' THEN                 -- active low
      state        <= idle;
      buddy_size_i <= (OTHERS => '0');

      write_counter <= 0;
    ELSE



      IF state = idle THEN
        
        IF req_valid = '1' THEN
          
          write_counter <= 0;

          state        <= malloc;
          buddy_size_i <= slv((usgn(request) + 3 + usgn(BLOCK_SIZE)) SRL LOG2BLOCK_SIZE);
          IF command = '0' THEN
            state <= free;
          END IF;
        END IF;
        
      END IF;


      IF state = malloc THEN

        buddy_command <= '1';
        -- size = ceil((req+4)/bsize) = floor((req+4+bsize-1)/bsize)
        -- buddy_size_i  <= slv((usgn(request) + 3 + usgn(BLOCK_SIZE)) SRL LOG2BLOCK_SIZE);
        buddy_start   <= '1';
      END IF;

      IF state = busy THEN
        
        IF buddy_done = '1' THEN

          IF command = '1' THEN
            IF buddy_malloc_failed = '0' THEN
              -- performance

              --
              state          <= write_wait;  -- if malloc            
              saddr          := (usgn(buddy_malloc_addr) SLL (LOG2BLOCK_SIZE)) + usgn(DDR_BASE);
              result         <= slv(saddr + 4);
              -- write to ddr
              ddr_command    <= '0';
              ddr_start      <= '1';
              ddr_addr       <= slv(saddr);
              ddr_write_data <= buddy_size_i;
            ELSE                             -- malloc failed
              state  <= done_state;
              result <= (OTHERS => '1');     -- indicating allocation failed
            END IF;
          END IF;

          IF command = '0' THEN
            state     <= done_state;
            done_free <= '1';
            --res_valid <= '1';
            
          END IF;

        END IF;  -- end buddy done

      END IF;  -- end state busy

      IF state = write_wait THEN
        IF ddr_done = '1' THEN
          -- performance
          write_counter_out <= write_counter;
          --             
          state             <= done_state;
          res_valid         <= '1';
        END IF;
      END IF;

      IF state = free THEN
        saddr       := usgn(request) - 4;
        ddr_command <= '1';
        ddr_start   <= '1';
        ddr_addr    <= slv(saddr);
      END IF;  -- end free

      IF state = read_wait THEN
        IF ddr_done = '1' THEN
          state             <= busy;
          buddy_command     <= '0';
          buddy_start       <= '1';
          buddy_size_i      <= ddr_read_data;
          buddy_free_addr_i <= slv((saddr - usgn(DDR_BASE)) SRL (LOG2BLOCK_SIZE));
          result            <= slv((saddr - usgn(DDR_BASE)) SRL (LOG2BLOCK_SIZE));  --------------
        END IF;
      END IF;

    END IF;  -- end reset
    
  END PROCESS;

  buddy_size      <= buddy_size_i;
  buddy_free_addr <= buddy_free_addr_i;
END ARCHITECTURE;
