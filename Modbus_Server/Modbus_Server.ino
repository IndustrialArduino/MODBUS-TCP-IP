#include <SPI.h>
#include <Ethernet.h>       // Ethernet library v2 is required
#include <ModbusEthernet.h>

// Enter a MAC address and IP address for your controller below.
byte mac[] = {
  0x08, 0xD1, 0xF9, 0xA0, 0xFA, 0x20
};
IPAddress ip(192, 168, 1, 26); // The IP address will be dependent on your local network:
ModbusEthernet mb;              // Declare ModbusTCP instance

void setup() {
  Serial.begin(115200);     // Open serial communications and wait for port to open
  #if defined(AVR_LEONARDO)
  while (!Serial) {}        // wait for serial port to connect. Needed for Leonardo only
  #endif
  Ethernet.init(26);        // SS pin
  Ethernet.begin(mac, ip);  // start the Ethernet connection
  delay(1000);              // give the Ethernet shield a second to initialize
  mb.server();              // Act as Modbus TCP server
  mb.addHreg(0, 0);     // Add Holding register #100
  mb.addHreg(1, 0);             // Add HREG(1)
  mb.addHreg(2, 0);             // Add HREG(2)
  mb.addHreg(3, 0);             // Add HREG(3)

  Serial.println("Enter register index (0-3) and value separated by a space to write:");
}

void loop() {
  mb.task();                // Server Modbus TCP queries

  // Read and print the value of multiple registers
  for (int i = 0; i < 4; i++) {
    uint16_t registerValue = mb.Hreg(i);
    Serial.print("Register HREG(");
    Serial.print(i);
    Serial.print(") value: ");
    Serial.println(registerValue);
  }

  // Check if there is serial input
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read user input from serial monitor

    // Parse the input to get register index and value
    int regIndex;
    int regValue;
    if (sscanf(input.c_str(), "%d %d", &regIndex, &regValue) == 2) {
      if (regIndex >= 0 && regIndex < 4) { // Validate register index
        mb.Hreg(regIndex, regValue);  // Update the register with the new value
        Serial.print("Updated HREG(");
        Serial.print(regIndex);
        Serial.print(") with value: ");
        Serial.println(regValue);
      } else {
        Serial.println("Invalid register index. Please enter a value between 0 and 3.");
      }
    } else {
      Serial.println("Invalid input format. Please enter in 'index value' format.");
    }
  }

  delay(1000);  // Wait 1 second before checking again
}
