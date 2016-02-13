CALL dfu-programmer atmega16u2 erase
CALL dfu-programmer atmega16u2 flash --suppress-bootloader-mem Arduino-COMBINED-dfu-usbserial-atmega16u2-Uno-Rev3.hex
CALL dfu-programmer atmega16u2 reset