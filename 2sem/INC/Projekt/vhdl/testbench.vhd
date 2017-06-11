-- testbench.vhd: Testbench file
-- Author(s): Tomas Martinek <martinto at fit.vutbr.cz>
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_textio.all;
use ieee.numeric_std.all;
use std.textio.all;

use work.testbench_pkg.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity testbench is
end entity testbench;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of testbench is

   constant    CLK_PERIOD : time :=  10 ns;
   constant    PRESS_TIME : time := 100 ns;


   signal clk         : std_logic;
   signal reset       : std_logic;

   -- Input signals
   signal key         : std_logic_vector(15 downto 0);
   signal cnt_of      : std_logic;

   -- Output signals
   signal fsm_cnt_ce  : std_logic;
   signal fsm_mx_mem  : std_logic;
   signal fsm_mx_lcd  : std_logic;
   signal fsm_lcd_wr  : std_logic;
   signal fsm_lcd_clr : std_logic;


   -- Counter for ROM memory
   signal cnt_mem_ce : std_logic;
   signal cnt_mem_ld : std_logic;
   signal cnt_mem    : std_logic_vector(3 downto 0);
   signal cnt_mem_of : std_logic;



-- ----------------------------------------------------------------------------
--                      Architecture body
-- ----------------------------------------------------------------------------
begin

uut : entity work.fsm
port map (
   CLK         => clk,
   RESET       => reset,

   -- Input signals
   KEY         => key,
   CNT_OF      => cnt_of,

   -- Output signals
   FSM_CNT_CE  => fsm_cnt_ce,
   FSM_MX_MEM  => fsm_mx_mem,
   FSM_MX_LCD  => fsm_mx_lcd,
   FSM_LCD_WR  => fsm_lcd_wr,
   FSM_LCD_CLR => fsm_lcd_clr
);


-- -------------------------------------------------------------
-- Clock generator 
process
begin
   clk <= '1';
   wait for CLK_PERIOD/2;
   clk <= '0';
   wait for CLK_PERIOD/2;
end process;


-- -------------------------------------------------------------
-- Citac adresy pro LCD display 
cnt_mem_ce <= fsm_cnt_ce or fsm_lcd_clr;
cnt_mem_ld <= fsm_lcd_clr;
process(RESET, CLK)
begin
   if (RESET = '1') then
      cnt_mem <= (others => '0');
   elsif (CLK'event AND CLK = '1') then
      if (cnt_mem_ce = '1') then
         if (cnt_mem_ld = '1') then
            cnt_mem <= "0000";
         else
            cnt_mem <= cnt_mem + 1;
         end if;
      end if;
   end if;
end process;
cnt_mem_of <= '1' when (cnt_mem = "1111") else '0';
cnt_of     <= cnt_mem_of; 

-- ----------------------------------------------------------------------------
--                               Ukladani vyslekdu 
-- ----------------------------------------------------------------------------
write_result : process 
    variable i        : integer;
    file     f_output : text;
    variable out_line : line;

    constant STR_INPUT_TITLE : string := 
    "========================== Test slova ==========================";
    constant STR_NULL        : string := "# ";
    constant STR_KOD         : string := "# kod = ";
    constant STR_SEPARATOR   : string := "--------------";

begin

    file_open(f_output, FILE_NAME, WRITE_MODE);

    wait until reset = '0'; 

    for i in 1 to N_TEST_WORDS loop

        -- Zapis hlavicky pro pattern
        write(out_line, STR_INPUT_TITLE);
        writeline(f_output, out_line);
        write(out_line, STR_KOD);
        write(out_line, test_words(i));
        writeline(f_output, out_line);
        write(out_line, STR_NULL);
        writeline(f_output, out_line);

        while (reset='0') loop
            wait until (clk'event  and clk='0' and fsm_lcd_wr='1' ) or 
                       (clk'event  and clk='0' and fsm_lcd_clr='1') or
                       (reset='1');
                       
            if reset='1' then
                exit;
            end if;
            -- ----- Vstupy -----
            write(f_output, "key=");
            write(out_line, key);
            writeline(f_output, out_line);

            write(f_output, "cnt_of=");
            write(out_line, cnt_of);
            writeline(f_output, out_line);

            -- ----- Vystupy -----
            write(f_output, "fsm_cnt_ce=");
            write(out_line, fsm_cnt_ce);
            writeline(f_output, out_line);

            write(f_output, "fsm_mx_mem=");
            write(out_line, fsm_mx_mem);
            writeline(f_output, out_line);

            write(f_output, "fsm_mx_lcd=");
            write(out_line, fsm_mx_lcd);
            writeline(f_output, out_line);

            -- ------ Trigry -----
            write(f_output, "fsm_lcd_wr=");
            write(out_line, fsm_lcd_wr);
            writeline(f_output, out_line);

            write(f_output, "fsm_lcd_clr=");
            write(out_line, fsm_lcd_clr);
            writeline(f_output, out_line);


            write(out_line, STR_SEPARATOR);
            writeline(f_output, out_line);
        end loop;

        wait until reset='0';

    end loop;


    file_close(f_output);

    wait;
end process write_result;

-- ----------------------------------------------------------------------------
--                         Main testbench process
-- ----------------------------------------------------------------------------
tb : process

   -- -----------------------------------------------------------------
   -- Zasilani stisku klaves 
   procedure keypress(k: in character) is
   begin
      case k is 
         when '0'    => key <= "0000000000000001";
         when '1'    => key <= "0000000000000010";
         when '2'    => key <= "0000000000000100";
         when '3'    => key <= "0000000000001000";
         when '4'    => key <= "0000000000010000";
         when '5'    => key <= "0000000000100000";
         when '6'    => key <= "0000000001000000";
         when '7'    => key <= "0000000010000000";
         when '8'    => key <= "0000000100000000";
         when '9'    => key <= "0000001000000000";
         when 'A'    => key <= "0000010000000000";
         when 'B'    => key <= "0000100000000000";
         when 'C'    => key <= "0001000000000000";
         when 'D'    => key <= "0010000000000000";
         when '*'    => key <= "0100000000000000";
         when '#'    => key <= "1000000000000000";
         when others => key <=X"0000";
      end case;
      wait for CLK_PERIOD;
      key <= X"0000";
   end procedure keypress;

   -- -----------------------------------------------------------------
   -- Odeslani jednoho retezce
   procedure put_string(str: in string(1 to 15); press: in time) is
       variable i : integer;
   begin
      i:=1;

      while (str(i)/='#') and (i<=15) loop
          keypress(str(i));
          wait for press;
          i:=i+1;
      end loop;
      keypress('#');
      wait for press;

   end procedure put_string;

begin
   key   <= X"0000";
   reset <= '1';
   wait for 4*CLK_PERIOD;
   reset <= '0';

   for i in 1 to N_TEST_WORDS loop
        -- Vlozeni retezce znaku
        put_string(test_words(i), PRESS_TIME);

        -- Zobrazeni vysledku na LCD 
        wait for   CLK_PERIOD*20;     

        -- Reset zarizeni 
        reset <= '1';
        wait for 4*CLK_PERIOD;
        reset <= '0';
   end loop;


   wait;
end process;

end architecture behavioral;

