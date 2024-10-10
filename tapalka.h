#include <Adafruit_SSD1306.h>
#include <GyverButton.h>
#include "homyak.h"

extern Adafruit_SSD1306 oled;
extern GButton ok;

int coins = 0;

void tapalka() {
  bool exitScene = false;
  while (!exitScene) {
    ok.tick();  // Always check button state

    // Clear the display and set text size/color
    oled.clearDisplay();
    oled.setTextSize(1);  // Regular size for the first line
    oled.setCursor(0, 0);
    oled.drawBitmap(48, 32, homyak, 32, 32, 1);
    oled.println("Hamster Kombat");
    oled.setTextSize(2);
    oled.print(coins);
    oled.print(" coins");
    oled.display();  // Update the display

    if (ok.isClick()) {
      coins++;
    }

    // Check for button hold to exit the scene
    if (ok.isHolded()) {
      exitScene = true;
    }
  }

  // Clear the display on exit
  oled.setTextSize(1);
  oled.clearDisplay();
  oled.display();
}
