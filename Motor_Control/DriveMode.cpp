#include "DriveMode.h"
#include "ui_DriveMode.h"

DriveMode::DriveMode(MotorControl* a,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DriveMode),
    motor_control_(a)
{
    ui->setupUi(this);
    QDialog::setWindowTitle("Moving Vehicle");
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->progressBar,SLOT(setValue(int)));
    connect(ui->progressBar, &QProgressBar::valueChanged, this, &DriveMode::on_progressbar_change);
    gripperTimer = new QTimer(this);
    connect(gripperTimer, SIGNAL(timeout()), this, SLOT(onGripperTimerTimeout()));
}

DriveMode::~DriveMode()
{
    delete ui;
}

// Moving Forward
void DriveMode::on_Forward_Button_pressed()
{
    motor_control_->movement_start_time_ = std::chrono::steady_clock::now(); // counting time
    motor_control_->mecanum_Drive(speed_ms,Null,Null, motor_control_->LeftFront, motor_control_->RightFront, motor_control_->LeftBack,  motor_control_->RightBack);
    motor_control_->send_drive_data();
    if(record_flag == true){
        motor_control_->record_current_movement(speed_ms,Null,Null,0.0,1,0,false,false,0);
    }
}

// Moving Backward
void DriveMode::on_Backward_Button_pressed()
{

    // Calculate the time elapsed since the movement started
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
    double elapsed_time = elapsed_seconds.count();

    motor_control_->movement_start_time_ = current_time;

    motor_control_->mecanum_Drive(-speed_ms,Null,Null, motor_control_->LeftFront, motor_control_->RightFront, motor_control_->LeftBack,  motor_control_->RightBack);
    motor_control_->send_drive_data();
    if(record_flag == true){
        motor_control_->record_current_movement(-speed_ms,Null,Null, elapsed_time,1,0,false,false,0);
    }
}

// Moving Left
void DriveMode::on_Left_Button_pressed()
{
    // Calculate the time elapsed since the movement started
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
    double elapsed_time = elapsed_seconds.count();

    motor_control_->movement_start_time_ = current_time;

    motor_control_->mecanum_Drive(Null,-speed_ms,Null, motor_control_->LeftFront, motor_control_->RightFront, motor_control_->LeftBack,  motor_control_->RightBack);
    motor_control_->send_drive_data();
    if(record_flag == true){
        motor_control_->record_current_movement(Null,-speed_ms,Null, elapsed_time,1,0,false,false,0);
    }
}

// Moving Right
void DriveMode::on_Right_Button_pressed()
{
    // Calculate the time elapsed since the movement started
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
    double elapsed_time = elapsed_seconds.count();

    motor_control_->movement_start_time_ = current_time;

    motor_control_->mecanum_Drive(Null,speed_ms,Null, motor_control_->LeftFront, motor_control_->RightFront, motor_control_->LeftBack,  motor_control_->RightBack);
    motor_control_->send_drive_data();
    if(record_flag == true){
        motor_control_->record_current_movement(Null,speed_ms,Null, elapsed_time,1,0,false,false,0);
    }
}

// Moving LeftForward Angle
void DriveMode::on_LeftForward_Button_pressed()
{
    // Calculate the time elapsed since the movement started
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
    double elapsed_time = elapsed_seconds.count();

    motor_control_->movement_start_time_ = current_time;

    motor_control_->mecanum_Drive(speed_ms,speed_ms,Null, motor_control_->LeftFront, motor_control_->RightFront, motor_control_->LeftBack,  motor_control_->RightBack);
    motor_control_->send_drive_data();
    if(record_flag == true){
        motor_control_->record_current_movement(speed_ms,speed_ms,Null, elapsed_time,1,0,false,false,0);
    }
}

// Moving RightForward Angle
void DriveMode::on_RightForward_Button_pressed()
{
    // Calculate the time elapsed since the movement started
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
    double elapsed_time = elapsed_seconds.count();

    motor_control_->movement_start_time_ = current_time;

    motor_control_->mecanum_Drive(speed_ms,-speed_ms,Null, motor_control_->LeftFront, motor_control_->RightFront, motor_control_->LeftBack,  motor_control_->RightBack);
    motor_control_->send_drive_data();
    if(record_flag == true){
        motor_control_->record_current_movement(speed_ms,-speed_ms,Null, elapsed_time,1,0,false,false,0);
    }
}

// Moving LeftBackward Angle
void DriveMode::on_LeftBackward_Button_pressed()
{
    // Calculate the time elapsed since the movement started
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
    double elapsed_time = elapsed_seconds.count();

    motor_control_->movement_start_time_ = current_time;

    motor_control_->mecanum_Drive(-speed_ms,speed_ms,Null, motor_control_->LeftFront, motor_control_->RightFront, motor_control_->LeftBack,  motor_control_->RightBack);
    motor_control_->send_drive_data();
    if(record_flag == true){
        motor_control_->record_current_movement(-speed_ms,speed_ms,Null, elapsed_time,1,0,false,false,0);
    }
}

// Moving RightBackward Angle
void DriveMode::on_RightBackward_Button_pressed()
{
    // Calculate the time elapsed since the movement started
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
    double elapsed_time = elapsed_seconds.count();

    motor_control_->movement_start_time_ = current_time;

    motor_control_->mecanum_Drive(-speed_ms,-speed_ms,Null, motor_control_->LeftFront, motor_control_->RightFront, motor_control_->LeftBack,  motor_control_->RightBack);
    motor_control_->send_drive_data();
    if(record_flag == true){
        motor_control_->record_current_movement(-speed_ms,-speed_ms,Null, elapsed_time,1,0,false,false,0);
    }
}

// Rotate Left
void DriveMode::on_LeftRotate_Button_pressed()
{
    // Calculate the time elapsed since the movement started
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
    double elapsed_time = elapsed_seconds.count();

    motor_control_->movement_start_time_ = current_time;

    motor_control_->mecanum_Drive(Null, Null, speed_ms, motor_control_->LeftFront, motor_control_->RightFront, motor_control_->LeftBack,  motor_control_->RightBack);
    motor_control_->send_drive_data();
    if(record_flag == true){
        motor_control_->record_current_movement(Null, Null, speed_ms, elapsed_time,1,0,false,false,0);
    }
}

// Rotate Right
void DriveMode::on_RightRotate_Button_pressed()
{
    // Calculate the time elapsed since the movement started
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
    double elapsed_time = elapsed_seconds.count();

    motor_control_->movement_start_time_ = current_time;

    motor_control_->mecanum_Drive(Null, Null, -speed_ms, motor_control_->LeftFront, motor_control_->RightFront, motor_control_->LeftBack,  motor_control_->RightBack);
    motor_control_->send_drive_data();
    if(record_flag == true){
        motor_control_->record_current_movement(Null, Null, -speed_ms, elapsed_time,1,0,false,false,0);
    }
}

// Stop the vehicle
void DriveMode::on_Stop_Button_pressed()
{
    // Calculate the time elapsed since the movement started
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
    double elapsed_time = elapsed_seconds.count();

    motor_control_->movement_start_time_ = current_time;

    motor_control_->mecanum_Drive(Null, Null, Null, motor_control_->LeftFront, motor_control_->RightFront, motor_control_->LeftBack,  motor_control_->RightBack);
    motor_control_->send_drive_data();
    if(record_flag == true){
        motor_control_->record_current_movement(Null, Null, Null, elapsed_time,1,0,false,false,0);
    }
}

// Changing the speed
void DriveMode::on_progressbar_change()
{
    int percentage = ui->progressBar->value();
    speed_ms = ((double)percentage/100)*(top_speed_ms-initial_speed_ms)+initial_speed_ms;
}


void DriveMode::on_GripperSlider_valueChanged(int value)
{  
    gripperTimer->start(100);  // Start the timer
}

// Function to move the linear guide to up direction
void DriveMode::on_UpButton_clicked()
{

    int up_value = 1;
    motor_control_->send_linear_guide_data(true,false,up_value);
    if (record_flag == true) {
        // Calculate the time elapsed since the movement started
        std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
        double elapsed_time = elapsed_seconds.count();
        motor_control_->movement_start_time_ = current_time;
        motor_control_->record_current_movement(Null, Null, Null, elapsed_time,3,0,true,false,up_value);
    }
}

// Funciton to move the linear guide to move down direction
void DriveMode::on_DownButton_clicked()
{


    int down_value = 1;
    motor_control_->send_linear_guide_data(false,true,down_value);
    if (record_flag == true) {

        // Calculate the time elapsed since the movement started
        std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
        double elapsed_time = elapsed_seconds.count();
        motor_control_->movement_start_time_ = current_time;
        motor_control_->record_current_movement(Null, Null, Null, elapsed_time,3,0,false,true,down_value);
    }
}

// Function to start the recording
void DriveMode::on_StartRecordBut_clicked()
{
    record_flag = true;
}

// Function to stop the recording
void DriveMode::on_StopRecordBut_clicked()
{
    record_flag = false;
}

// Function to delete the recording
void DriveMode::on_DeleteRecordBut_clicked()
{
    motor_control_->recorded_movements.clear();
}


// Function to play the recorded movements
void DriveMode::on_PlayBackBut_clicked()
{
    motor_control_->playback_recorded_movements();
}

// Function to take the gripper value when the timer starts
void DriveMode::onGripperTimerTimeout()
{
    gripperTimer->stop();

    // Now, you can send the gripper value
    int gripper_value = ui->GripperSlider->value();
    motor_control_->send_gripper_data(gripper_value);

    // Add your record logic if needed
    if (record_flag == true) {
        // Calculate the time elapsed since the movement started
        std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = current_time - motor_control_->movement_start_time_;
        double elapsed_time = elapsed_seconds.count();
        motor_control_->movement_start_time_ = current_time;
        motor_control_->record_current_movement(Null, Null, Null, elapsed_time, 2, gripper_value, false, false, 0);
    }
}

// Function to save the recorded data
void DriveMode::on_SaveRecordBut_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Recorded Movements", "", "Data Files (*.dat)");
        if (!filename.isEmpty()) {
            motor_control_->save_recorded_vector(motor_control_->recorded_movements, filename);
        }
}

// Function to play back from saved data
void DriveMode::on_PlayBackSavedBut_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Recorded Movements", "", "Data Files (*.dat)");
       if (!filename.isEmpty()) {
           motor_control_->playback_recorded_vector(filename);
       }
}
