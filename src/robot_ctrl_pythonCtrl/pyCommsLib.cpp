#include "pyCommsLib.h"
#include "Arduino.h"


#define size_of_array(arr) sizeof(arr) / sizeof(*arr)


String rawMsgFromPython = "NO_PYTHON_MESSAGE";
String payload = "";


void load_msg_to_python(String* msgName, String* msg, int numOfMsg) {
  // If we have the same number of data compared to the message
  payload = "";
  
  for (int i = 0; i < numOfMsg; i++) {
    payload.concat(msgName[i]);
    payload.concat(";");
    payload.concat(msg[i]);
    payload.concat(":");
  }
}


void receive_msg_from_python() {
  String msg = "";

  while (Serial.available()) {
    if (Serial.available()) {
      msg = Serial.readStringUntil('\n');
    }
  }

  if (msg != "") {
    rawMsgFromPython = msg;
  }

}


String latest_received_msg() { 
  return rawMsgFromPython;
}

void set_new_message_payload(String new_payload) {
  payload = new_payload;
}

void init_python_communication() {
  
  while (true) {    
    // if the python side sent a message
    if (Serial.available() > 0) {
      String tempMsg = Serial.readStringUntil('\n');
      if (tempMsg == "handshake1") {
        break;
      }
    }
  }

  long timer = millis();
  while (millis() - timer < 1000) {
    Serial.println("handshake2");
  }
  delay(1000);
   
  // empty buffer
  while (Serial.available()) {
    Serial.read();
  }
}

void sync() {
  receive_msg_from_python();
  String final_payload = "<echo_python;";
  final_payload.concat(latest_received_msg());
  final_payload.concat(":");
  final_payload.concat(payload); 
  final_payload.concat(">");
  Serial.println(final_payload);
}
