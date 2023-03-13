#ifndef PYCOMMSLIB_H
#define PYCOMMSLIB_H

#define size_of_array(arr) sizeof(arr) / sizeof(*arr)


#include <Arduino.h>

void load_msg_to_python(String* msgName, String* msg, int numOfMsg);

String latest_received_msg();

void set_new_message_payload(String new_payload);

void init_python_communication();

void sync();

#endif
