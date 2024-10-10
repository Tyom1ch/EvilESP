#include <Adafruit_SSD1306.h>
#include <GyverButton.h>
#include "cat_play.h"

extern Adafruit_SSD1306 oled;
extern GButton ok;


void indev_msg() {
  bool exitScene = false;

  while (!exitScene) {
    ok.tick();  // Always check button state

    // Clear the display and set text size/color
    oled.clearDisplay();
    oled.setTextSize(2);  // Regular size for the first line
    oled.setCursor(0, 0);
    oled.drawBitmap(0, 16, cat_play, 128, 48, 1);
    oled.print("In develop");
    oled.setTextSize(1);
    oled.display();  // Update the display

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
