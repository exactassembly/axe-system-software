import smbus2
import time

TPS55288_REG_VREF_LSB      = 0x00
TPS55288_REG_VREF_MSB      = 0x01
TPS55288_REG_IOUT_LIM      = 0x02
TPS55288_REG_VOUT_SR       = 0x03
TPS55288_REG_VOUT_FS       = 0x04
TPS55288_REG_CDC           = 0x05
TPS55288_REG_MODE          = 0x06
TPS55288_REG_STATUS        = 0x07

TPS55288_MODE_ENABLED      = 0x80
TPS55288_MODE_HICCUP       = 0x20
TPS55288_MODE_DISCHARGE    = 0x10
TPS55288_VOUTFS_EXTFB      = 0x80
TPS55288_VOUTFS_FS_MASK    = 0x03
TPS55288_VOUTSR_OCP_MASK   = 0x30
TPS55288_VOUTSR_OCP_SHIFT  = (4)
TPS55288_VOUTSR_SR_MASK    = 0x03
TPS55288_VOUTSR_SR1_25MVUS = 0x00
TPS55288_VOUTSR_SR2_50MVUS = 0x01
TPS55288_VOUTSR_SR5_00MVUS = 0x02
TPS55288_VOUTSR_SR10_0MVUS = 0x03

TPS55288_CDC_SC            = 0x80
TPS55288_CDC_OCP           = 0x40
TPS55288_CDC_OVP           = 0x20
TPS55288_CDC_EXT           = 0x08
TPS55288_CDC_INT_MASK      = 0x07

TPS55288_STATUS_SC         = 0x80
TPS55288_STATUS_OCP        = 0x40
TPS55288_STATUS_OVP        = 0x20
TPS55288_STATUS_REG_MASK   = 0x03
TPS55288_STATUS_REG_BOOST  = 0x00
TPS55288_STATUS_REG_BUCK   = 0x01
TPS55288_STATUS_REG_BKBST  = 0x02


sb = smbus2.SMBus(0) 

#ilim = sb.read_byte(0x74,0x02)
#cdc = sb.read_byte(0x74,0x05)
#mode = sb.read_byte(0x74,0x06)

def dumpregs():
    regblock = sb.read_i2c_block_data(0x74,0,8)
    print(regblock)

def fb_ratio(vout_fs):
    intfbr = (TPS55288_VOUTFS_FS_MASK & vout_fs)
    if intfbr == 0x03:
        return 0.0564
    if intfbr == 0x02:
        return 0.0752
    if intfbr == 0x01:
        return 0.1128
    if intfbr == 0x00:
        return 0.2256
    raise Exception("impossible fallthrough")

def v_step(vout_fs):
    intfbr = (TPS55288_VOUTFS_FS_MASK & vout_fs)
    return (intfbr + 1) * 5

def vref_mv(vref_reg):
    return 0.045 + (0.001129 * vref_reg)

def read_vref():
    vref_lsb = sb.read_byte(0x74,0x00)
    vref_msb = sb.read_byte(0x74,0x01);
    return (((vref_msb & 0x03) << 8) | vref_lsb)

def set_vref(vout_goal,fb_ratio):
    vref_goal = round((((vout_goal * 1000.0) * fb_ratio) - 45) / 1.129); # in mV
    # We need an int in order to properly set our vref, so round vref_goal to a whole number
    vref_goal = round(vref_goal)
    sb.write_byte_data(0x74,TPS55288_REG_VREF_LSB,(0xFF & vref_goal))
    sb.write_byte_data(0x74,TPS55288_REG_VREF_MSB,(0x03 & (vref_goal >> 8)))

sb.write_byte_data(0x74,TPS55288_REG_VOUT_FS,0)
set_vref(3.3,fb_ratio(0))
sb.write_byte_data(0x74,TPS55288_REG_MODE,TPS55288_MODE_ENABLED)
dumpregs()
time.sleep(0.5)
sb.write_byte_data(0x74,TPS55288_REG_MODE,TPS55288_MODE_DISCHARGE)
dumpregs()
