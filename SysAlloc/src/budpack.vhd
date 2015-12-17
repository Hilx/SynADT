LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;

PACKAGE budpack IS
  
  CONSTANT BLOCK_SIZE     : std_logic_vector(31 DOWNTO 0) := std_logic_vector(to_unsigned(4, 32));  -- 16 Bytes
  CONSTANT LOG2BLOCK_SIZE : integer RANGE 0 TO 31         := 2;

  CONSTANT DDR_BASE : std_logic_vector(31 DOWNTO 0) := x"10000000";  -- starting address of the memory being managed

  CONSTANT DDR_TREE_BASE : std_logic_vector := x"18000000";  -- starting address of the memory space to store the allocation tree
  CONSTANT ParAddr       : std_logic_vector := x"00008000";  -- partition, put the number of groups you want to cache in BRAM

  -- total number of memory blocks managed by the allocator
  CONSTANT TOTAL_MEM_BLOCKS : std_logic_vector(31 DOWNTO 0) := std_logic_vector(to_unsigned(16777216, 32));  -- calculated number of minimum size blocks 
  -- log2(total number of minumum sizememory blocks)
  CONSTANT LOG2TMB          : std_logic_vector(6 DOWNTO 0)  := std_logic_vector(to_unsigned(24, 7));  -- MAX TREE DEPTH
  CONSTANT MAX_TREE_DEPTH   : integer                       := 24;
  -- if the allocation vector is used 
  CONSTANT USE_ALVEC        : std_logic                     := '0';
  CONSTANT ALVEC_SHIFT      : std_logic_vector(31 DOWNTO 0) := std_logic_vector(to_unsigned(0, 32));  -- the starting address of it -> depends the size of tree in DDR

  CONSTANT WORDS_MEM_TOTAL : integer := 2396745;  -- number of words required to store tree structure

  TYPE tree_probe IS RECORD  -- possible type for interface from DB to RCD. Change as required
    verti   : std_logic_vector(31 DOWNTO 0);
    horiz   : std_logic_vector(31 DOWNTO 0);
    nodesel : std_logic_vector(2 DOWNTO 0);  -- 8 nodes to select from
    saddr   : std_logic_vector(31 DOWNTO 0);
    rowbase : std_logic_vector(31 DOWNTO 0);
    alvec   : std_logic;
  END RECORD;

  TYPE holder_type IS RECORD
    mtree   : std_logic_vector(31 DOWNTO 0);
    nodesel : std_logic_vector(2 DOWNTO 0);
    gaddr   : std_logic_vector(31 DOWNTO 0);
  END RECORD;

END PACKAGE budpack;

