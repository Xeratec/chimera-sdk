# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

adapter speed 8000
adapter driver ftdi
ftdi_vid_pid 0x0403 0x6014
ftdi_layout_init 0x00e8 0x60eb
ftdi_channel 0
set irlen 5

transport select jtag
telnet_port disabled
tcl_port disabled
gdb_port 3333
reset_config none

set _CHIPNAME riscv
jtag newtap $_CHIPNAME cpu -irlen ${irlen} -expected-id 0x1c5e5db3

set _TARGETNAME $_CHIPNAME.cpu
target create $_TARGETNAME riscv -chain-position $_TARGETNAME -coreid 0

gdb_report_data_abort enable
gdb_report_register_access_error enable

riscv set_reset_timeout_sec 120
riscv set_command_timeout_sec 120

# riscv set_prefer_sba off

# Set SCRATCH0 to 2 after reset to indicate interactive mode
$_TARGETNAME configure -event reset-end {
    echo "Setting SCRATCH0 to 2 for interactive test mode."
    mww 0x03000000 0x00000002
}

init
halt
echo "Ready for Remote Connections."