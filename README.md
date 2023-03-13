# Robot Control
This little script was made for a presentation in the context of a highschool project day, to introduce students into python robot control. It is designed for the [adeept robotic arm](http://www.adeept.com/robotic-arm-uno_p0118.html), but in principle it can be applied for differen robots based on arduino control and servos.

## Python - Arduino
The communication is based on a serial link, the baud rate might need to be adapted based on the board. To instantiate a RobotCtrl object you need to pass as argument the string which identifies the **COM port** to which the robot is connected, e.g. _COM5_.

Once communication is established you can use the RobotControl class to control the robot after resolving the TODOs or downloading the script with the solutions (not yet available).

The commands which are currently supported are listed below:
A command is build on the following logic:
* A Command Header (**CH**)
* A Message Header (**MH**) which specifies the payload
* A value if needed (**VAL**)

And the messages are built as follows:
````
CH:MH:VAL:MH:VAL:...
````

### Angualr Position Ctrl
````
CH:     POS
MH:     AX1, AX2, ...
VAL:    float in [0;180]

example message:

POS:AX1:135                 //sets axis 1 to 135 degrees
POS:AX0:45:AX3:180:AX4:0    //sets axis 0, 3 and 4 to 45 respectively 180 and 0 degrees
````

### Potentiometer Control
````
CH:     POT
MH:     ENA or DSA
VAL:    None

example message:

POT:ENA     //enables potentiometer control, disabeling python remote control
POT:DSA     //disables potentiometer control, enabeling python remote control
````
