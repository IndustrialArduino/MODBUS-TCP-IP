#include <SPI.h>
#include <Ethernet.h>       // Ethernet library v2 is required
#include <ModbusEthernet.h>

const uint16_t REG = 00000;               // Modbus Hreg Offset
IPAddress remote(192, 168, 1, 19);  // Address of Modbus Slave device
const int LOOP_COUNT = 10;
const int32_t showDelay = 5000;   // Show result every n'th mellisecond

// Enter a MAC address and IP address for your controller below.
byte mac[] = {
  0x08, 0xD1, 0xF9, 0xA0, 0xFA, 0x20
};
IPAddress ip(192, 168, 1, 26); // The IP address will be dependent on your local network:
ModbusEthernet mb;               // Declare ModbusTCP instance

void setup() {
  Serial.begin(115200);     // Open serial communications and wait for port to open
  #if defined(AVR_LEONARDO)
  while (!Serial) {}        // wait for serial port to connect. Needed for Leonardo only
  #endif
  Ethernet.init(26);         // SS pin
  Ethernet.begin(mac,ip);  // start the Ethernet connection
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  delay(1000);              // give the Ethernet shield a second to initialize
  mb.client();              // Act as Modbus TCP client
}

uint16_t res[10];


void loop() {
if (mb.isConnected(remote)) {   // Check if connection to Modbus Slave is established
    Serial.println("connected");
    mb.readHreg(remote, REG, res,LOOP_COUNT);  // Initiate Read Hreg from Modbus Slave
    for(int i=REG+LOOP_COUNT;i<=REG+LOOP_COUNT+2;i++){
      uint16_t send_data =random(20,100);
      mb.writeHreg(remote, i, &send_data, 1);
    }
  } else {
    Serial.println("notconnected");
    mb.connect(remote);           // Try to connect if not connected
  }                    
  mb.task();                      // Common local Modbus task
   delay(100);
   for(int j=0; j<=LOOP_COUNT-1; j++){ // Display register value every 5 seconds (with default settings)

    Serial.println(res[j]);
  }
  Serial.println();
}
