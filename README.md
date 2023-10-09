# MecanumVehicleControl

MecanumVehicleControl is a comprehensive desktop application designed to facilitate control over a mecanum vehicle, including its linear arm and gripper. The project is intricately divided into three components: a server hosted on BeagleBoneBlack, an Arduino client running on ESP32, and a user interface client executing on the Desktop.

## Project Structure

The repository comprises three distinct folders:

1. **Motor_Control:** This folder houses the user interface with all requisite functionalities.
2. **QT_Network:** The server component, responsible for running on the primary controller (BeagleBoneBlack).
3. **Stepper_Servo_Control:** Executed on the ESP32, this client handles motor control tasks.

For seamless operation, all three devices must be connected within the same network, typically established through a basic router.

![System Architecture](https://github.com/RoboInnovatorX/MecanumVehicleControl/assets/61263042/95e4d9c7-a457-46b5-84e8-11699bb1d8fb)

## User Interface

The desktop user interface is thoughtfully designed, featuring three distinct windows catering to specific tasks. The primary window is dedicated to establishing a connection between the user and the server/main controller.

![Main Window](https://github.com/RoboInnovatorX/MecanumVehicleControl/assets/61263042/96b5e906-d74d-4d5f-8685-b941c1cdde7c)

### Main Window Functionalities:

1. **Connection Establishment:** Allows users to input specific IP addresses and server ports to connect to the desired server.
2. **Direct Mode:** Facilitates running individual motors using Trinamic Motion Control Language (TMCL) commands for configuration and control.
3. **Moving Mode:** Enables setting velocity for both X and Y directions.
4. **Specific Mode:** Developed to move to a specific position using known X and Y coordinates, with a fixed start position.
5. **Additional Controls:** Buttons for resetting the motor to 0, and initiating Direct Drive mode.

![Direct Drive Window](https://github.com/RoboInnovatorX/MecanumVehicleControl/assets/61263042/a89471a4-6c3c-4a46-8438-b3b32cf4e833)

### Direct Drive Window Functionalities:

1. **Directional Movement:** Allows the vehicle to move in 10 different directions.
2. **Speed Control:** Utilizes a slider to adjust the speed of the vehicle.
3. **Gripper Control:** Another slider facilitates the opening and closing of the gripper.
4. **Linear Guide Control:** Includes buttons to move the linear guide up and down.
5. **Automation Functionality:** Allows recording and playback of movements for specific tasks.
    - Start recording, stop, and delete recording vectors.
    - Playback recorded movements.
    - Save specific vectors to a file for future use.

This application empowers users with precise control and automation capabilities, enhancing the functionality of mecanum vehicles.




