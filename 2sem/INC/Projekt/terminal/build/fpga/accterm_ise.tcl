#============================================================================
# Run: 
#    xtclsh accterm_ise.tcl  - creates XILINX ISE project file
#    ise accterm_project.ise - opens the project
#============================================================================
source "../../../../../base/xilinxise.tcl"

project_new "accterm_project"
project_set_props
puts "Adding source files"
xfile add "../../../../../fpga/units/clkgen/clkgen_config.vhd"
xfile add "accterm_config.vhd"
xfile add "../../../../../fpga/units/clkgen/clkgen.vhd"
xfile add "../../../../../fpga/chips/fpga_xc3s50.vhd"
xfile add "../../../../../fpga/ctrls/keyboard/keyboard_ctrl.vhd"
xfile add "../../../../../fpga/ctrls/keyboard/keyboard_ctrl_high.vhd"
xfile add "../../../../../fpga/ctrls/lcd/lcd_fsm_main.vhd"
xfile add "../../../../../fpga/ctrls/lcd/lcd_fsm_cycle.vhd"
xfile add "../../../../../fpga/ctrls/lcd/lcd_ctrl_rw.vhd"
xfile add "../../../../../fpga/ctrls/lcd/lcd_ctrl_high.vhd"
xfile add "../../../../../fpga/units/math/math_pack.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc_entity.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc_autoincr.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_reg.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_ctrl.vhd"
xfile add "../../fpga/rom_memory.vhd"
xfile add "../../fpga/fsm.vhd"
xfile add "../../fpga/fpga_mcu_dbg.vhd"
xfile add "../../fpga/fpga.vhd"
puts "Adding simulation files"
xfile add "../../../../../fpga/models/keyboard/keyboard.vhd" -view Simulation
xfile add "../../../../../fpga/models/lcd/lcd.vhd" -view Simulation
xfile add "../../fpga/sim/tb.vhd" -view Simulation
puts "Libraries"
project_set_isimscript "accterm_xsim.tcl"
project_set_top "fpga"
project_close
