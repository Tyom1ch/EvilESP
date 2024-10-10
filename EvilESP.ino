#include "menu_icons.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <GyverButton.h>  // Либа кнопок

#define IR_RECEIVE_PIN 14
#define IR_TX_PIN 12
#define IN_BUTTON_PIN 17

struct MenuItem {
  const char* name;     // Название пункта меню
  void (*action)();     // Функция действия
  const uint8_t* icon;  // Указатель на битмап пиктограммы
};

#include "in_dev.h"
#include "ble_spammer.h"
#include "about.h"
#include "ir_handler.h"
#include "main_ui.h"
#include "tapalka.h"
#include "wifi_app.h"

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Кнопки
GButton ok(IN_BUTTON_PIN, HIGH_PULL);

// Константы
#define ITEMS 3           // Количество пунктов меню
#define TIME_SLEEP 15000  // Время до отключения дисплея в миллисекундах

// Переменные
int8_t pointer = 0;  // Переменная указатель на элемент меню
uint32_t timer_sleep = 0;
bool oled_sleep = false;  // Флаг состояния дисплея (включен/выключен)

// Определение меню
MenuItem mainMenu[] = {
  { "BLE Spammer", ble_spammer_with_display, bluetooth_icon },
  { "Wi-Fi", wifi_app, subghz_icon },
  { "IR Blaster", ir_app, ir_icon },
  { "Sub-Ghz", indev_msg, subghz_icon },  // Без пиктограммы
  { "Hamster Kombat", tapalka, game_icon },
  { "About", about_page, info_icon }  // Без пиктограммы
};

#define MAIN_MENU_SIZE (sizeof(mainMenu) / sizeof(mainMenu[0]))

void setup() {
  Wire.begin();
  Wire.setClock(400000L);

  pinMode(IR_TX_PIN, OUTPUT);

  ok.setDebounce(80);       // настройка антидребезга (по умолчанию 80 мс)
  ok.setTimeout(500);       // настройка таймаута на удержание (по умолчанию 500 мс)
  ok.setClickTimeout(100);  // настройка таймаута между кликами (по умолчанию 300 мс)

  // Инициализация OLED
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true)
      ;  // Ошибка инициализации
  }

  // Настройка дисплея
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
}

void loop() {
  ok.tick();                                                // Обработка кнопки
  handleMenuNavigation(mainMenu, MAIN_MENU_SIZE, pointer);  // Обработка навигации и действий
  displayMenu(mainMenu, MAIN_MENU_SIZE, pointer);           // Отображение меню

  // Таймер сна дисплея
  if (millis() - timer_sleep >= TIME_SLEEP && !oled_sleep) {
    sleepOled();  // Отключение дисплея после истечения времени
  }
}

void handleMenuNavigation(MenuItem menu[], int menuSize, int8_t& pointer) {
  // Навигация по пунктам меню
  if (ok.isClick()) {
    if (!oled_sleep) {
      pointer++;
    }
    wakeOled();
    if (pointer >= menuSize) {
      pointer = 0;  // Возврат к первому элементу при выходе за пределы
    }
  }

  // Выполнение действия при удерживании кнопки
  if (ok.isHolded()) {
    menu[pointer].action();  // Вызов функции из массива меню
    wakeOled();              // Пробуждение дисплея
  }
}

void displayMenu(MenuItem menu[], int menuSize, int8_t& pointer) {
  oled.clearDisplay();

  const int itemHeight = 16;  // Высота каждого элемента
  const int iconSize = 8;     // Размер пиктограммы (8x8)
  const int boxWidth = 120;   // Ширина рамки
  const int boxHeight = itemHeight;
  const int boxX = 4;
  const int radius = 4;
  const int visibleItems = 4;
  int firstVisibleItem = pointer - pointer % visibleItems;

  for (int i = 0; i < visibleItems; i++) {
    int menuIndex = firstVisibleItem + i;
    if (menuIndex >= menuSize) break;
    // Рисуем рамку вокруг выбранного пункта
    if (menuIndex == pointer) {
      oled.drawRoundRect(boxX, i * itemHeight, boxWidth, boxHeight, radius, SSD1306_WHITE);
    }

    // Проверяем наличие пиктограммы и отображаем её
    if (menu[menuIndex].icon != nullptr) {
      // Вычисляем координаты для центрирования
      int iconX = 8;                                             // Центрирование по X
      int iconY = i * itemHeight + (itemHeight - iconSize) / 2;  // Центрирование по Y
      oled.drawBitmap(iconX, iconY, menu[menuIndex].icon, iconSize, iconSize, SSD1306_WHITE);
      oled.setCursor(iconX + iconSize + 4, iconY + 1);  // Сдвиг текста после иконки
    } else {
      oled.setCursor(boxX + 4, i * itemHeight + 4);  // Текст без иконки
    }

    oled.println(menu[menuIndex].name);
  }

  oled.display();  // Обновление дисплея
}

void wakeOled() {
  if (oled_sleep) {
    oled_sleep = false;
    oled.ssd1306_command(SSD1306_DISPLAYON);  // Включаем дисплей
    setCpuFrequencyMhz(240);                  // Устанавливаем максимальную частоту (например, 240 МГц)
  }
  timer_sleep = millis();  // Сбрасываем таймер сна
}

void sleepOled() {
  oled_sleep = true;
  oled.ssd1306_command(SSD1306_DISPLAYOFF);  // Выключаем дисплей
  setCpuFrequencyMhz(80);                    // Устанавливаем минимальную частоту (например, 80 МГц)
}
