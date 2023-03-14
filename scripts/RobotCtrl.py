# %% [markdown]
## Welcome to the Robotics Practical! 
### Objective:
#The objective of this practical is to familiarize yourself with the control of a simple robot
#using Python. This will allow to gain some insights in potential uses for Python and why it is
#such a popular language.</br></br>
#To complete the practical let the robot complete a virtual pick-and-place routine, by filling out
# the missing spots where there is a `TODO` in the script 
#This file is organized in such a way that we first have the definition of the functions and then
#an execution script where we make use of the defined functions to move.
#### Might be usefull:
# Use `Shift`+`Enter` to run cell and advance to the next to go through the script, so you don't
# have to use the mouse all the time to run the cell. Or just use `Ctrl`+`Enter` to run the current
# cell
# %%
#Imports
from time import sleep
try:
    #manage kernel run in parent dir
    from scripts.comms_wrapper import *
except:
    from comms_wrapper import *


class RobotCtrl(object):
    """Robot Control Class, you don't need to understand what a class is to do this practical"""
    #--- CALIBRATE THIS VALUES --------------------------------------------------------------------
    #Angular positions for gripper servo for the positions open and close gripper
    OPEN_POSITION = None            #TODO: change to calibrated value
    CLOSE_POSITION = None           #TODO: change to calibrated value

    #TODO:  implement the functions to move the other axis. Bound the input value of the 
    #       angular_position parameter between 0 and 180.
    #       Complete functions defined below:
    #       - rotate_ax0
    #       - rotate_ax1
    #       - rotate_ax2
    #       - rotate_ax3
    #       by replacing the code of each function with the relevant code (may be more than one 
    #       line)
    #
    #       Hint: use the function set_gripper_aperture as an example and if-else statements
    #       for enforcing the bound on the angular_position in [0;180]
    def rotate_ax0(self, angular_position):
        #prints a placeholder line, to be replaced with actual code
        print("\n~bzzzzz... bip bip bop.... bzzz~\n")

    def rotate_ax1(self, angular_position):
        #raises an error if this function is executed
        raise NotImplementedError
    
    def rotate_ax2(self, angular_position):
        raise NotImplementedError
    
    def rotate_ax3(self, angular_position):
        raise NotImplementedError

    #--- FUNCTIONS YOU CAN USE --------------------------------------------------------------------
    #--! DO NOT MODIFY !---------------------------------------------------------------------------
    def enable_potentiometer_control(self):
        """
        Allows you to control the robot with the potentiometers, disabeling remote control
        """
        ret = self._send_message_to_robot(f"POT:ENA")
        
        return bool(ret)
    
    def disable_potentiometer_control(self):
        """
        Disables potentiometer control, enabeling remote control
        """
        ret = self._send_message_to_robot(f"POT:DSA")
        
        return bool(ret)
    
    def set_gripper_aperture(self, aperture):
        """
        Sets Servo of gripper to the aperture defined by the aperture parameter
        """
        ret = self._send_message_to_robot(f"POS:AX4:{aperture}:")
        
        return bool(ret)
    
    def open_gripper(self):
        """
        Opens gripper once gripper has been calibrated
        """
        if self.OPEN_POSITION is None:
            print(
                "Calibration must be done first:\nuse set_gripper_aperture with a value between 0", 
                " and 180 to calibrate"
                )
            return False
        
        self._is_open = bool(self._send_message_to_robot(f"POS:AX4:{self.OPEN_POSITION}:"))

        return self._is_open

    def close_gripper(self):
        """
        Closes gripper once gripper has been calibrated
        """
        if self.CLOSE_POSITION is None:
            print(
                "Calibration must be done first:\nuse set_gripper_aperture with a value between 0", 
                " and 180 to calibrate"
                )
            return False
        
        self._is_open = not bool(self._send_message_to_robot(f"POS:AX4:{self.CLOSE_POSITION}:"))

        return not self._is_open
    
    def gripper_is_open(self):
        """
        returns boolean value (T/F):
            True if gripper is open
            False if gripper is closed
        """
        return self._is_open
    

    #--- FUNCTIONS YOU DON'T NEED TO CONCERN YOURSELF WITH ----------------------------------------
    #--!! DO NOT MODIFY !!-------------------------------------------------------------------------
    def __init__(self, com_port, baudrate, device_name = "Robot"):
        super(RobotCtrl, self).__init__()

        self._ard = Arduino(descriptiveDeviceName=device_name,
                            portName=com_port, baudrate=baudrate)
        self._ard.connect_and_handshake()

        self._is_open = False

    def reinit(self):
        self._ard.disconnect_arduino()
        self._ard.connect_and_handshake()

    def __del__(self):
        self._ard.disconnect_arduino()

    def _send_message_to_robot(self, msg):
        return self._ard.send_message(msg)

# %% 0. Find the COM port--------------------------------------------------------------------------
#TODO:  Find COM port to which you robot is connected and set the variable comport to the correct
#       value at line 155, e.g. COM5, COM6, ... 
#       To do so type Device Manager in the start search and open it. 
#       Then look under "Anschlüsse (COM & LPT)" or use the function:
#       find_used_com_ports()

#HINT:  print all COM ports in use, call the function once with the robot disconnected and once 
#       with it connected to see thich COM port is used by it.
find_used_com_ports()

com_port = None

# %% 1. Create robot control object----------------------------------------------------------------
#creating virtual robot, now you can control the robot by calling the above defined functions like:
#   - my_robot.open_gripper() or 
#   - my_robot.move_ax1(45)
#   - ...

baudrate=115200                         #"communication speed"
my_robot=RobotCtrl(com_port, baudrate)  #creating variable my_robot to control the robot

# %% 2. Play around with the robot-----------------------------------------------------------------
#TODO:  enable the potentiomoeter control mode by executing the code below and see how you can move 
#       each articulation independantly using the 5 blue potentiometers

my_robot.enable_potentiometer_control()

# %% 3. Calibration--------------------------------------------------------------------------------
#TODO:  Make sure that remote control is enabled by disabeling the potentiometer control by adding 
#       the relevant function call and then change the value of test_aperture variable to find the 
#       two calibration values for the OPEN_POSITION and CLOSE_POSITION variables. Replace "None" 
#       with the found values in 29 and 30. 

test_aperture=0

my_robot.set_gripper_aperture(test_aperture)

# %% 4. Virtual pick-and-place---------------------------------------------------------------------
#TODO:  Play with the robot and create a sequence of commands to generate a pick and place
#       movement. HEre some sample code to see how to use the robot controller:

#open gripper
my_robot.open_gripper()
sleep(0.5)              #wait 0.5 seconds for the movement to end

#test if gripper is open
if my_robot.gripper_is_open():
    print("Gripper is open")
else:
    print("Gripper did not open")

#rotate the ax0 to 45 degrees
my_robot.rotate_ax0(45)
sleep(1)

#close gripper
my_robot.close_gripper()
sleep(0.5)

#rotate the ax0 to 90 degrees
my_robot.rotate_ax0(90)
sleep(1)

print("Gripper open-close sequence completed")

# %%
