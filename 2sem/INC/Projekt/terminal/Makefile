BASE = ../../../
FCMAKE = C:/Program Files/FITkit/bin/fcmake.exe
FKFLASH = C:/Program Files/FITkit/bin/fkflash.exe
FKTERM = C:/Program Files/FITkit/bin/fkterm.exe
FKTEST = C:/Program Files/FITkit/bin/fktest.exe
PROJECT = project.xml
OUTPUTPREFIX = accterm
FPGACHIP = xc3s50
FPGASPEEDGRADE = 4
FPGAPACKAGE = pq208

all: dependencycheck build/accterm_f1xx.hex build/accterm_f2xx.hex build/accterm.bin

#MCU part
#=====================================================================
HEXFILE_F1XX = build/accterm_f1xx.hex
HEXFILE_F2XX = build/accterm_f2xx.hex

build/mcu/main_f1xx.o: mcu/main.c
	$(comp_tpl_f1xx)

build/mcu/main_f2xx.o: mcu/main.c
	$(comp_tpl_f2xx)

OBJFILES_F1XX = build/mcu/main_f1xx.o
OBJFILES_F2XX = build/mcu/main_f2xx.o

#FPGA part
#=====================================================================
BINFILE = build/accterm.bin
HDLFILES  = ../../../fpga/units/clkgen/clkgen_config.vhd
HDLFILES += ../../../fpga/units/clkgen/clkgen.vhd
HDLFILES += ../../../fpga/chips/fpga_xc3s50.vhd
HDLFILES += ../../../fpga/ctrls/keyboard/keyboard_ctrl.vhd
HDLFILES += ../../../fpga/ctrls/keyboard/keyboard_ctrl_high.vhd
HDLFILES += ../../../fpga/ctrls/lcd/lcd_fsm_main.vhd
HDLFILES += ../../../fpga/ctrls/lcd/lcd_fsm_cycle.vhd
HDLFILES += ../../../fpga/ctrls/lcd/lcd_ctrl_rw.vhd
HDLFILES += ../../../fpga/ctrls/lcd/lcd_ctrl_high.vhd
HDLFILES += ../../../fpga/units/math/math_pack.vhd
HDLFILES += ../../../fpga/ctrls/spi/spi_adc_entity.vhd
HDLFILES += ../../../fpga/ctrls/spi/spi_adc.vhd
HDLFILES += ../../../fpga/ctrls/spi/spi_adc_autoincr.vhd
HDLFILES += ../../../fpga/ctrls/spi/spi_reg.vhd
HDLFILES += ../../../fpga/ctrls/spi/spi_ctrl.vhd
HDLFILES += fpga/rom_memory.vhd
HDLFILES += fpga/fsm.vhd
HDLFILES += fpga/fpga_mcu_dbg.vhd
HDLFILES += fpga/fpga.vhd

build/accterm.bin: build/fpga/accterm.par.ncd build/fpga/accterm.pcf

PKGS_LIST = ../../../fpga/units/clkgen/package.xml
PKGS_LIST += ../../../fpga/chips/architecture_top/package.xml
PKGS_LIST += ../../../fpga/ctrls/keyboard/package.xml
PKGS_LIST += ../../../fpga/ctrls/lcd/lcd_ctrl_high.xml
PKGS_LIST += ../../../fpga/units/math/package.xml
PKGS_LIST += ../../../fpga/ctrls/spi/package.xml

include $(BASE)/base/Makefile.inc
