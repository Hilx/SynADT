LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
USE work.ALL;

ENTITY SysAlloc_v1_0 IS
  GENERIC (
    -- Users to add parameters here

    -- User parameters ends
    -- Do not modify the parameters beyond this line


    -- Parameters of Axi Slave Bus Interface S00_AXI
    C_S00_AXI_DATA_WIDTH : integer := 32;
    C_S00_AXI_ADDR_WIDTH : integer := 5;

    -- Parameters of Axi Master Bus Interface M00_AXI
    C_M00_AXI_START_DATA_VALUE       : std_logic_vector := x"AA000000";
    C_M00_AXI_TARGET_SLAVE_BASE_ADDR : std_logic_vector := x"40000000";
    C_M00_AXI_ADDR_WIDTH             : integer          := 32;
    C_M00_AXI_DATA_WIDTH             : integer          := 32;
    C_M00_AXI_TRANSACTIONS_NUM       : integer          := 4
    );
  PORT (
    -- Users to add ports here

    -- User ports ends
    -- Do not modify the ports beyond this line


    -- Ports of Axi Slave Bus Interface S00_AXI
    s00_axi_aclk    : IN  std_logic;
    s00_axi_aresetn : IN  std_logic;
    s00_axi_awaddr  : IN  std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 DOWNTO 0);
    s00_axi_awprot  : IN  std_logic_vector(2 DOWNTO 0);
    s00_axi_awvalid : IN  std_logic;
    s00_axi_awready : OUT std_logic;
    s00_axi_wdata   : IN  std_logic_vector(C_S00_AXI_DATA_WIDTH-1 DOWNTO 0);
    s00_axi_wstrb   : IN  std_logic_vector((C_S00_AXI_DATA_WIDTH/8)-1 DOWNTO 0);
    s00_axi_wvalid  : IN  std_logic;
    s00_axi_wready  : OUT std_logic;
    s00_axi_bresp   : OUT std_logic_vector(1 DOWNTO 0);
    s00_axi_bvalid  : OUT std_logic;
    s00_axi_bready  : IN  std_logic;
    s00_axi_araddr  : IN  std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 DOWNTO 0);
    s00_axi_arprot  : IN  std_logic_vector(2 DOWNTO 0);
    s00_axi_arvalid : IN  std_logic;
    s00_axi_arready : OUT std_logic;
    s00_axi_rdata   : OUT std_logic_vector(C_S00_AXI_DATA_WIDTH-1 DOWNTO 0);
    s00_axi_rresp   : OUT std_logic_vector(1 DOWNTO 0);
    s00_axi_rvalid  : OUT std_logic;
    s00_axi_rready  : IN  std_logic;

    -- Ports of Axi Master Bus Interface M00_AXI
    --  m00_axi_init_axi_txn    : in std_logic;
    m00_axi_error    : OUT std_logic;
    m00_axi_txn_done : OUT std_logic;
    m00_axi_aclk     : IN  std_logic;
    m00_axi_aresetn  : IN  std_logic;
    m00_axi_awaddr   : OUT std_logic_vector(C_M00_AXI_ADDR_WIDTH-1 DOWNTO 0);
    m00_axi_awprot   : OUT std_logic_vector(2 DOWNTO 0);
    m00_axi_awvalid  : OUT std_logic;
    m00_axi_awready  : IN  std_logic;
    m00_axi_wdata    : OUT std_logic_vector(C_M00_AXI_DATA_WIDTH-1 DOWNTO 0);
    m00_axi_wstrb    : OUT std_logic_vector(C_M00_AXI_DATA_WIDTH/8-1 DOWNTO 0);
    m00_axi_wvalid   : OUT std_logic;
    m00_axi_wready   : IN  std_logic;
    m00_axi_bresp    : IN  std_logic_vector(1 DOWNTO 0);
    m00_axi_bvalid   : IN  std_logic;
    m00_axi_bready   : OUT std_logic;
    m00_axi_araddr   : OUT std_logic_vector(C_M00_AXI_ADDR_WIDTH-1 DOWNTO 0);
    m00_axi_arprot   : OUT std_logic_vector(2 DOWNTO 0);
    m00_axi_arvalid  : OUT std_logic;
    m00_axi_arready  : IN  std_logic;
    m00_axi_rdata    : IN  std_logic_vector(C_M00_AXI_DATA_WIDTH-1 DOWNTO 0);
    m00_axi_rresp    : IN  std_logic_vector(1 DOWNTO 0);
    m00_axi_rvalid   : IN  std_logic;
    m00_axi_rready   : OUT std_logic
    );
END SysAlloc_v1_0;

ARCHITECTURE arch_imp OF SysAlloc_v1_0 IS

  -- component declaration
  COMPONENT SysAlloc_v1_0_S00_AXI IS
    GENERIC (
      C_S_AXI_DATA_WIDTH : integer := 32;
      C_S_AXI_ADDR_WIDTH : integer := 5
      );
    PORT (
      command   : OUT std_logic;
      request   : OUT std_logic_vector(31 DOWNTO 0);
      req_valid : OUT std_logic;
      result    : IN  std_logic_vector(31 DOWNTO 0);
      res_valid : IN  std_logic;
      done_free : IN  std_logic;

      write_counter, write_counter2 : IN std_logic_vector(31 DOWNTO 0);
      counter_interc                : IN integer;

      init_start_bit    : OUT std_logic;
      init_finished_bit : IN  std_logic;
      init_range : out integer;
	  init_done_reset : out std_logic;

      S_AXI_ACLK    : IN  std_logic;
      S_AXI_ARESETN : IN  std_logic;
      S_AXI_AWADDR  : IN  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 DOWNTO 0);
      S_AXI_AWPROT  : IN  std_logic_vector(2 DOWNTO 0);
      S_AXI_AWVALID : IN  std_logic;
      S_AXI_AWREADY : OUT std_logic;
      S_AXI_WDATA   : IN  std_logic_vector(C_S_AXI_DATA_WIDTH-1 DOWNTO 0);
      S_AXI_WSTRB   : IN  std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 DOWNTO 0);
      S_AXI_WVALID  : IN  std_logic;
      S_AXI_WREADY  : OUT std_logic;
      S_AXI_BRESP   : OUT std_logic_vector(1 DOWNTO 0);
      S_AXI_BVALID  : OUT std_logic;
      S_AXI_BREADY  : IN  std_logic;
      S_AXI_ARADDR  : IN  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 DOWNTO 0);
      S_AXI_ARPROT  : IN  std_logic_vector(2 DOWNTO 0);
      S_AXI_ARVALID : IN  std_logic;
      S_AXI_ARREADY : OUT std_logic;
      S_AXI_RDATA   : OUT std_logic_vector(C_S_AXI_DATA_WIDTH-1 DOWNTO 0);
      S_AXI_RRESP   : OUT std_logic_vector(1 DOWNTO 0);
      S_AXI_RVALID  : OUT std_logic;
      S_AXI_RREADY  : IN  std_logic
      );
  END COMPONENT SysAlloc_v1_0_S00_AXI;

  COMPONENT SysAlloc_v1_0_M00_AXI IS
    GENERIC (
      C_M_START_DATA_VALUE       : std_logic_vector := x"AA000000";
      C_M_TARGET_SLAVE_BASE_ADDR : std_logic_vector := x"40000000";
      C_M_AXI_ADDR_WIDTH         : integer          := 32;
      C_M_AXI_DATA_WIDTH         : integer          := 32;
      C_M_TRANSACTIONS_NUM       : integer          := 4
      );
    PORT (

      command       : IN  std_logic;    -- 0 = write, 1 = read
      start         : IN  std_logic;
      ddr_addr      : IN  std_logic_vector(31 DOWNTO 0);
      write_data    : IN  std_logic_vector(31 DOWNTO 0);
      read_data     : OUT std_logic_vector(31 DOWNTO 0);
      done_bit      : OUT std_logic;
	  
	  init_ddr_start : in std_logic;
	  init_ddr_finished : out std_logic;
	  init_ddr_range : in integer;
	  init_done_reset : in std_logic;
	  
      --        INIT_AXI_TXN    : in std_logic;
      error         : OUT std_logic;
      TXN_DONE      : OUT std_logic;
      M_AXI_ACLK    : IN  std_logic;
      M_AXI_ARESETN : IN  std_logic;
      M_AXI_AWADDR  : OUT std_logic_vector(C_M_AXI_ADDR_WIDTH-1 DOWNTO 0);
      M_AXI_AWPROT  : OUT std_logic_vector(2 DOWNTO 0);
      M_AXI_AWVALID : OUT std_logic;
      M_AXI_AWREADY : IN  std_logic;
      M_AXI_WDATA   : OUT std_logic_vector(C_M_AXI_DATA_WIDTH-1 DOWNTO 0);
      M_AXI_WSTRB   : OUT std_logic_vector(C_M_AXI_DATA_WIDTH/8-1 DOWNTO 0);
      M_AXI_WVALID  : OUT std_logic;
      M_AXI_WREADY  : IN  std_logic;
      M_AXI_BRESP   : IN  std_logic_vector(1 DOWNTO 0);
      M_AXI_BVALID  : IN  std_logic;
      M_AXI_BREADY  : OUT std_logic;
      M_AXI_ARADDR  : OUT std_logic_vector(C_M_AXI_ADDR_WIDTH-1 DOWNTO 0);
      M_AXI_ARPROT  : OUT std_logic_vector(2 DOWNTO 0);
      M_AXI_ARVALID : OUT std_logic;
      M_AXI_ARREADY : IN  std_logic;
      M_AXI_RDATA   : IN  std_logic_vector(C_M_AXI_DATA_WIDTH-1 DOWNTO 0);
      M_AXI_RRESP   : IN  std_logic_vector(1 DOWNTO 0);
      M_AXI_RVALID  : IN  std_logic;
      M_AXI_RREADY  : OUT std_logic
      );
  END COMPONENT SysAlloc_v1_0_M00_AXI;

  COMPONENT intercept IS
    PORT (
      clk                 : IN  std_logic;
      reset               : IN  std_logic;
      ddr_sel             : OUT std_logic;
      -- AXI slave
      command             : IN  std_logic;
      request             : IN  std_logic_vector(31 DOWNTO 0);
      req_valid           : IN  std_logic;
      result              : OUT std_logic_vector(31 DOWNTO 0);
      res_valid           : OUT std_logic;
      done_free           : OUT std_logic;
      -- Buddy  
      buddy_start         : OUT std_logic;
      buddy_command       : OUT std_logic;  -- 0 = alloc, 1 = free; Differ from  marking process 
      buddy_size          : OUT std_logic_vector(31 DOWNTO 0);
      buddy_free_addr     : OUT std_logic_vector(31 DOWNTO 0);
      buddy_done          : IN  std_logic;
      buddy_malloc_addr   : IN  std_logic_vector(31 DOWNTO 0);
      buddy_malloc_failed : IN  std_logic;

      write_counter_out : OUT integer;

      -- ddr
      ddr_command    : OUT std_logic;   -- 0 = write, 1 = read
      ddr_start      : OUT std_logic;
      ddr_addr       : OUT std_logic_vector(31 DOWNTO 0);
      ddr_write_data : OUT std_logic_vector(31 DOWNTO 0);
      ddr_read_data  : IN  std_logic_vector(31 DOWNTO 0);
      ddr_done       : IN  std_logic
      );
  END COMPONENT intercept;

  COMPONENT rbuddy_top IS
    PORT(
      -- init
      init_start_bit    : IN  std_logic;
      init_finished_bit : OUT std_logic;

      clk           : IN  std_logic;
      reset         : IN  std_logic;
      -- Request Processor
      start         : IN  std_logic;
      cmd           : IN  std_logic;  -- 0 = alloc, 1 = free; Differ from  marking process 
      size          : IN  std_logic_vector(31 DOWNTO 0);
      free_addr     : IN  std_logic_vector(31 DOWNTO 0);
      done          : OUT std_logic;
      malloc_addr   : OUT std_logic_vector(31 DOWNTO 0);
      malloc_failed : OUT std_logic;

      counter_out, counter_out2 : OUT std_logic_vector(31 DOWNTO 0);

      -- DDR
      ddr_command    : OUT std_logic;   -- 0 = write, 1 = read
      ddr_start      : OUT std_logic;
      ddr_addr       : OUT std_logic_vector(31 DOWNTO 0);
      ddr_write_data : OUT std_logic_vector(31 DOWNTO 0);
      ddr_read_data  : IN  std_logic_vector(31 DOWNTO 0);
      ddr_done       : IN  std_logic
      );
  END COMPONENT rbuddy_top;

  --  AXI SLAVE to Request Processor
  SIGNAL rproc_command   : std_logic;
  SIGNAL rproc_request   : std_logic_vector(31 DOWNTO 0);
  SIGNAL rproc_req_valid : std_logic;
  SIGNAL rproc_result    : std_logic_vector(31 DOWNTO 0);
  SIGNAL rproc_res_valid : std_logic;
  SIGNAL rproc_done_free : std_logic;

  -- Request Processor to Buddy SysAlloc
  SIGNAL buddy_start         : std_logic;
  SIGNAL buddy_command       : std_logic;
  SIGNAL buddy_size          : std_logic_vector(31 DOWNTO 0);
  SIGNAL buddy_free_addr     : std_logic_vector(31 DOWNTO 0);
  SIGNAL buddy_done          : std_logic;
  SIGNAL buddy_malloc_addr   : std_logic_vector(31 DOWNTO 0);
  SIGNAL buddy_malloc_failed : std_logic;

  -- to AXI master (for DDR)
  SIGNAL ddr_command    : std_logic;    -- 0 = write, 1 = read
  SIGNAL ddr_start      : std_logic;
  SIGNAL ddr_addr       : std_logic_vector(31 DOWNTO 0);
  SIGNAL ddr_write_data : std_logic_vector(31 DOWNTO 0);
  SIGNAL ddr_read_data  : std_logic_vector(31 DOWNTO 0);
  SIGNAL ddr_done       : std_logic;

  -- BUDDY to DDR
  SIGNAL buddy_ddr_command    : std_logic;  -- 0 = write, 1 = read
  SIGNAL buddy_ddr_start      : std_logic;
  SIGNAL buddy_ddr_addr       : std_logic_vector(31 DOWNTO 0);
  SIGNAL buddy_ddr_write_data : std_logic_vector(31 DOWNTO 0);
  SIGNAL buddy_ddr_read_data  : std_logic_vector(31 DOWNTO 0);
  SIGNAL buddy_ddr_done       : std_logic;

  -- INTERCEPT to DDR
  SIGNAL interc_ddr_command    : std_logic;  -- 0 = write, 1 = read
  SIGNAL interc_ddr_start      : std_logic;
  SIGNAL interc_ddr_addr       : std_logic_vector(31 DOWNTO 0);
  SIGNAL interc_ddr_write_data : std_logic_vector(31 DOWNTO 0);
  SIGNAL interc_ddr_read_data  : std_logic_vector(31 DOWNTO 0);
  SIGNAL interc_ddr_done       : std_logic;

  SIGNAL ddr_sel : std_logic;

  SIGNAL write_counter : integer;

  SIGNAL buddy_counter_out, buddy_counter_out2 : std_logic_vector(31 DOWNTO 0);

  SIGNAL init_start_bit, init_all_done, init_bram_done : std_logic;
  signal init_range : integer;
  signal init_done_reset : std_logic;
  
BEGIN

-- Instantiation of Axi Bus Interface S00_AXI
  SysAlloc_v1_0_S00_AXI_inst : SysAlloc_v1_0_S00_AXI
    GENERIC MAP (
      C_S_AXI_DATA_WIDTH => C_S00_AXI_DATA_WIDTH,
      C_S_AXI_ADDR_WIDTH => C_S00_AXI_ADDR_WIDTH
      )
    PORT MAP (
      command   => rproc_command,
      request   => rproc_request,
      req_valid => rproc_req_valid,
      result    => rproc_result,
      res_valid => rproc_res_valid,
      done_free => rproc_done_free,

      write_counter  => buddy_counter_out,
      write_counter2 => buddy_counter_out2,
      counter_interc => write_counter,

      init_start_bit    => init_start_bit,
      init_finished_bit => init_all_done,
      init_range => init_range,
	  init_done_reset => init_done_reset,

      S_AXI_ACLK    => s00_axi_aclk,
      S_AXI_ARESETN => s00_axi_aresetn,
      S_AXI_AWADDR  => s00_axi_awaddr,
      S_AXI_AWPROT  => s00_axi_awprot,
      S_AXI_AWVALID => s00_axi_awvalid,
      S_AXI_AWREADY => s00_axi_awready,
      S_AXI_WDATA   => s00_axi_wdata,
      S_AXI_WSTRB   => s00_axi_wstrb,
      S_AXI_WVALID  => s00_axi_wvalid,
      S_AXI_WREADY  => s00_axi_wready,
      S_AXI_BRESP   => s00_axi_bresp,
      S_AXI_BVALID  => s00_axi_bvalid,
      S_AXI_BREADY  => s00_axi_bready,
      S_AXI_ARADDR  => s00_axi_araddr,
      S_AXI_ARPROT  => s00_axi_arprot,
      S_AXI_ARVALID => s00_axi_arvalid,
      S_AXI_ARREADY => s00_axi_arready,
      S_AXI_RDATA   => s00_axi_rdata,
      S_AXI_RRESP   => s00_axi_rresp,
      S_AXI_RVALID  => s00_axi_rvalid,
      S_AXI_RREADY  => s00_axi_rready
      );

-- Instantiation of Axi Bus Interface M00_AXI
  SysAlloc_v1_0_M00_AXI_inst : SysAlloc_v1_0_M00_AXI
    GENERIC MAP (
      C_M_START_DATA_VALUE       => C_M00_AXI_START_DATA_VALUE,
      C_M_TARGET_SLAVE_BASE_ADDR => C_M00_AXI_TARGET_SLAVE_BASE_ADDR,
      C_M_AXI_ADDR_WIDTH         => C_M00_AXI_ADDR_WIDTH,
      C_M_AXI_DATA_WIDTH         => C_M00_AXI_DATA_WIDTH,
      C_M_TRANSACTIONS_NUM       => C_M00_AXI_TRANSACTIONS_NUM
      )
    PORT MAP (
      command    => ddr_command,
      start      => ddr_start,
      ddr_addr   => ddr_addr,
      write_data => ddr_write_data,
      read_data  => ddr_read_data,
      done_bit   => ddr_done,
	  
	  init_ddr_start => init_bram_done,
	  init_ddr_finished => init_all_done,
	  init_ddr_range => init_range,
	   init_done_reset => init_done_reset,

      --        INIT_AXI_TXN    => m00_axi_init_axi_txn,
      error         => m00_axi_error,
      TXN_DONE      => m00_axi_txn_done,
      M_AXI_ACLK    => m00_axi_aclk,
      M_AXI_ARESETN => m00_axi_aresetn,
      M_AXI_AWADDR  => m00_axi_awaddr,
      M_AXI_AWPROT  => m00_axi_awprot,
      M_AXI_AWVALID => m00_axi_awvalid,
      M_AXI_AWREADY => m00_axi_awready,
      M_AXI_WDATA   => m00_axi_wdata,
      M_AXI_WSTRB   => m00_axi_wstrb,
      M_AXI_WVALID  => m00_axi_wvalid,
      M_AXI_WREADY  => m00_axi_wready,
      M_AXI_BRESP   => m00_axi_bresp,
      M_AXI_BVALID  => m00_axi_bvalid,
      M_AXI_BREADY  => m00_axi_bready,
      M_AXI_ARADDR  => m00_axi_araddr,
      M_AXI_ARPROT  => m00_axi_arprot,
      M_AXI_ARVALID => m00_axi_arvalid,
      M_AXI_ARREADY => m00_axi_arready,
      M_AXI_RDATA   => m00_axi_rdata,
      M_AXI_RRESP   => m00_axi_rresp,
      M_AXI_RVALID  => m00_axi_rvalid,
      M_AXI_RREADY  => m00_axi_rready
      );

  INTERC : intercept
    PORT MAP(

      write_counter_out => write_counter,

      clk                 => s00_axi_aclk,
      reset               => s00_axi_aresetn,
      ddr_sel             => ddr_sel,
      -- axi slave
      command             => rproc_command,
      request             => rproc_request,
      req_valid           => rproc_req_valid,
      result              => rproc_result,
      res_valid           => rproc_res_valid,
      done_free           => rproc_done_free,
      -- buddy
      buddy_start         => buddy_start,
      buddy_command       => buddy_command,
      buddy_done          => buddy_done,
      buddy_size          => buddy_size,
      buddy_free_addr     => buddy_free_addr,
      buddy_malloc_addr   => buddy_malloc_addr,
      buddy_malloc_failed => buddy_malloc_failed,
      -- ddr
      ddr_command         => interc_ddr_command,
      ddr_start           => interc_ddr_start,
      ddr_addr            => interc_ddr_addr,
      ddr_write_data      => interc_ddr_write_data,
      ddr_read_data       => interc_ddr_read_data,
      ddr_done            => interc_ddr_done
      );        

  buddy : rbuddy_top
    PORT MAP (
      counter_out  => buddy_counter_out,
      counter_out2 => buddy_counter_out2,

      init_start_bit    => init_start_bit,
      init_finished_bit => init_bram_done,

      clk            => s00_axi_aclk,
      reset          => s00_axi_aresetn,
      -- request processor
      start          => buddy_start,
      cmd            => buddy_command,
      size           => buddy_size,
      free_addr      => buddy_free_addr,
      done           => buddy_done,
      malloc_addr    => buddy_malloc_addr,
      malloc_failed  => buddy_malloc_failed,
      -- for using DDR
      ddr_command    => buddy_ddr_command,
      ddr_start      => buddy_ddr_start,
      ddr_addr       => buddy_ddr_addr,
      ddr_write_data => buddy_ddr_write_data,
      ddr_read_data  => buddy_ddr_read_data,
      ddr_done       => buddy_ddr_done
      );        

  -- DDR

  -- Add user logic here
  PROCESS(ddr_sel,
          interc_ddr_command, interc_ddr_start, interc_ddr_addr, interc_ddr_write_data,
          buddy_ddr_command, buddy_ddr_start, buddy_ddr_addr, buddy_ddr_write_data,
          ddr_read_data, ddr_done
          )
  BEGIN
    ddr_command          <= interc_ddr_command;
    ddr_start            <= interc_ddr_start;
    ddr_addr             <= interc_ddr_addr;
    ddr_write_data       <= interc_ddr_write_data;
    interc_ddr_read_data <= ddr_read_data;
    interc_ddr_done      <= ddr_done;

    -- to avoid latches
    buddy_ddr_read_data <= (OTHERS => '0');
    buddy_ddr_done      <= '0';

    IF ddr_sel = '1' THEN
      ddr_command         <= buddy_ddr_command;
      ddr_start           <= buddy_ddr_start;
      ddr_addr            <= buddy_ddr_addr;
      ddr_write_data      <= buddy_ddr_write_data;
      buddy_ddr_read_data <= ddr_read_data;
      buddy_ddr_done      <= ddr_done;
    END IF;
  END PROCESS;

  -- User logic ends

END arch_imp;
