#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <cmath>
#include <thread>
#include <chrono>
#include <QVector>


QT_BEGIN_NAMESPACE
namespace Ui { class MotorControl; }
QT_END_NAMESPACE

class MotorControl : public QMainWindow
{
    Q_OBJECT
private:
    //QTcpSocket motor_control;
    const double L_m = 0.248; // Distance from wheel shaft to the gravity centre, X axis in m
    const double l_m = 0.286; // Distance from wheel shaft to the gravity centre, Y axis in m
    const double R_m = 0.05; // Wheel radius in m
    const double Pi = 3.1416; // Universal value of Pi
    const double wheel_base_ = 0.49;// Distance between center of one wheel to another(front and rear)

public:
    MotorControl(QWidget *parent = nullptr);
    ~MotorControl();
    QTcpSocket motor_control;
    double LeftFront, RightFront, LeftBack,RightBack; // Variable declaration for four motor
    double current_pos_x = 0.0; // assign initial pos as current pose x
    double current_pos_y = 0.0; // assign initial pos as current pose y

    // Funciton to calculate the driving data for each motor
    void mecanum_Drive(double Vx_ms, double Vy_ms, double theta_ms, double& Lf_micro_step_s, double& Rf_micro_step_s, double& Lb_micro_step_s, double& Rb_micro_step_s);
    void send_drive_data(); // To send the drive data to the server
    void send_pos_data(); // To send the pos data to the server
    double estimate_movement_distance(double speed_ms, double time_sec); // calculate the estimate distance of movement of the vehicle
    void send_gripper_data( int gripper_value ); // To send the grippper data to the server
    void send_linear_guide_data( bool up, bool down, int value); // To send the linear guide data to the server
    void move_to_pos(double target_X, double target_Y); // Function for moving to specific position

    // Function to record the current movement of the vehicle, gripper and also linear guide...
    void record_current_movement(double Vx_ms_rec, double Vy_ms_rec, double theta_rec, double elapsed_time, int movement_type, int gripper_value, bool linearGuideDown, bool linearGuideUp, int Value);
    void playback_recorded_movements(); // Function to playback the recorded movements

    // Structure to record all the essential movement data
    struct RecordedMovement {
        double leftFrontRec;     // Recorded data for left front movement
        double rightFrontRec;    // Recorded data for right front movement
        double leftBackRec;      // Recorded data for left back movement
        double rightBackRec;     // Recorded data for right back movement
        double Vx_ms_rec;        // Recorded linear velocity in the x-direction
        double Vy_ms_rec;        // Recorded linear velocity in the y-direction
        double theta_rec;        // Recorded angle (orientation)
        double elapsed_time;     // Recorded elapsed time during the movement
        int movement_type;       // Type of movement (Vehicle , Gripper or Linear guide)
        int gripper_value;       // Recorded gripper value
        bool linearGuideDown;    // Flag indicating if linear guide is down
        bool linearGuideUp;      // Flag indicating if linear guide is up
        int Value;               // Linear guide value for spcific flag
    };

    QVector<RecordedMovement> recorded_movements; // Vector to hold recorded movement instances
    void send_recorded_data(RecordedMovement& recorded_movement); // Function to send recorded movement data
    std::chrono::steady_clock::time_point movement_start_time_; // Declare a steady clock to track movement start time
    void save_recorded_vector(const QVector<RecordedMovement>& recorded_movements, const QString& filename); // Function to save the vectors
    void playback_recorded_vector(const QString& filename);

private slots:
    void on_qConnectButton_clicked(); // Connection establishment function
    void on_qDisconnectButton_clicked(); // Remove the connection from the server
    void on_qDirectModeButton_clicked(); // Function for direct Drive the vehicle
    void on_qTargetPositionButton_clicked(); // Function to move the vehicle to a specific pos
    void on_qTargetCurrentButton_clicked(); // Function to setup specific torque
    void OnConnected(); // Function to handle when connection is established
    void OnDisconnected(); // Function to handle when client is disconnected from the server
    void OnError(QAbstractSocket::SocketError error); // Funciton to handle error
    void OnDataReceived(); // Function to handle the data receive from the server
    void on_qTargetVelocityButton_clicked(); // Function to setup specific velocity
    void on_qMovingModeButton_clicked(); // Function to Move the vehicle
    void on_qResetButton_clicked(); // Function to set the motor reset (initial 0)
    void on_qDirectDriveButton_clicked(); // Function to handle the direct drive window
    void on_qJoyStickButton_clicked(); // Fucntion to handle the joystick window to control the vehicle


private:
    Ui::MotorControl *ui;




};
#endif // MOTORCONTROL_H
