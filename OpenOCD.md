----- /usr/share/openocd/scripts/interface/fwaxe.cfg ----
#
# Config for using RaspberryPi's expansion header
#
# This is best used with a fast enough buffer but also
# is suitable for direct connection if the target voltage
# matches RPi's 3.3V
#
# Do not forget the GND connection, pin 6 of the expansion header.
#

adapter driver sysfsgpio

# Each of the JTAG lines need a gpio number set: tck tms tdi tdo
# Header pin numbers: 23 22 19 21
sysfsgpio_jtag_nums 2042 2041 2044 2043

# Each of the SWD lines need a gpio number set: swclk swdio
# Header pin numbers: 23 22
sysfsgpio_swd_nums 2045 2046

# If you define trst or srst, use appropriate reset_config
# Header pin numbers: TRST - 26, SRST - 18

# sysfsgpio_trst_num 7
# reset_config trst_only

# sysfsgpio_srst_num 24
# reset_config srst_only srst_push_pull

# or if you have both connected,
# reset_config trst_and_srst srst_push_pull

-------------- openocd.cfg

source [find interface/fwaxe.cfg]
transport select swd
source [find target/nrf52.cfg]