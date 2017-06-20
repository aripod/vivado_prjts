library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity I2S_Transmitter_block is
    Port ( clk : in  STD_LOGIC;
           ws : in  STD_LOGIC;
           data : in  STD_LOGIC_VECTOR(31 downto 0);	-- Input data (16 bits left, 16 bits right).
           sd : out  STD_LOGIC);
   end I2S_Transmitter_block;
   
   
architecture Behavioral of I2S_Transmitter_block is
    signal wsd, wsp : STD_LOGIC :='0';
    signal input_data : STD_LOGIC_VECTOR (15 downto 0) := (others=>'0');
    signal delay : STD_LOGIC_VECTOR (1 downto 0);
    
begin
    input_data <= data(31 downto 16) when wsd='1' else data(15 downto 0);
    sync_edge_detection: process(clk)
        begin
        if(falling_edge(clk)) then
            delay(0) <= delay(1);
            delay(1) <= ws;
        end if;
    end process;
    wsd <= delay(0);
    wsp <= delay(1) xor delay(0);
    
    Inst_Shift_Register_ParallelLoad: entity work.Shift_Register_ParallelLoad PORT MAP(
        clk => clk,
        pl => wsp,
        data => input_data,
        sd => sd
        );
        
end Behavioral;