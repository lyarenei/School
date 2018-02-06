-- ledc8x8.vhd
-- INP project 1
-- Author: Dominik Krivohlavek
-- login: xkrivo02

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port (
		SMCLK: in std_logic;
		RESET: in std_logic;

		ROW: out std_logic_vector(0 to 7);
		LED: out std_logic_vector(0 to 7)
);
end ledc8x8;

architecture main of ledc8x8 is

	-- CLK, active every 256th SMCLK period
	signal CLK: std_logic;
	
	-- ROW cloning for LED decoder (because ROW is not input => cannot be read)
	signal ROW_NUMBER: std_logic_vector(0 to 7);
begin    
	-- Counter for dividing SMCLK
	process (SMCLK, RESET)
		variable count: std_logic_vector(7 downto 0);
		begin
			if(RESET = '1') then
				count := "00000000";
			elsif(SMCLK='1' and SMCLK'event) then
				if count = "11111111" then
					count := "00000000";
					CLK <= '1';
				else
					count := count + 1;
					CLK <= '0';
				end if;
			end if;
	end process;

	-- Rotating register for switching active rows
	process(CLK, RESET)
		variable row_select	: bit_vector(7 downto 0) := "10000000";
		begin
			if(RESET = '1') then
				row_select := "10000000";
			elsif(CLK'event and CLK = '1') then
				row_select := row_select ror 1;
				ROW <= to_stdlogicvector(row_select);
				ROW_NUMBER <= to_stdlogicvector(row_select);
			end if;
	end process;

	-- Decoder for switching LEDs on/off on particular row
	process(ROW_NUMBER, RESET)
		begin
			if(RESET = '1') then
				LED <= "11111111";
			else
				case ROW_NUMBER is
					when "10000000" => LED <= "00011111";	-- xxx
					when "01000000" => LED <= "01101111";	-- x  x
					when "00100000" => LED <= "01101111";	-- x  x
					when "00010000" => LED <= "01100110";	-- x  xx  x
					when "00001000" => LED <= "00010101";	-- xxx x x
					when "00000100" => LED <= "11110011";	--     xx
					when "00000010" => LED <= "11110101";	--     x x
					when "00000001" => LED <= "11110110";	--     x  x
					when others => LED <= "11111111";
				end case;
			end if;
	end process;
end main;
