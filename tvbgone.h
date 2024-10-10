#include <IRremote.h>
#include <Adafruit_SSD1306.h>
#include <GyverButton.h>

extern Adafruit_SSD1306 oled;
extern GButton ok;


#define IR_INTERVAL 400

const uint16_t epson1[] = {
  8984, 4453, 599, 495, 599, 495, 573, 521, 599, 1615, 599, 495, 573, 521, 599, 495, 573, 521,
  599, 1615, 599, 1615, 599, 1615, 599, 495, 573, 1615, 573, 1615, 599, 1615, 599, 1615, 599,
  1615, 573, 521, 599, 1589, 599, 521, 573, 521, 573, 521, 573, 495, 599, 495, 573, 495, 599,
  1615, 599, 495, 573, 1615, 573, 1615, 599, 1615, 599, 1615, 599, 1615, 573, 40182, 8906, 2240,
  547, 100156, 8906, 2240, 547, 100156, 8906, 2240, 547, 100156
};

const uint16_t epson2[] = {
  8984, 4531, 521, 1719, 547, 1693, 547, 573, 547, 573, 547, 573, 547, 573, 547, 599, 521, 1719,
  521, 1719, 521, 599, 521, 1719, 547, 573, 547, 1693, 547, 573, 547, 1719, 521, 599, 521, 1719,
  521, 599, 521, 599, 521, 599, 547, 1693, 547, 573, 547, 573, 547, 1693, 547, 599, 521, 1719,
  521, 1719, 521, 1719, 547, 573, 547, 1693, 547, 1693, 547, 573, 547, 39193, 8906, 2240, 547,
  100156
};

// Raw IR signal for "power"
const uint16_t epson3[] = {
  8984, 4531, 521, 1719, 547, 1693, 547, 573, 547, 573, 547, 573, 547, 573, 547, 599, 521, 1719,
  521, 1719, 521, 599, 521, 1719, 573, 573, 547, 1693, 547, 573, 547, 1719, 521, 599, 521, 599,
  521, 599, 521, 599, 521, 599, 521, 1719, 547, 573, 547, 573, 547, 1693, 547, 1719, 521, 1719,
  521, 1719, 547, 1719, 547, 573, 547, 1693, 573, 1693, 547, 573, 547, 39193, 8906, 2240, 547,
  100156
};

const uint16_t epson4[] = {
  8473, 4248, 502, 564, 501, 1617, 502, 564, 501, 1617, 501, 564, 501, 1617, 502, 564, 502, 1643,
  501, 1617, 501, 564, 501, 1617, 502, 564, 501, 1617, 501, 565, 502, 1616, 501, 565, 501, 564,
  501, 565, 501, 1617, 501, 1617, 501, 1617, 502, 564, 501, 565, 501, 565, 501, 1617, 501, 1617,
  501, 565, 501, 565, 501, 565, 501, 1618, 501, 1617, 501, 1591, 501, 25190, 8473, 4249, 501, 565,
  501, 1617, 501, 565, 501, 1617, 501, 565, 501, 1617, 501, 565, 501, 1644, 501, 1617, 501, 565,
  501, 1617, 501, 565, 501, 1617, 501, 565, 501, 1617, 501, 565, 501, 565, 501, 566, 500, 1617,
  501, 1617, 501, 1617, 501, 565, 500, 565, 501, 565, 501, 1618, 500, 1617, 501, 565, 501, 565,
  501, 565, 501, 1618, 501, 1617, 501, 1591, 501, 25477
};

void TvBgone() {
  bool exitScene = false;

  IRsend irsend(IR_TX_PIN);

  unsigned long previousMillis = 0;
  int cycle = 54;

  IrSender.begin(IR_TX_PIN, true, 22);  // Инициализация IR-отправителя
  delay(100);                           // Allow IR transmitter to stabilize

  while (!exitScene) {
    ok.tick();  // Always check button state
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= IR_INTERVAL) {
      previousMillis = currentMillis;

      switch (cycle) {
        case 0: irsend.sendSony(0xA90, 12); break;              // SONY
        case 1: irsend.sendNEC(0x20DF10EF, 32); break;          // LG, SAMSUNG, etc.
        case 2: irsend.sendRC5(0x80C, 12); break;               // PHILIPS, etc.
        case 3: irsend.sendRC5(0xC, 12); break;                 // PHILIPS, etc.
        case 4: irsend.sendRC5(0x84C, 12); break;               // TELEFUNKEN, etc.
        case 5: irsend.sendRC5(0x4C, 12); break;                // SHARP, etc.
        case 6: irsend.sendNEC(0x4CB340BF, 32); break;          // OPTOMA PROJECTOR
        case 7: irsend.sendNEC(0xC1AA09F6, 32); break;          // EPSON PROJECTOR/TV
        case 8: irsend.sendNEC(0x189728D7, 32); break;          // NEC PROJECTOR
        case 9: irsend.sendRC5(0x44C, 12); break;               // PANASONIC
        case 10: irsend.sendSony(0xF50, 12); break;             // SONY
        case 11: irsend.sendNEC(0x2FD48B7, 32); break;          // TOSHIBA, etc.
        case 12: irsend.sendSamsung(0x6060F00F, 32, 1); break;  // AIKO
        case 13: irsend.sendNEC(0x86C6807F, 32); break;         // ACER
        case 14: irsend.sendSamsung(0xE0E040BF, 32, 1); break;  // SAMSUNG TV1
        case 15: irsend.sendSamsung(0xE0E019E6, 32, 1); break;  // SAMSUNG TV2
        case 16: irsend.sendSamsung(0x1010D02F, 32, 1); break;  // HAIER
        case 17: irsend.sendNEC(0x6F900FF, 32); break;          // BENQ
        case 18: irsend.sendNEC(0x1FE41BE, 32); break;          // TELEFUNKEN
        case 19: irsend.sendRC6((uint32_t)0x1000C, 20); break;  // PHILIPS RC6
        case 20: irsend.sendRC6((uint32_t)0xC, 20); break;      // PHILIPS RC6
        case 21: irsend.sendNEC(0x7B6B4FB0, 32); break;         // LG
        case 22: irsend.sendNEC(0x986718E7, 32); break;         // MEDION
        case 23: irsend.sendNEC(0x4B36E21D, 32); break;         // ONKYO
        case 24: irsend.sendNEC(0x4B36D32C, 32); break;         // ONKYO
        case 25: irsend.sendNEC(0x189710EF, 32); break;         // NEC PROJECTOR ON
        case 26: irsend.sendNEC(0x1897639C, 32); break;         // NEC PROJECTOR [SVIDEO]
        case 27: irsend.sendNEC(0xAB500FF, 32); break;          // YAMAHA
        case 28: irsend.sendNEC(0x1FE48B7, 32); break;          // FUJITSU
        case 29: irsend.sendNEC(0x3E060FC0, 32); break;         // AIWA
        case 30: irsend.sendRC5(0xC3D, 12); break;              // GRUNDIG FINE ARTS
        case 31: irsend.sendNEC(0xFB38C7, 32); break;           // GRUNDIG, MEDION
        case 32: irsend.sendRC5(0x301, 12); break;              // SHARP
        case 33: irsend.sendRC5(0xB01, 12); break;              // SHARP
        case 34: irsend.sendSamsung(0x909C837, 32, 1); break;   // SAMSUNG TV3
        case 35: irsend.sendRC5(0x9CC, 12); break;              // PHILIPS ITV
        case 36: irsend.sendNEC(0x20DFBE41, 32); break;         // LG TV3
        case 37: irsend.sendSamsung(0x909040BF, 32, 1); break;  // SAMSUNG
        case 38: irsend.sendNEC(0x55AA38C7, 32); break;         // PIONEER, SHARP
        case 39: irsend.sendNEC(0x1CE348B7, 32); break;         // SANYO
        case 40: irsend.sendNEC(0x1CE338C7, 32); break;         // HITACHI, SANYO, etc.
        case 41: irsend.sendNEC(0x10EFEB14, 32); break;         // AOC
        case 42: irsend.sendNEC(0x18E710EF, 32); break;         // NEC TV
        case 43: irsend.sendNEC(0xAF5FC03, 32); break;          // DENON
        case 44: irsend.sendNEC(0xBD807F, 32); break;           // PHILIPS LCD MONITOR
        case 45: irsend.sendNEC(0xC18F50AF, 32); break;         // VIEWSONIC
        case 46: irsend.sendNEC(0x8C73817E, 32); break;         // LENOVO
        case 47: irsend.sendNEC(0x38C7AC0A, 32); break;         // MALATA
        case 48: irsend.sendNEC(0xDE010FC0, 32); break;         // AIWA
        case 49: irsend.sendNEC(0xFD00FF, 32); break;           // TELEFUNKEN
        case 50: irsend.sendNEC(0x8E7152AD, 32); break;         // TOSHIBA
        case 51: irsend.sendRaw(epson1, sizeof(epson1) / sizeof(epson1[0]), 38); break;
        case 52: irsend.sendRaw(epson2, sizeof(epson2) / sizeof(epson2[0]), 38); break;
        case 53: irsend.sendRaw(epson3, sizeof(epson3) / sizeof(epson3[0]), 38); break;
        case 54: irsend.sendRaw(epson4, sizeof(epson4) / sizeof(epson4[0]), 38); break;
      }

      oled.clearDisplay();
      oled.setTextSize(2);  // Regular size for the first line
      oled.setCursor(0, 0);
      oled.print("BLASTING!\n");
      //oled.setTextSize(1);
      oled.print(cycle);
      oled.display();  // Update the display

      cycle = (cycle - 1) % 54;
      if (cycle <= 0) cycle = 54;
    }
    // Check for button hold to exit the scene
    if (ok.isHolded()) {
      oled.setTextSize(1);
      exitScene = true;
    }
  }
}
