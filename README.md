# nfclogin
Some Arduino fun with Windows login

read your NFC tag with password, then send to virtual keyboard.

Components:
* Arduino Uno R3
* PN532_NFC_RFID (http://robocraft.ru/files/datasheet/PN532_NFC_RFID_Module_Manual_V3.pdf)


Libs:
* https://github.com/elechouse/PN532.git


Custom firmware for keyboard emulation:  ./Arduino-keyboard-0.3.hex

Steps:
* Compile and upload code from Arduino Studio
* Unlock firmware upgrade
* run apply.bat script (arduino now will emulate keyboard)
* Put a card on it

