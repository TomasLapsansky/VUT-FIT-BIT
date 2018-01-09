-- xlapsa00
-- VUT FIT INP
-- 2017/2018

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( 
	RESET, SMCLK : in std_logic;
	ROW, LED : out std_logic_vector(0 to 7)
);
end ledc8x8;

architecture main of ledc8x8 is

	signal ctr_row: std_logic_vector(7 downto 0);
	signal ctr_led: std_logic_vector(7 downto 0);
	signal citac: std_logic;
	signal ctr_citac: std_logic_vector(7 downto 0);

begin
	-- Citac
	citacka: process(RESET, SMCLK)
	begin
		
		if RESET = '1' then
			ctr_citac <= "00000000";
		elsif SMCLK'event and SMCLK = '1' then
			ctr_citac <= ctr_citac + 1;
		end if;

		if(ctr_citac = X"FF") then
			citac <= '1';
		else
			citac <= '0';
		end if;

	end process citacka;

	-- Rotacia
	rotation: process(SMCLK, RESET)
	begin
		
		if RESET = '1' then
			ctr_row <= "10000000";
		elsif SMCLK'event and SMCLK = '1' and citac = '1' then
			ctr_row <= ctr_row(0) & ctr_row(7 downto 1);
		end if;
		
	end process rotation;
	
	-- Dekoder
	decoder: process(ctr_row)
	begin
		
		case(ctr_row) is
		when "10000000" =>
			ctr_led <= "00000111";
		when "01000000" =>
			ctr_led <= "11011111";
		when "00100000" =>
			ctr_led <= "11011111";
		when "00010000" =>
			ctr_led <= "11010111";
		when "00001000" =>
			ctr_led <= "11010111";
		when "00000100" =>
			ctr_led <= "11110111";
		when "00000010" =>
			ctr_led <= "11110111";
		when "00000001" =>
			ctr_led <= "11110000";
		when others =>
			ctr_led <= "11111111";
		end case;
		
	end process decoder;

	ROW <= ctr_row;
	LED <= ctr_led;
	
end main;
