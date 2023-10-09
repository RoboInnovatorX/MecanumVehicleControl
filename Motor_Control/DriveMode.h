#ifndef DRIVEMODE_H
#define DRIVEMODE_H

#include <QDialog>
#include "MotorControl.h"
#include "ReadWriteXml.h"
#include <QTcpSocket>
#include <QTimer>
#include <QFile>
#include <QFileDialog>

namespace Ui {
class DriveMode;
}

class DriveMode : public QDialog
{
    Q_OBJECT

public:
    explicit DriveMode(MotorControl* a,QWidget *parent = nullptr);
    ~DriveMode();


private slots:
    void on_Forward_Button_pressed(); // Function to go forward
    void on_Backward_Button_pressed(); // Function to go backward
    void on_Left_Button_pressed(); // Function to go left
    void on_Right_Button_pressed(); // Function to go right
    void on_LeftForward_Button_pressed(); // Function to go left-forward angle
    void on_RightForward_Button_pressed(); // Function to go right-forward angle
    void on_LeftBackward_Button_pressed(); // Function to go left-backward angle
    void on_RightBackward_Button_pressed(); // Function to go right-backward angle
    void on_LeftRotate_Button_pressed(); // Function to rotate left
    void on_Stop_Button_pressed(); // Function to stop the vehicle
    void on_RightRotate_Button_pressed(); // Function to rotate right

    void on_progressbar_change(); // Funciton for adjust the speed of the vehicle
    void on_GripperSlider_valueChanged(int value); // Function to control the gripper
    void on_UpButton_clicked(); // Function to move the linear guide up
    void on_DownButton_clicked(); // Function to move the linear guide down
    void on_StartRecordBut_clicked(); // Function to start the recording
    void on_StopRecordBut_clicked(); // Function to stop the recording
    void on_DeleteRecordBut_clicked(); // Function to delete the rocording
    void on_PlayBackBut_clicked(); // Function to playback the recorded movements
    void onGripperTimerTimeout(); // Timing function to take the last gripper value
    void on_SaveRecordBut_clicked(); // Funtion to save the recorded dat to a file
    void on_PlayBackSavedBut_clicked(); // Funciton to play back from saved file

private:
    Ui::DriveMode *ui;
    MotorControl* motor_control_;
    double top_speed_ms =0.1; // Defining the top speed in m/s
    double initial_speed_ms = 0.001; // Definingn the initial speed in m/s
    double speed_ms = initial_speed_ms; // Assign the initial speed as normal speed in m/s
    double Null =0; // Assigning empty value
    bool record_flag = false; // Flag to control the recording
    QTimer *gripperTimer; // Timer for the gripperdata
};

#endif // DRIVEMODE_H
