#include <IRremote.h>
#include <FS.h>  // Filesystem support

IRsend irsend;

struct IRSignal {
  String name;
  String type;
  String protocol;
  uint32_t address;
  uint32_t command;
  uint32_t frequency;
  float dutyCycle;
  String rawData;
};

bool parseIRFile(FS &fs, const char *filepath) {
  File file = fs.open(filepath);
  if (!file) {
    Serial.println("Failed to open file");
    return false;
  }

  IRSignal irSignal;
  String line;
  
  while (file.available()) {
    line = file.readStringUntil('\n');
    line.trim(); // Remove extra spaces and line breaks

    // Skip comments
    if (line.startsWith("#") || line.length() == 0) {
      continue;
    }

    // Parse name
    if (line.startsWith("name:")) {
      irSignal.name = line.substring(5).trim();
      Serial.println("Name: " + irSignal.name);
    }

    // Parse type (parsed/raw)
    else if (line.startsWith("type:")) {
      irSignal.type = line.substring(5).trim();
      Serial.println("Type: " + irSignal.type);
    }

    // Parse protocol for parsed type
    else if (line.startsWith("protocol:")) {
      irSignal.protocol = line.substring(9).trim();
      Serial.println("Protocol: " + irSignal.protocol);
    }

    // Parse address for parsed type
    else if (line.startsWith("address:")) {
      String addressStr = line.substring(8).trim();
      irSignal.address = strtol(addressStr.c_str(), nullptr, 16);
      Serial.println("Address: " + String(irSignal.address, HEX));
    }

    // Parse command for parsed type
    else if (line.startsWith("command:")) {
      String commandStr = line.substring(8).trim();
      irSignal.command = strtol(commandStr.c_str(), nullptr, 16);
      Serial.println("Command: " + String(irSignal.command, HEX));

      // After reading parsed protocol, send the signal
      sendParsedIRSignal(irSignal);
    }

    // Parse frequency for raw type
    else if (line.startsWith("frequency:")) {
      irSignal.frequency = line.substring(10).toInt();
      Serial.println("Frequency: " + String(irSignal.frequency));
    }

    // Parse duty cycle for raw type
    else if (line.startsWith("duty_cycle:")) {
      irSignal.dutyCycle = line.substring(11).toFloat();
      Serial.println("Duty Cycle: " + String(irSignal.dutyCycle));
    }

    // Parse raw data for raw type
    else if (line.startsWith("data:")) {
      irSignal.rawData = line.substring(5).trim();
      Serial.println("Raw Data: " + irSignal.rawData);

      // After reading raw data, send the raw IR signal
      sendRawIRSignal(irSignal);
    }
  }

  file.close();
  return true;
}

// Function to send parsed IR signals
void sendParsedIRSignal(IRSignal signal) {
  if (signal.protocol == "NEC") {
    irsend.sendNEC(signal.address, signal.command);
  } else if (signal.protocol == "NECext") {
    irsend.sendNEC(signal.address, signal.command, 32);
  } else if (signal.protocol == "Samsung32") {
    irsend.sendSamsung(signal.command, 32);
  }
  // Add support for other protocols as needed
}

// Function to send raw IR signals
void sendRawIRSignal(IRSignal signal) {
  // Convert raw data to an array of integers
  int rawDataArray[500];  // Make sure this is large enough
  int rawDataLength = 0;

  char *data = strtok((char*)signal.rawData.c_str(), " ");
  while (data != nullptr) {
    rawDataArray[rawDataLength++] = atoi(data);
    data = strtok(nullptr, " ");
  }

  // Send the raw signal
  irsend.sendRaw(rawDataArray, rawDataLength, signal.frequency);
}

void irBlasterNew() {
    Serial.begin(115200);
  irsend.begin();

  // Replace with appropriate filesystem initialization (SPIFFS, SD, etc.)
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // Parse the IR file and send signals
  parseIRFile(SPIFFS, "/ir_signals.txt");
}
