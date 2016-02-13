// choose to SPI or I2C or HSU
#if 0
  #include <SPI.h>
  #include <PN532_SPI.h>
  #include "PN532.h"

  PN532SPI pn532spi(SPI, 10);
  PN532 nfc(pn532spi);
#elif 0
  #include <PN532_HSU.h>
  #include <PN532.h>
      
  PN532_HSU pn532hsu(Serial1);
  PN532 nfc(pn532hsu);
#else 
  #include <Wire.h>
  #include <PN532_I2C.h>
  #include <PN532.h>

  PN532_I2C pn532i2c(Wire);
  PN532 nfc(pn532i2c);
#endif


#define MODE_KEYBOARD 1
#define MODE_DEFAULT 0
#define OUTPUT_MODE MODE_KEYBOARD

#define KEY_LEFT_SHIFT  0x02

void setup(void)
{
  #if OUTPUT_MODE == MODE_KEYBOARD
    Serial.begin(9600);
  #elif OUTPUT_MODE == MODE_DEFAULT
    Serial.begin(115200);
    Serial.println("Looking for PN532...");
  #endif
  delay(200);

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata)
  {
    #if OUTPUT_MODE == MODE_DEFAULT
      Serial.println("Failed nfc init");
    #endif
    while (1); // halt
  }
  nfc.SAMConfig();
}

uint8_t buf[8] = { 0 };  // Keyboard report buffer
void emulateKey(char ch)
{
  #if OUTPUT_MODE == MODE_KEYBOARD
    if ((ch >= 'a') && (ch <= 'z'))
    {
      buf[2] = ch - 'a' + 4;
    }
    else if ((ch >= 'A') && (ch <= 'Z'))
    {
      buf[0] = KEY_LEFT_SHIFT;  // Caps 
      buf[2] = ch - 'A' + 4;
    }
    else if ((ch >= '0') && (ch <= '9'))
    {
      if (ch == '0')
      {
        buf[2] = 0x27;
      }
      else
      {
        buf[2] = ch - '0' - 1 + 0x1E;
      }
    }
    else
    {
      switch (ch)
      {
        case ' ':
          buf[2] = 0x2c;  // Space
          break;
        case '\r':
          buf[2] = 0x28;  // Carriage return
          break;
        default:
          buf[2] = 0x33;  // colon
          // Character not handled. TODO: add rest of chars http://www.mindrunway.ru/IgorPlHex/USBKeyScan.pdf 
          break;
      }
    }  
    Serial.write(buf, 8); // Send keypress
    buf[0] = 0;
    buf[2] = 0;
    Serial.write(buf, 8); // Release key
  #elif OUTPUT_MODE == MODE_DEFAULT
    Serial.print(ch);
  #endif  
}

void loop(void)
{
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  #if OUTPUT_MODE == MODE_DEFAULT
    Serial.println("FAILED READ ");
  #endif
  
  if (success && (uidLength == 4)) // mifare classic 1k
  {
    uint8_t keya[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 }; // my NFC tag auth key
  
    // Start with block 4 (the first block of sector 1)
    success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
  
    if (success)
    {
      short size = 16;
      uint8_t data[size];

      nfc.mifareclassic_ReadDataBlock(4, data);
      for (byte i = 9; i < size; ++i)
      {
        emulateKey(char(data[i]));
      }

      nfc.mifareclassic_ReadDataBlock(5, data);
      for (byte i = 0; i < 3; ++i)
      {
        emulateKey(char(data[i]));
      }
      emulateKey('\r');
      
      delay(5000); // Wait a bit before reading the card again
    }
  }
}

