#ifndef IR_HANDLER_H
#define IR_HANDLER_H

#include <Arduino.h>
#include <IRremote.hpp>
#include <ArduinoJson.h>
#include <Adafruit_SSD1306.h>
#include <GyverButton.h>
#include "tvbgone.h"
#include "cat_play.h"

// Переменные меню ИК
bool ir_menu_active = true;
int8_t ir_pointer = 0;   // Указатель на пункт меню
#define IR_MENU_ITEMS 3  // Количество пунктов в меню

extern Adafruit_SSD1306 oled;  // Экстерн для доступа к дисплею
extern GButton ok;             // Экстерн для кнопки ok

bool ir_app_run = true;
bool isSavingMode = false;  // Режим подтверждения сохранения
int selectedOption = 0;     // 0 - Yes, 1 - No

// Структура для хранения данных IR-сигнала
struct IRSignal {
  String protocol;
  uint32_t address;
  uint32_t command;
  uint32_t rawData;

  String toJson() {
    StaticJsonDocument<200> doc;
    doc["protocol"] = protocol;
    doc["address"] = address;
    doc["command"] = command;
    doc["rawData"] = rawData;

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
  }
};

// Прототипы функций
void displayIrMenu(MenuItem irMenu[], int menuSize, int8_t& pointer);
void handleIrMenuNavigation(MenuItem irMenu[], int menuSize, int8_t& pointer);
void executeIrMenuAction();
void drawIrPointer(uint8_t pointer);
void irBlaster();  // Функция для записи ИК
void displaySaveMenu();
void exit_ir();
void irRecieve();
void TvBgone();
void irInit();
void ir_app();


// Создаем массив пунктов меню с пиктограммами
MenuItem irMenu[] = {
  { "TvBgone", TvBgone, ir_icon },
  { "Analyze IR", irRecieve, record_icon },
  { "Back", exit_ir, back_icon }
};

#define MAIN_IR_MENU_SIZE (sizeof(irMenu) / sizeof(irMenu[0]))

IRSignal irSignal;  // Экземпляр структуры для хранения сигнала

void ir_app() {
  int8_t pointer = 0;  // Переменная указатель на элемент меню

  ir_app_run = true;
  while (ir_app_run) {
    ok.tick();
    displayIrMenu(irMenu, MAIN_IR_MENU_SIZE, pointer);
    handleIrMenuNavigation(irMenu, MAIN_IR_MENU_SIZE, pointer);
  }
}

// Функция для отображения меню ИК
void displayIrMenu(MenuItem irMenu[], int menuSize, int8_t& pointer) {
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
    if (irMenu[menuIndex].icon != nullptr) {
      // Вычисляем координаты для центрирования
      int iconX = 8;                                             // Центрирование по X
      int iconY = i * itemHeight + (itemHeight - iconSize) / 2;  // Центрирование по Y
      oled.drawBitmap(iconX, iconY, irMenu[menuIndex].icon, iconSize, iconSize, SSD1306_WHITE);
      oled.setCursor(iconX + iconSize + 4, iconY + 1);  // Сдвиг текста после иконки
    } else {
      oled.setCursor(boxX + 4, i * itemHeight + 4);  // Текст без иконки
    }

    oled.println(irMenu[menuIndex].name);
  }

  oled.display();  // Обновление дисплея
}

// Функция для обработки IR-сигнала и сохранения
void irRecieve() {
  bool ir_ir_app_run = true;
  irInit();

  while (ir_ir_app_run) {
    ok.tick();

    if (!isSavingMode) {  // Обычный режим, ждем сигнал
      if (IrReceiver.decode()) {
        if (IrReceiver.decodedIRData.protocol != UNKNOWN) {
          irSignal.protocol = getProtocolString(IrReceiver.decodedIRData.protocol);
          irSignal.address = IrReceiver.decodedIRData.address;
          irSignal.command = IrReceiver.decodedIRData.command;
          irSignal.rawData = IrReceiver.decodedIRData.decodedRawData;

          isSavingMode = true;
          selectedOption = 0;
          displaySaveMenu();  // Переход в режим сохранения
        }
        IrReceiver.resume();
      }
    } else {  // Режим сохранения
      if (ok.isClick()) {
        selectedOption = (selectedOption == 0) ? 1 : 0;
        displaySaveMenu();
      }

      if (ok.isHolded()) {
        if (selectedOption == 0) {
          Serial.println(irSignal.toJson());  // Сохранение данных
        }

        isSavingMode = false;
        irInit();  // Возвращаемся в режим ожидания сигнала
      }
    }

    if (!isSavingMode && ok.isHolded()) {
      while (!ok.isRelease()) {
        ok.tick();
      }
      ir_ir_app_run = false;
    }
  }
}

void displaySaveMenu() {
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.print("Protocol: ");
  oled.println(irSignal.protocol);
  oled.print("Address: 0x");
  oled.println(irSignal.address, HEX);
  oled.print("Command: 0x");
  oled.println(irSignal.command, HEX);

  oled.setCursor(0, 40);  // Отступ от данных

  // Отображаем Yes/No с выделением выбранной опции
  oled.print("Save? ");
  if (selectedOption == 0) {
    oled.print("> Yes   No");
  } else {
    oled.print("  Yes > No");
  }

  oled.display();
}

// Функция для обработки навигации по меню ИК
void handleIrMenuNavigation(MenuItem irMenu[], int menuSize, int8_t& pointer) {
  // Навигация по пунктам меню
  if (ok.isClick()) {
    pointer++;
    if (pointer >= menuSize) {
      pointer = 0;  // Возврат к первому элементу при выходе за пределы
    }
  }

  // Выполнение действия при удерживании кнопки
  if (ok.isHolded()) {
    while (!ok.isRelease()) {
      ok.tick();  // Ожидание отпускания кнопки
    }
    irMenu[pointer].action();  // Вызов функции из массива меню
  }
}


// Начальная функция для подготовки ИК-приемника и отображения
void irInit() {
  ir_app_run = true;
  IrReceiver.begin(IR_RECEIVE_PIN);

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.drawBitmap(0, 16, cat_play, 128, 48, 1);
  oled.print("Waiting for signal...");
  oled.display();
}

void exit_ir() {
  ir_app_run = false;
}

#endif  // IR_HANDLER_H
