#include "pyCommsLib.h"
#include "Servo.h"

#define BAUDRATE 115200
#define MSG_HEADER 4
#define NB_AXES 5
#define AX_ID_IDX 2
#define SERVO_PINS {9, 6, 5, 3, 11}
#define ANALOG_PINS {0, 1, 2, 3, 6}

//error status
enum class RobotStatus {
  NO_ERROR = 0,
  ERROR,
  UNKNOWN_CMD,
  BUSY,
  NULL_POINTER,
  NO_AX_ERROR,
  INVALID_AX_ERROR
};

const int servo_pins[NB_AXES] = SERVO_PINS;
const int analog_pins[NB_AXES] = ANALOG_PINS;

//FUNC DECL//////////////////////////////////////////////////////
void handle_command(String cmd, bool* pot_ctrl, float* ax_vals, int max_ax);
bool unknown_command(String cmd);
void convert_analog_to_position(float* ax_vals, int max_ax);
bool extract_pos_command_value(String cmd, float* ax_vals, int max_ax);
bool potentiometer_ctrl_enable(String cmd, bool* pot_ctrl);
bool valid_ax_idx(int ax_idx, int min_idx, int max_idx);
void move_servos(float* ax_vals, int max_ax);

//SETUP /////////////////////////////////////////////////////////
Servo servos[NB_AXES];

void setup() {  
  //start serial com
  Serial.begin(BAUDRATE);
  init_python_communication();
  
  for(int i=0; i<NB_AXES; i++) {
    servos[i]=Servo();
    servos[i].attach(servo_pins[i]);
    servos[i].write(90);
    delay(1000);
  }
}

//MAIN LOOP//////////////////////////////////////////////////////
void loop() {
  static bool pot_ctrl=false; 
  static float ax_vals[NB_AXES]={90.,90.,90.,90.,90.};
  String cmd = latest_received_msg();

  handle_command(cmd, &pot_ctrl, ax_vals, NB_AXES);

  if(pot_ctrl)
    convert_analog_to_position(ax_vals, NB_AXES);
  
  move_servos(ax_vals, NB_AXES);
  
  sync();  
}

//FUNC DEF///////////////////////////////////////////////////////
void handle_command(String cmd, bool* pot_ctrl, float* ax_vals, int max_ax) { 
  
  if(unknown_command(cmd)){
    set_new_message_payload(String((int)RobotStatus::UNKNOWN_CMD));
    return;
  } else if(cmd == "NO_PYTHON_MESSAGE") {
    //do nothing
    set_new_message_payload("");
    return;
  }

  if(pot_ctrl==NULL) {
    set_new_message_payload(String((int)RobotStatus::NULL_POINTER)); 
    return false; 
  }

  if(!(*pot_ctrl) && cmd.startsWith("POS:")) {
    extract_pos_command_value(cmd, ax_vals, max_ax);
  } else if(cmd.startsWith("POT:")) {
    potentiometer_ctrl_enable(cmd, pot_ctrl);
  }
}

bool unknown_command(String cmd) {
  return !cmd.startsWith("POS:") &&
         !cmd.startsWith("POT:") &&
         cmd != "NO_PYTHON_MESSAGE";
}

void convert_analog_to_position(float* ax_vals, int max_ax) {
  if(ax_vals==NULL) {
    set_new_message_payload(String((int)RobotStatus::NULL_POINTER)); 
    return; 
  }
  
  for(int i=0; i<NB_AXES; i++) {
    ax_vals[i]=map(analogRead(analog_pins[i]), 0, 1023, 0, 180);
  }
}

bool extract_pos_command_value(String cmd, float* ax_vals, int max_ax) {
  //command is header looks like: XXX:
  String msg=cmd.substring(MSG_HEADER);
  int i=0, max_iter=msg.length(), ax_idx=0;

  if(ax_vals==NULL) {
    set_new_message_payload(String((int)RobotStatus::NULL_POINTER)); 
    return false; 
  }

  while(msg[0]!='\0' && i<max_ax) {
    if(!msg.startsWith("AX")) {
      set_new_message_payload(String((int)RobotStatus::NO_AX_ERROR));
      return false; 
    }
    
    ax_idx=String(msg[2]).toInt();
    msg.remove(0,MSG_HEADER);

    if(!valid_ax_idx(ax_idx, 0, max_ax)) {
      set_new_message_payload(String((int)RobotStatus::INVALID_AX_ERROR));
      return false; 
    }

    float ax_pos=constrain((msg.substring(0, msg.indexOf(":"))).toFloat(), 0., 180.);
    msg.remove(0, msg.indexOf(":")+1);

    ax_vals[ax_idx]=ax_pos;

    i++;
  }

  set_new_message_payload(String((int)RobotStatus::NO_ERROR));
  return true;
}

bool potentiometer_ctrl_enable(String cmd, bool* pot_ctrl) {
  String msg=cmd.substring(MSG_HEADER);

  if(pot_ctrl==NULL) {
    set_new_message_payload(String((int)RobotStatus::NULL_POINTER)); 
    return false; 
  }

  if(msg.startsWith("ENA")){
    *pot_ctrl=true;
  } else if(msg.startsWith("DSA")) {
    *pot_ctrl=false;
  } else {
    set_new_message_payload(String((int)RobotStatus::UNKNOWN_CMD));
    return false;
  }

  return true;
}

bool valid_ax_idx(int ax_idx, int min_idx, int max_idx) {
  return ax_idx>=min_idx && ax_idx<max_idx;
}

void move_servos(float* ax_vals, int max_ax) {
  if(ax_vals==NULL) {
    set_new_message_payload(String((int)RobotStatus::NULL_POINTER)); 
    return; 
  }
  
  for(int i=0; i<max_ax; i++) {
      servos[i].write(ax_vals[i]);
  }
}
