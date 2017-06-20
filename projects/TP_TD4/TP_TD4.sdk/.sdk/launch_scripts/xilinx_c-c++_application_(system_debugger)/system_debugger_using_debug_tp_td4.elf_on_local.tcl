connect -url tcp:127.0.0.1:3121
source /home/ariel/Documents/VHDL/Vivado_prjts/TP_TD4/TP_TD4.sdk/design_1_wrapper_hw_platform_0/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-HS3 210299A17976"} -index 0
stop
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-HS3 210299A17976"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-HS3 210299A17976"} -index 0
dow /home/ariel/Documents/VHDL/Vivado_prjts/TP_TD4/TP_TD4.sdk/TP_TD4/Debug/TP_TD4.elf
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-HS3 210299A17976"} -index 0
con
