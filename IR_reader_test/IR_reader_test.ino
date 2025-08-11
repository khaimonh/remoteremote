#include <IRremote.h>

// Pin connected to the IR receiver
const int recv_pin = 11;

IRrecv irrecv(recv_pin);
decode_results results;

void setup()
{
  // Start the serial monitor
  Serial.begin(9600);
  // Start the IR receiver
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {
    // Print the decoded signal
    Serial.println(results.value, HEX);

    // Optional: To print the raw data (in case you need more details)
    // irrecv.resume(); // Receive the next value
    irrecv.resume();  // Continue to the next IR signal
  }
}
