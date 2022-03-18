# Source: https://www.thegeekpub.com/18263/raspberry-pi-to-arduino-i2c-communication/
# Source: https://pypi.org/project/smbus2/

import smbus
 
SLAVE_ADDRESS = 0x01                                            # I2C adresse from slave (ESP32) 

I2Cbus = smbus.SMBus(1)                                         # create the I2C bus
 
BytesToSend = 1                                                 # if command speed_up will be send
                                                                # specify the appropriate command
                                                                # no_command = 0 / speed_up = 1 / speed_down = 2 / stopp = 3
I2Cbus.write_i2c_block_data(SLAVE_ADDRESS, 0x00, BytesToSend)   # write a block of 1 bytes to SLAVE_ADDRESS from offset 0
                                                                # run this function only on an action 

block = I2Cbus.read_i2c_block_data(SLAVE_ADDRESS, 0, 3)         # read a block of 3 bytes from SLAVE_ADDRESS from offset 0
                                                                # run this function continuously every n seconds
