# nfclogin
Some Arduino fun with Windows login

read your NFC tag with password, then send to virtual keyboard.

Components:
1) Arduino Uno R3
2) PN532_NFC_RFID (http://robocraft.ru/files/datasheet/PN532_NFC_RFID_Module_Manual_V3.pdf)


Libs:
1) https://github.com/elechouse/PN532.git 


Custom firmware for keyboard emulation:  ./Arduino-keyboard-0.3.hex


Steps:
1) Compile and upload code from Arduino Studio
2) Unlock firmware upgrade 
3) run apply.bat script (arduino now will emulate keyboard)
4) Put a card on it

