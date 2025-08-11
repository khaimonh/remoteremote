#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// IR Receiver pin - you can change this to any available GPIO
const uint16_t RECV_PIN = 14;

// Create an IRrecv object
IRrecv irrecv(RECV_PIN);

// Storage for the received IR data
decode_results results;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("ESP32 Universal IR Remote Reader");
  Serial.println("=================================");
  
  // Start the IR receiver
  irrecv.enableIRIn();
  
  Serial.print("IR Receiver started on GPIO ");
  Serial.println(RECV_PIN);
  Serial.println("Point any IR remote at the receiver and press buttons...");
  Serial.println("Supports: TV, A/C, Audio, Fan, LED strips, and more!");
  Serial.println();
}

void loop() {
  // Check if we received an IR signal
  if (irrecv.decode(&results)) {
    
    Serial.println("🔥 IR Signal Detected!");
    Serial.println("========================");
    
    // Print the protocol
    Serial.print("Protocol: ");
    Serial.println(typeToString(results.decode_type));
    
    // Print the hex value
    Serial.print("Code: 0x");
    serialPrintUint64(results.value, HEX);
    Serial.println();
    
    // Print number of bits
    Serial.print("Bits: ");
    Serial.println(results.bits);
    
    // Print the state array for protocols that use it (mostly A/C and some complex remotes)
    if (hasACState(results.decode_type)) {
      Serial.print("State Length: ");
      Serial.println(results.bits / 8);
      Serial.print("State: ");
      for (uint16_t i = 0; i < results.bits / 8; i++) {
        if (results.state[i] < 0x10) Serial.print("0");
        Serial.print(results.state[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    
    // Show raw timing data (useful for unknown protocols)
    Serial.print("Raw Data (");
    Serial.print(results.rawlen - 1);
    Serial.print(" entries): ");
    
    // Print first few raw values to avoid spam
    int maxValues = min(20, (int)(results.rawlen - 1));
    for (uint16_t i = 1; i <= maxValues; i++) {
      Serial.print(results.rawbuf[i] * kRawTick);
      if (i < maxValues) Serial.print(", ");
    }
    if (maxValues < results.rawlen - 1) {
      Serial.print("... (");
      Serial.print(results.rawlen - 1 - maxValues);
      Serial.print(" more)");
    }
    Serial.println();
    
    // Provide guidance based on protocol
    if (results.decode_type == UNKNOWN) {
      Serial.println("⚠️  Unknown Protocol");
      Serial.println("   This might be a proprietary or less common protocol");
      Serial.println("   Raw timing data above can be used for replay");
    } else {
      Serial.println("✅ Known Protocol - Can be decoded and sent!");
      
      // Give some context about common protocols
      String protocolName = typeToString(results.decode_type);
      if (protocolName.indexOf("NEC") >= 0) {
        Serial.println("   → Common in: TVs, DVD players, audio systems");
      } else if (protocolName.indexOf("SAMSUNG") >= 0) {
        Serial.println("   → Common in: Samsung TVs, A/C units, appliances");
      } else if (protocolName.indexOf("LG") >= 0) {
        Serial.println("   → Common in: LG TVs, A/C units, audio systems");
      } else if (protocolName.indexOf("SONY") >= 0) {
        Serial.println("   → Common in: Sony TVs, audio systems, cameras");
      } else if (protocolName.indexOf("RC5") >= 0 || protocolName.indexOf("RC6") >= 0) {
        Serial.println("   → Common in: European TVs, set-top boxes");
      } else if (protocolName.indexOf("PANASONIC") >= 0) {
        Serial.println("   → Common in: Panasonic TVs, A/C units");
      } else if (hasACState(results.decode_type)) {
        Serial.println("   → A/C or complex appliance remote");
      }
    }
    
    Serial.println("========================");
    Serial.println();
    
    // Enable receiving the next value
    irrecv.resume();
  }
  
  delay(50);  // Small delay
}