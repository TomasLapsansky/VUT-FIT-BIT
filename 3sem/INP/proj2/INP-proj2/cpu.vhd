-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2017 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Tomas Lapsansky (xlapsa00)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='0') / zapis do pameti (DATA_RDWR='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	signal ptr_addr: std_logic_vector(9 downto 0);
	signal ptr_inc: std_logic;
	signal ptr_dec: std_logic;

	signal pc_addr: std_logic_vector(11 downto 0);
	signal pc_inc: std_logic;
	signal pc_dec: std_logic;

	signal cnt_addr: std_logic_vector(7 downto 0);
	signal cnt_inc: std_logic;
	signal cnt_dec: std_logic;

	signal sel: std_logic_vector(1 downto 0);

	type instructions is (
		ptr_inc_inst, ptr_dec_inst,
		var_inc, var_dec,
		while_start, while_end, while_break,
		putchar, getchar,
		halt, undefined
	);

	signal instruction: instructions;

	type fsm_state is (
		fetch, p_decode,
		p_ptr_inc, p_ptr_dec,
		p_var_inc, p_var_inc_2, p_var_dec, p_var_dec_2,
		p_putchar, p_putchar_2, p_getchar, 
		pnull, p_undefined,
		p_while_start, p_while_start_2, p_while_start_3, p_while_start_4,
		p_while_end, p_while_end_2, p_while_end_3, p_while_end_4, p_while_end_5, p_while_end_6,
		p_while_break

	);
	signal pstate: fsm_state;
	signal nstate: fsm_state;

begin

	decoder: process(CODE_DATA)
	begin

		case(CODE_DATA) is
			when X"3E" => instruction <= ptr_inc_inst;
			when X"3C" => instruction <= ptr_dec_inst;
			when X"2B" => instruction <= var_inc;
			when X"2D" => instruction <= var_dec;
			when X"5B" => instruction <= while_start;
			when X"5D" => instruction <= while_end;
			when X"2E" => instruction <= putchar;
			when X"2C" => instruction <= getchar;
			when X"7E" => instruction <= while_break;
			when X"00" => instruction <= halt;
			when others => instruction <= undefined;
		end case;

	end process;

	mx: process(IN_DATA, DATA_RDATA, sel)
	begin

		case(sel) is
			when "00" => DATA_WDATA <= IN_DATA;
			when "01" => DATA_WDATA <= DATA_RDATA + 1;
			when "10" => DATA_WDATA <= DATA_RDATA - 1;
			when "11" => DATA_WDATA <= X"00";
			when others =>
		end case;

	end process;

	reg_pc: process(CLK, RESET, pc_addr, pc_inc, pc_dec)
	begin
		
		if(RESET = '1') then
			pc_addr <= "000000000000";
		elsif(CLK'event and CLK = '1') then
			if(pc_inc = '1') then
				pc_addr <= pc_addr + 1;
			elsif(pc_dec = '1') then
				pc_addr <= pc_addr - 1;
			end if;
		end if;

		CODE_ADDR <= pc_addr;

	end process;

	--CODE_ADDR <= pc_addr;

	reg_ptr: process(CLK, RESET, ptr_addr, ptr_inc, ptr_dec)
	begin

		if(RESET = '1') then
			ptr_addr <= "0000000000";
		elsif(CLK'event and CLK = '1') then
			if(ptr_inc = '1') then
				ptr_addr <= ptr_addr + 1;
			elsif(ptr_dec = '1') then
				ptr_addr <= ptr_addr - 1;
			end if;
		end if;

		DATA_ADDR <= ptr_addr;

	end process;

	--DATA_ADDR <= ptr_addr;

	reg_cnt: process(CLK, RESET, cnt_inc, cnt_dec)
	begin 
		
		if(RESET = '1') then
			cnt_addr <= "00000000";
		elsif(CLK'event and CLK = '1') then
			if(cnt_inc = '1') then
				cnt_addr <= cnt_addr + 1;
			elsif(cnt_dec = '1') then
				cnt_addr <= cnt_addr - 1;
			end if;
		end if;	

	end process;

	reg_pstate: process(CLK, RESET)
	begin

		if(RESET = '1') then
			pstate <= fetch;
		elsif(CLK'event and CLK = '1') then
			if(EN = '1') then
				pstate <= nstate;
			end if;
		end if;

	end process;

	fsm_nstate: process(CODE_DATA, IN_VLD, OUT_BUSY, DATA_RDATA, instruction, cnt_addr, pstate)
	begin

		pc_inc <= '0';
		pc_dec <= '0';
		ptr_inc <= '0';
		ptr_dec <= '0';
		cnt_inc <= '0';
		cnt_dec <= '0';
		CODE_EN <= '1';
		DATA_EN <= '0';
		OUT_WE <= '0';
		IN_REQ <= '0';
		sel <= "00";
		
		case pstate is
			when fetch =>
				CODE_EN <= '1';
				nstate <= p_decode;

			when p_decode =>
				case(instruction) is
					when ptr_inc_inst => nstate <= p_ptr_inc;
					when ptr_dec_inst => nstate <= p_ptr_dec;
					when var_inc => nstate <= p_var_inc;
					when var_dec => nstate <= p_var_dec;
					when while_start => nstate <= p_while_start;
					when while_end => nstate <= p_while_end;
					when while_break => nstate <= p_while_break;
					when putchar => nstate <= p_putchar;
					when getchar => nstate <= p_getchar;
					when halt => nstate <= pnull;
					when undefined => nstate <= p_undefined;
					when others =>
				end case;
			when p_ptr_inc =>
				ptr_inc <= '1';
				pc_inc <= '1';
				nstate <= fetch;

			when p_ptr_dec =>
				ptr_dec <= '1';
				pc_inc <= '1';
				nstate <= fetch;

			when p_var_inc =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				nstate <= p_var_inc_2;
			when p_var_inc_2 =>
				--DATA_WDATA <= DATA_RDATA + 1;
				sel <= "01";
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				pc_inc <= '1';
				nstate <= fetch;

			when p_var_dec =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				nstate <= p_var_dec_2;
			when p_var_dec_2 =>
				--DATA_WDATA <= DATA_RDATA - 1;
				sel <= "10";
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				pc_inc <= '1';
				nstate <= fetch;

			when p_putchar =>
				if(OUT_BUSY = '1') then
					nstate <= p_putchar;
				else
					DATA_EN <= '1';
					DATA_RDWR <= '0';
					nstate <= p_putchar_2;
				end if;
			when p_putchar_2 =>
				OUT_WE <= '1';
				OUT_DATA <= DATA_RDATA;
				pc_inc <= '1';
				nstate <= fetch;

			when p_getchar =>
				IN_REQ <= '1';
				if(IN_VLD = '0') then
					nstate <= p_getchar;
				else
					--DATA_WDATA <= IN_DATA;
					sel <= "00";
					DATA_EN <= '1';
					DATA_RDWR <= '1';
					pc_inc <= '1';
					nstate <= fetch;
				end if;

			when p_while_start =>
				pc_inc <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				nstate <= p_while_start_2;
			when p_while_start_2 =>
				if(DATA_RDATA = "00000000") then
					cnt_inc <= '1';
					nstate <= p_while_start_3;
				else
					nstate <= fetch;
				end if;
			when p_while_start_3 =>
				if(cnt_addr = "00000000") then
					nstate <= fetch;
				else
					CODE_EN <= '1';
					nstate <= p_while_start_4;
				end if;
			when p_while_start_4 =>
				if(CODE_DATA = X"5B") then
					cnt_inc <= '1';
				elsif(CODE_DATA = X"5D") then
					cnt_dec <= '1';
				end if;
				pc_inc <= '1';
				nstate <= p_while_start_3;

			when p_while_end =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				nstate <= p_while_end_2;
			when p_while_end_2 =>
				if(DATA_RDATA = "00000000") then
					pc_inc <= '1';
					nstate <= fetch;
				else
					nstate <= p_while_end_3;
				end if;
			when p_while_end_3 =>
				cnt_inc <= '1';
				pc_dec <= '1';
				nstate <= p_while_end_4;
			when p_while_end_4 =>
				if(cnt_addr = "00000000") then
					nstate <= fetch;
				else
					CODE_EN <= '1';
					nstate <= p_while_end_5;
				end if;
			when p_while_end_5 =>
				if(CODE_DATA = X"5D") then
					cnt_inc <= '1';
				elsif(CODE_DATA = X"5B") then
					cnt_dec <= '1';
				end if;
				nstate <= p_while_end_6;
			when p_while_end_6 =>
				if(cnt_addr = "00000000") then
					pc_inc <= '1';
				else
					pc_dec <= '1';
				end if;
				nstate <= p_while_end_4;

			when p_while_break =>
				pc_inc <= '1';
				cnt_inc <= '1';
				nstate <= p_while_start_3; -- zhodne kody pre break a while_start

			when pnull =>
				nstate <= pnull;

			when p_undefined =>
				pc_inc <= '1';
				nstate <= fetch;

			when others =>

		end case;

	end process;

end behavioral;

