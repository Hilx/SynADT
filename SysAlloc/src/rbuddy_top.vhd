-- RAM-based Buddy Allocator
-- Created by Hilda Xue, 24 Feb 2015
-- This file contains the top level of the buddy allocator

LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;
USE work.ALL;
USE work.budpack.ALL;

ENTITY rbuddy_top IS
  PORT(
    init_start_bit    : IN  std_logic;
    init_finished_bit : OUT std_logic;

    clk           : IN  std_logic;
    reset         : IN  std_logic;
    start         : IN  std_logic;
    cmd           : IN  std_logic;      -- 0 = free, 1 = alloc
    size          : IN  std_logic_vector(31 DOWNTO 0);
    free_addr     : IN  std_logic_vector(31 DOWNTO 0);
    done          : OUT std_logic;
    malloc_addr   : OUT std_logic_vector(31 DOWNTO 0);
    malloc_failed : OUT std_logic;      -- 1 when failed

    counter_out, counter_out2 : OUT std_logic_vector(31 DOWNTO 0);

    ddr_command    : OUT std_logic;     -- 0 = write, 1 = read
    ddr_start      : OUT std_logic;
    ddr_addr       : OUT std_logic_vector(31 DOWNTO 0);
    ddr_write_data : OUT std_logic_vector(31 DOWNTO 0);
    ddr_read_data  : IN  std_logic_vector(31 DOWNTO 0);
    ddr_done       : IN  std_logic
    );
END ENTITY rbuddy_top;

ARCHITECTURE synth OF rbuddy_top IS
  
  SIGNAL init_start_wire : std_logic;

  ALIAS slv IS std_logic_vector;
  ALIAS usgn IS unsigned;

  TYPE StateType IS (idle,
                     init0, init_enable, init1, init2, init_check,
                     malloc0, malloc1,  -- preprocess info for search in malloc
                     free,              -- preprocess info for free
                     search,            -- search for malloc deciding group
                     track,             -- update tracker
                     downmark,          -- downward marking
                     upmark,            -- upward marking
                     done_state);
  SIGNAL state, nstate : StateType;

  -- init
  SIGNAL init_addr     : std_logic_vector(31 DOWNTO 0);
  SIGNAL init_we       : std_logic;
  SIGNAL init_data_in  : std_logic_vector(31 DOWNTO 0);
  SIGNAL init_done     : std_logic;
  SIGNAL init_addr_int : integer;
  -- ram0
  SIGNAL ram0_we       : std_logic;
  SIGNAL ram0_addr     : std_logic_vector(31 DOWNTO 0);
  SIGNAL ram0_data_in  : std_logic_vector(31 DOWNTO 0);
  SIGNAL ram0_data_out : std_logic_vector(31 DOWNTO 0);

  -- wires connecting ram0 from each modules
  SIGNAL search0_addr     : std_logic_vector(31 DOWNTO 0);
  SIGNAL search0_data_out : std_logic_vector(31 DOWNTO 0);

  SIGNAL down0_we       : std_logic;
  SIGNAL down0_addr     : std_logic_vector(31 DOWNTO 0);
  SIGNAL down0_data_in  : std_logic_vector(31 DOWNTO 0);
  SIGNAL down0_data_out : std_logic_vector(31 DOWNTO 0);

  SIGNAL up0_we       : std_logic;
  SIGNAL up0_addr     : std_logic_vector(31 DOWNTO 0);
  SIGNAL up0_data_in  : std_logic_vector(31 DOWNTO 0);
  SIGNAL up0_data_out : std_logic_vector(31 DOWNTO 0);

  SIGNAL search_start_probe : tree_probe;
  SIGNAL search_done_probe  : tree_probe;
  SIGNAL search_done_bit    : std_logic;
  SIGNAL flag_malloc_failed : std_logic;
  SIGNAL start_search       : std_logic;

  SIGNAL start_dmark        : std_logic;
  SIGNAL flag_alloc         : std_logic;
  SIGNAL dmark_start_probe  : tree_probe;
  SIGNAL dmark_done_bit     : std_logic;
  SIGNAL down_node_out      : std_logic_vector(1 DOWNTO 0);
  SIGNAL up_done_bit        : std_logic;
  SIGNAL start_upmark       : std_logic;
  SIGNAL upmark_start_probe : tree_probe;
  SIGNAL flag_markup        : std_logic;

  SIGNAL start_top_node_size     : std_logic_vector(31 DOWNTO 0);
  SIGNAL start_log2top_node_size : std_logic_vector(6 DOWNTO 0);

  SIGNAL start_free_info     : std_logic;
  SIGNAL free_info_probe_out : tree_probe;
  SIGNAL free_info_done_bit  : std_logic;
  SIGNAL free_tns            : std_logic_vector(31 DOWNTO 0);
  SIGNAL free_log2tns        : std_logic_vector(6 DOWNTO 0);
  SIGNAL free_group_addr     : std_logic_vector(31 DOWNTO 0);

  -- tracker ram
  SIGNAL tracker_we       : std_logic;
  SIGNAL tracker_addr     : integer RANGE 0 TO 31;
  SIGNAL tracker_data_in  : std_logic_vector(31 DOWNTO 0);
  SIGNAL tracker_data_out : std_logic_vector(31 DOWNTO 0);

  SIGNAL group_addr_to_tracker : std_logic_vector(31 DOWNTO 0);
  SIGNAL tracker_func_sel      : std_logic;  -- 0 = update, 1 = make probe_in
  SIGNAL tracker_done          : std_logic;
  SIGNAL tracker_probe_out     : tree_probe;

  SIGNAL start_check_blocking : std_logic;
  SIGNAL flag_blocking        : std_logic;
  SIGNAL cblock_probe_in      : tree_probe;
  SIGNAL cblock_probe_out     : tree_probe;
  SIGNAL cblock_done          : std_logic;
  SIGNAL cblock_ram_addr      : std_logic_vector(31 DOWNTO 0);
  SIGNAL cblock_ram_data_out  : std_logic_vector(31 DOWNTO 0);

  SIGNAL start_tracker : std_logic;

  SIGNAL vg_addr_malloc : std_logic_vector(31 DOWNTO 0);
  SIGNAL vg_addr_free   : std_logic_vector(31 DOWNTO 0);

  -- for ddr/bram partition
  SIGNAL search_read_done  : std_logic;
  SIGNAL search_read_start : std_logic;

  SIGNAL down_read_done  : std_logic;
  SIGNAL down_read_start : std_logic;
  SIGNAL down_write_done : std_logic;

  SIGNAL up_read_done  : std_logic;
  SIGNAL up_read_start : std_logic;
  SIGNAL up_write_done : std_logic;

  SIGNAL cb_read_start : std_logic;
  SIGNAL cb_read_done  : std_logic;

  -- ram control signals
  SIGNAL control_we         : std_logic;  -- control_write_start
  SIGNAL control_addr       : std_logic_vector(31 DOWNTO 0);
  SIGNAL control_data_in    : std_logic_vector(31 DOWNTO 0);
  SIGNAL control_data_out   : std_logic_vector(31 DOWNTO 0);
  SIGNAL control_write_done : std_logic;
  SIGNAL control_read_start : std_logic;
  SIGNAL control_read_done  : std_logic;

  SIGNAL tracker_we_wire : std_logic_vector(0 DOWNTO 0);
  SIGNAL ram0_we_wire    : std_logic_vector(0 DOWNTO 0);

  SIGNAL wait_flag       : std_logic;
  SIGNAL write_wait_flag : std_logic;

  SIGNAL counter_search : integer;
  SIGNAL counter_down   : integer;
  SIGNAL counter_up     : integer;

  SIGNAL counter_malloc0 : integer;
  SIGNAL counter_malloc1 : integer;
  SIGNAL counter_track   : integer;

  SIGNAL counter_latency : integer;

  COMPONENT TrackerRam IS
    PORT (
      clka  : IN  std_logic;
      wea   : IN  std_logic_vector(0 DOWNTO 0);
      addra : IN  std_logic_vector(4 DOWNTO 0);
      dina  : IN  std_logic_vector(31 DOWNTO 0);
      douta : OUT std_logic_vector(31 DOWNTO 0)
      );
  END COMPONENT;

  COMPONENT TreeBram IS
    PORT (
      clka  : IN  std_logic;
      wea   : IN  std_logic_vector(0 DOWNTO 0);
      addra : IN  std_logic_vector(14 DOWNTO 0);
      dina  : IN  std_logic_vector(31 DOWNTO 0);
      douta : OUT std_logic_vector(31 DOWNTO 0)
      );
  END COMPONENT;
  
BEGIN

  TreeBram_inst : TreeBram
    PORT MAP(
      clka  => clk,
      wea   => ram0_we_wire,
      addra => ram0_addr(14 DOWNTO 0),
      dina  => ram0_data_in,
      douta => ram0_data_out
      );

  TrackerRam_inst : TrackerRam
    PORT MAP (
      clka  => clk,
      wea   => tracker_we_wire,
      addra => slv(to_unsigned(tracker_addr, 5)),
      dina  => tracker_data_in,
      douta => tracker_data_out
      );          

  Searcher : ENTITY locator
    PORT MAP(
      clk             => clk,
      reset           => reset,
      start           => start_search,
      probe_in        => search_start_probe,
      size            => size,
      direction_in    => '0',           -- start direction is always DOWN
      probe_out       => search_done_probe,
      done_bit        => search_done_bit,
      ram_addr        => search0_addr,
      ram_data_out    => search0_data_out,
      flag_failed_out => flag_malloc_failed,
      vg_addr         => vg_addr_malloc,
      read_start      => search_read_start,
      read_done       => search_read_done
      );

  DownMarker : ENTITY down_marker
    PORT MAP(
      clk          => clk,
      reset        => reset,
      start        => start_dmark,
      flag_alloc   => flag_alloc,
      probe_in     => dmark_start_probe,
      reqsize      => size,
      done_bit     => dmark_done_bit,
      ram_we       => down0_we,
      ram_addr     => down0_addr,
      ram_data_in  => down0_data_in,
      ram_data_out => down0_data_out,
      node_out     => down_node_out,
      flag_markup  => flag_markup,
      read_start   => down_read_start,
      read_done    => down_read_done,
      write_done   => down_write_done
      );

  UpMarker : ENTITY up_marker
    PORT MAP(
      clk          => clk,
      reset        => reset,
      start        => start_upmark,
      probe_in     => upmark_start_probe,
      node_in      => down_node_out,
      done_bit     => up_done_bit,
      ram_we       => up0_we,
      ram_addr     => up0_addr,
      ram_data_in  => up0_data_in,
      ram_data_out => up0_data_out,
      read_start   => up_read_start,
      read_done    => up_read_done,
      write_done   => up_write_done
      );

  FreeInfo : ENTITY free_info
    PORT MAP(
      clk                   => clk,
      reset                 => reset,
      start                 => start_free_info,
      address               => free_addr,
      size                  => size,
      probe_out             => free_info_probe_out,
      done_bit              => free_info_done_bit,
      top_node_size_out     => free_tns,
      log2top_node_size_out => free_log2tns,
      group_addr_out        => free_group_addr,
      vg_addr               => vg_addr_free
      );



  TrackerBud : ENTITY tracker
    PORT MAP(
      clk           => clk,
      reset         => reset,
      start         => start_tracker,
      group_addr_in => group_addr_to_tracker,
      size          => size,
      flag_alloc    => flag_alloc,
      func_sel      => tracker_func_sel,
      done_bit      => tracker_done,
      probe_out     => tracker_probe_out,
      ram_we        => tracker_we,
      ram_addr      => tracker_addr,
      ram_data_in   => tracker_data_in,
      ram_data_out  => tracker_data_out,
      init_start    => init_start_wire
      );

  CheckBlock : ENTITY check_blocking
    PORT MAP(
      clk               => clk,
      reset             => reset,
      start             => start_check_blocking,
      flag_blocking_out => flag_blocking,
      probe_in          => cblock_probe_in,
      probe_out         => cblock_probe_out,
      done_bit          => cblock_done,
      ram_addr          => cblock_ram_addr,
      ram_data_out      => cblock_ram_data_out,
      read_start        => cb_read_start,
      read_done         => cb_read_done
      );

  
  P0 : PROCESS(state, up_done_bit)      -- controls FSM, only writes nstate!

  BEGIN

    nstate <= idle;                     -- default value
    done   <= '0';

    CASE state IS
      WHEN idle => nstate <= idle;

      WHEN init0       => nstate <= init_enable;
      WHEN init_enable => nstate <= init1;
      WHEN init1       => nstate <= init1;
      WHEN init2       => nstate <= init_check;
      WHEN init_check  => nstate <= init0;

      WHEN malloc0  => nstate <= malloc0;
      WHEN malloc1  => nstate <= malloc1;
      WHEN free     => nstate <= free;
      WHEN search   => nstate <= search;
      WHEN track    => nstate <= track;
      WHEN downmark => nstate <= downmark;
      WHEN upmark   => nstate <= upmark;
                       IF up_done_bit = '1' THEN
                         nstate <= done_state;
                       END IF;
      WHEN done_state => nstate <= idle;
                         done <= '1';
      WHEN OTHERS => nstate <= idle;
    END CASE;

  END PROCESS;

  P1 : PROCESS

  BEGIN
    WAIT UNTIL clk'event AND clk = '1';

    state <= nstate;                    -- default

    -- start signals
    start_free_info      <= '0';
    start_tracker        <= '0';
    start_search         <= '0';
    start_check_blocking <= '0';
    start_dmark          <= '0';
    start_upmark         <= '0';
    init_we              <= '0';

    IF reset = '0' THEN                 -- active low
      state <= idle;
    ELSE

      -- ############# Initialisation #############
      

      IF state = init0 THEN
        init_addr <= std_logic_vector(to_unsigned(init_addr_int, init_addr'length));
      END IF;

      IF state = init_enable THEN
        init_we <= '1';                 -- default value
      END IF;

      IF state = init1 THEN
        IF init_done = '1' THEN
          state <= init2;
        END IF;
      END IF;

      IF state = init2 THEN
        init_addr_int <= init_addr_int + 1;
      END IF;

      IF state = init_check THEN
        IF init_addr_int = to_integer(unsigned(ParAddr)) THEN  --WORDS_MEM_TOTAL THEN
          state             <= idle;
          init_finished_bit <= '1'; -- should be a pulse
        END IF;
      END IF;

      -- ############# End Initialisation##########

      counter_latency <= counter_latency + 1;

      IF state = idle THEN
        
        counter_latency <= counter_latency;
		
		init_finished_bit <= '0'; -- should be a pulse, so placed here
        IF init_start_bit = '1' THEN
          state             <= init0;
          init_addr_int     <= 0;         
          
        ELSIF start = '1' THEN
          counter_search  <= 0;
          counter_down    <= 0;
          counter_up      <= 0;
          counter_malloc0 <= 0;
          counter_malloc1 <= 0;
          counter_track   <= 0;
          counter_latency <= 0;
          IF cmd = '0' THEN             -- cmd = 0 free
            state           <= free;
            start_free_info <= '1';
          ELSE                          -- cmd = 1 malloc
            state            <= malloc0;
            --start_tracker    <= '1';
            --tracker_func_sel <= '1';
          END IF;
        END IF;
        
      END IF;

       IF state = malloc0 THEN
        --IF tracker_done = '1' THEN
         -- IF to_integer(usgn(tracker_probe_out.verti)) = 0 THEN  -- skip cblock
            state              <= search;
            start_search       <= '1';
            --search_start_probe <= tracker_probe_out;
        search_start_probe.verti   <= (OTHERS => '0');
        search_start_probe.horiz   <= (OTHERS => '0');
        search_start_probe.saddr   <= (OTHERS => '0');
        search_start_probe.nodesel <= (OTHERS => '0');
        search_start_probe.rowbase <= (OTHERS => '0');
        search_start_probe.alvec   <= '0';
          --ELSE                                                   -- cblock 
          --  state                <= malloc1;
          --  start_check_blocking <= '1';
          --  cblock_probe_in      <= tracker_probe_out;
          --END IF;
        --END IF;
        
      END IF;

      IF state = malloc1 THEN
        IF cblock_done = '1' THEN
          state        <= search;
          start_search <= '1';
          IF flag_blocking = '0' THEN  -- no blocking, use the tracker_probe_out
            search_start_probe <= tracker_probe_out;
          ELSE
            search_start_probe <= cblock_probe_out;
          END IF;
        END IF;
      END IF;  -- end malloc1    

      IF state = free THEN

        IF free_info_done_bit = '1' THEN
        --  state                 <= track;
        --  start_tracker         <= '1';
        --  tracker_func_sel      <= '0';
       --   group_addr_to_tracker <= free_group_addr;  -- group addr  
       --   IF to_integer(usgn(size)) = 1 AND USE_ALVEC = '1' THEN
        --    group_addr_to_tracker <= vg_addr_free;
        --  END IF;
        
          state       <= downmark;
          start_dmark <= '1';
          dmark_start_probe <= free_info_probe_out;
  
        
        END IF;
      END IF;  -- end free       

      IF state = search THEN

        IF search_done_bit = '1' THEN
          IF flag_malloc_failed = '0' THEN
            --state                 <= track;
            --start_tracker         <= '1';
            --tracker_func_sel      <= '0';
            --group_addr_to_tracker <= search0_addr;  -- group addr   
            --IF to_integer(usgn(size)) = 1 AND USE_ALVEC = '1' THEN
            --  group_addr_to_tracker <= vg_addr_malloc;
            --END IF;
          state       <= downmark;
          start_dmark <= '1';
          dmark_start_probe <= search_done_probe;     
            
          ELSE                          -- if search for allocation failed
            state <= done_state;
          END IF;
        END IF;
      END IF;  -- end search     

    IF state = track THEN

        IF tracker_done = '1' THEN
          state       <= downmark;
          start_dmark <= '1';
          IF flag_alloc = '1' THEN      -- malloc
            dmark_start_probe <= search_done_probe;
          ELSE
            dmark_start_probe <= free_info_probe_out;
          END IF;
        END IF;
      END IF;  -- end track    

      IF state = downmark THEN
        IF dmark_done_bit = '1' THEN
          state <= done_state;
          IF flag_markup = '1' THEN
            state              <= upmark;
            start_upmark       <= '1';
            upmark_start_probe <= search_done_probe;  -- malloc
            IF flag_alloc = '0' THEN                  -- free
              upmark_start_probe <= free_info_probe_out;
            END IF;
          END IF;
        END IF;
      END IF;  -- end downmark 
      
    END IF;  -- end reset
    
  END PROCESS;

  p2 : PROCESS (state,
                search0_addr,
                down0_we, down0_addr, down0_data_in,
                up0_we, up0_addr, up0_data_in,
                control_data_out, control_write_done, control_read_done,
                down_read_start,
                cb_read_start,
                search_read_start,
                up_read_start,
                cblock_ram_addr,
                init_addr, init_we, init_data_in
                )                       -- select ram signals
  BEGIN

    -- default
    control_we         <= '0';
    control_addr       <= (OTHERS => '0');
    control_data_in    <= (OTHERS => '0');
    control_read_start <= '0';

    -- to avoid latches 
    cb_read_done        <= '0';
    search_read_done    <= '0';
    down_read_done      <= '0';
    up_read_done        <= '0';
    cblock_ram_data_out <= (OTHERS => '0');
    search0_data_out    <= (OTHERS => '0');
    down0_data_out      <= (OTHERS => '0');
    up0_data_out        <= (OTHERS => '0');

    down_write_done <= '0';
    up_write_done   <= '0';
    init_done       <= '0';

    IF state = downmark THEN
      control_we         <= down0_we;
      control_addr       <= down0_addr;
      control_data_in    <= down0_data_in;
      down0_data_out     <= control_data_out;
      down_write_done    <= control_write_done;
      down_read_done     <= control_read_done;
      control_read_start <= down_read_start;
    END IF;

    IF state = malloc1 THEN
      control_addr        <= cblock_ram_addr;
      cblock_ram_data_out <= control_data_out;
      cb_read_done        <= control_read_done;
      control_read_start  <= cb_read_start;
    END IF;

    IF state = search THEN
      control_addr       <= search0_addr;
      search0_data_out   <= control_data_out;
      search_read_done   <= control_read_done;
      control_read_start <= search_read_start;
    END IF;

    IF state = upmark THEN
      control_we         <= up0_we;
      control_addr       <= up0_addr;
      control_data_in    <= up0_data_in;
      up0_data_out       <= control_data_out;
      up_read_done       <= control_read_done;
      up_write_done      <= control_write_done;
      control_read_start <= up_read_start;
    END IF;

    IF state = init0 THEN
      control_we      <= init_we;
      control_addr    <= init_addr;
      control_data_in <= init_data_in;
      init_done       <= control_write_done;
    END IF;
    IF state = init_enable THEN
      control_we      <= init_we;
      control_addr    <= init_addr;
      control_data_in <= init_data_in;
      init_done       <= control_write_done;
    END IF;
    IF state = init1 THEN
      control_we      <= init_we;
      control_addr    <= init_addr;
      control_data_in <= init_data_in;
      init_done       <= control_write_done;
    END IF;
    IF state = init2 THEN
      control_we      <= init_we;
      control_addr    <= init_addr;
      control_data_in <= init_data_in;
      init_done       <= control_write_done;
    END IF;
    IF state = init_check THEN
      control_we      <= init_we;
      control_addr    <= init_addr;
      control_data_in <= init_data_in;
      init_done       <= control_write_done;
    END IF;
    
  END PROCESS;

  p3 : PROCESS  -- (control_we, control_addr, control_data_in)

  BEGIN
    WAIT UNTIL clk'event AND clk = '1';

    ddr_start          <= '0';
    control_read_done  <= '0';
    ram0_we            <= '0';
    control_write_done <= '0';
    
    IF reset = '0' OR state = idle THEN
      wait_flag       <= '0';
      write_wait_flag <= '0';
    ELSE

      IF to_integer(usgn(control_addr)) >= to_integer(usgn(ParAddr)) THEN  -- use DDR

        -- about done bit
        IF ddr_done = '1' THEN
          -- it's not halmful to set both to 1 now
          -- later, maybe seperate the read_one and write_done in AXI MASTER to avoid this
          control_write_done <= '1';
          control_read_done  <= '1';
          -- for read
          control_data_out   <= ddr_read_data;
          
        END IF;

        -- address
        ddr_addr <= slv(((usgn(control_addr) - usgn(ParAddr)) SLL 2) + usgn(DDR_TREE_BASE));  --------------- num * 4 + base

        IF control_read_start = '1' THEN  -- start, command
          ddr_start   <= '1';
          ddr_command <= '1';             -- read

        ELSIF control_we = '1' THEN     -- start, command, write_data
          ddr_start      <= '1';
          ddr_command    <= '0';        -- write
          ddr_write_data <= control_data_in;
        END IF;
        
      ELSE                              -- use BRAM

        IF control_read_start = '1' THEN
          
          IF wait_flag = '0' THEN
            wait_flag <= '1';
          END IF;
          
        ELSIF control_we = '1' THEN
          
          IF write_wait_flag = '0' THEN
            write_wait_flag <= '1';
          END IF;

          ram0_we      <= '1';
          ram0_data_in <= control_data_in;

        END IF;
        
      END IF;

      IF wait_flag = '1' THEN
        wait_flag         <= '0';
        control_data_out  <= ram0_data_out;
        control_read_done <= '1';
      END IF;

      IF write_wait_flag = '1' THEN
        write_wait_flag    <= '0';
        control_write_done <= '1';  -- DANGEROUS? IS DATA BEING VALID FOR ENOUGH TIME?      
      END IF;
      
      
    END IF;
  END PROCESS;

  malloc_addr   <= search_done_probe.saddr;
  flag_alloc    <= cmd;
  malloc_failed <= flag_malloc_failed;

  tracker_we_wire <= (0 => tracker_we, OTHERS => '0');
  ram0_we_wire    <= (0 => ram0_we, OTHERS => '0');

  ram0_addr <= control_addr;

  counter_out(7 DOWNTO 0)   <= slv(to_unsigned(counter_search, 8));
  counter_out(15 DOWNTO 8)  <= slv(to_unsigned(counter_down, 8));
  counter_out(23 DOWNTO 16) <= slv(to_unsigned(counter_up, 8));
  counter_out(31 DOWNTO 24) <= slv(to_unsigned(counter_malloc0, 8));

  counter_out2(7 DOWNTO 0)   <= slv(to_unsigned(counter_malloc1, 8));
  counter_out2(15 DOWNTO 8)  <= slv(to_unsigned(counter_track, 8));
  counter_out2(31 DOWNTO 16) <= slv(to_unsigned(counter_latency, 16));

  init_data_in <= (OTHERS => '0');

  init_start_wire <= init_start_bit;
  
END ARCHITECTURE synth;
