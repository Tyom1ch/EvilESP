#include <Adafruit_SSD1306.h>
#include <GyverButton.h>
#include "cat_play.h"

extern Adafruit_SSD1306 oled;
extern GButton ok;

void about_page() {
  bool exitScene = false;
  int posY = 24;  // Початкова позиція Y
  int direction = 1;  // Напрямок анімації: 1 - вниз, -1 - вверх
  unsigned long previousMillis = 0;
  const long interval = 100;  // Інтервал зміни позиції

  while (!exitScene) {
    ok.tick();  // Завжди перевіряйте стан кнопки

    // Перевірте, чи настав час для оновлення позиції
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Зміна позиції Y
      posY += direction;
      if (posY >= 26) direction = -1;  // Зміна напрямку на вверх
      else if (posY <= 22) direction = 1;  // Зміна напрямку на вниз
    }

    // Очищення дисплея і налаштування розміру/кольору тексту
    oled.clearDisplay();
    oled.setTextSize(2);  // Звичайний розмір для першого рядка
    oled.setCursor(0, 0);
    oled.drawBitmap(0, posY, cat, 128, 40, 1);
    oled.print("EspEvil\n");
    oled.setTextSize(1);
    oled.print("by @tyom1ch.");
    oled.display();  // Оновлення дисплея

    // Перевірка тривалої кнопки для виходу зі сцени
    if (ok.isHolded()) {
      exitScene = true;
    }
  }

  // Очищення дисплея при виході
  oled.setTextSize(1);
  oled.clearDisplay();
  oled.display();
}

