#include "MotorControl.h"
#include "ui_MotorControl.h"
#include "ReadWriteXml.h"
#include "DriveMode.h"
#include <QMessageBox>
#include "JoyStickMode.h"
#include <QThread>
#include <QTimer>
#include <QDataStream>
#include <QFile>
#include <fstream>

MotorControl::MotorControl(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MotorControl)
{
    ui->setupUi(this);

    bool ok = motor_control.bind(QHostAddress::Any,52001);
    QObject::connect(&motor_control,SIGNAL(connected()),this,SLOT(OnConnected()));
    QObject::connect(&motor_control,SIGNAL(disconnected()),this,SLOT(OnDisconnected()));
    QObject::connect(&motor_control,SIGNAL(readyRead()),this,SLOT(OnDataReceived()));
    QObject::connect(&motor_control,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(OnError(QAbstractSocket::SocketError)));
}

MotorControl::~MotorControl()
{
    delete ui;
}

// Function to calculate the value for motor in micro_step/s
void MotorControl::mecanum_Drive(double Vx_ms, double Vy_ms, double theta_ms, double &Lf_micro_step_s, double &Rf_micro_step_s, double &Lb_micro_step_s, double &Rb_micro_step_s)
{
    /* First Equation type */
    double A = Vx_ms+Vy_ms;
    double B = Vx_ms-Vy_ms;
    double C = theta_ms*(L_m+l_m);

    /* Convert to μstep/s */
    double w1_rad_s, w2_rad_s, w3_rad_s, w4_rad_s;

    w1_rad_s = (B-C)/R_m;
    w2_rad_s = -(A+C)/R_m;
    w3_rad_s = (A-C)/R_m;
    w4_rad_s = -(B+C)/R_m;

    Lf_micro_step_s = (w1_rad_s/2*Pi)*200*10;
    Rf_micro_step_s = (w2_rad_s/2*Pi)*200*10;
    Lb_micro_step_s = (w3_rad_s/2*Pi)*200*10;
    Rb_micro_step_s = (w4_rad_s/2*Pi)*200*10;
}

// To send data to move the motor in a specific driving mode
void MotorControl::send_drive_data()
{
    QString TextStream;
    ReadWriteXml writexml;
    TextStream = writexml.WriteXml("DriveMode","LeftFront",LeftFront,"RightFront",RightFront,"LeftBack",LeftBack,"RightBack",RightBack);
    motor_control.write(TextStream.toLatin1());
    //qDebug() << TextStream;
}

// To send data to move the motor in a specific position
void MotorControl::send_pos_data()
{
    QString TextStream;
    ReadWriteXml writexml;
    TextStream = writexml.WriteXml("PositionMode","LeftFront",LeftFront,"RightFront",RightFront,"LeftBack",LeftBack,"RightBack",RightBack);
    motor_control.write(TextStream.toLatin1());
    qDebug() << TextStream;
}
// To send the recorded movement to the server
void MotorControl::send_recorded_data(RecordedMovement &recorded_movement)
{
    QString TextStream;
    ReadWriteXml writexml;
    TextStream = writexml.WriteXml("DriveMode",
                                   "LeftFront", recorded_movement.leftFrontRec,
                                   "RightFront", recorded_movement.rightFrontRec,
                                   "LeftBack", recorded_movement.leftBackRec,
                                   "RightBack", recorded_movement.rightBackRec);
    motor_control.write(TextStream.toLatin1());
    qDebug() << TextStream;
}

// To save the vectors to a file
void MotorControl::save_recorded_vector(const QVector<RecordedMovement>& recorded_movements, const QString &filename)
{
    QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Failed to open file for writing:" << filename;
            return;
        }

        QDataStream out(&file);
        out << recorded_movements.size();

        for (const RecordedMovement& movement : recorded_movements) {
            out << movement.leftFrontRec;
            out << movement.rightFrontRec;
            out << movement.leftBackRec;
            out << movement.rightBackRec;
            out << movement.Vx_ms_rec;
            out << movement.Vy_ms_rec;
            out << movement.theta_rec;
            out << movement.elapsed_time;
            out << movement.movement_type;
            out << movement.gripper_value;
            out << movement.linearGuideDown;
            out << movement.linearGuideUp;
            out << movement.Value;
        }

        file.close();
}

// Playing back from the saved file...
void MotorControl::playback_recorded_vector(const QString &filename)
{
    QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file for reading:" << filename;
            return;
        }

        QDataStream in(&file);
        recorded_movements.clear();

        size_t vector_size;
        in >> vector_size;
        recorded_movements.resize(vector_size);

        for (RecordedMovement& movement : recorded_movements) {
            //in >> movement;
            in >> movement.leftFrontRec;
            in >> movement.rightFrontRec;
            in >> movement.leftBackRec;
            in >> movement.rightBackRec;
            in >> movement.Vx_ms_rec;
            in >> movement.Vy_ms_rec;
            in >> movement.theta_rec;
            in >> movement.elapsed_time;
            in >> movement.movement_type;
            in >> movement.gripper_value;
            in >> movement.linearGuideDown;
            in >> movement.linearGuideUp;
            in >> movement.Value;
        }

        file.close();

        playback_recorded_movements();
}

// Estimate the movement distance
double MotorControl::estimate_movement_distance(double speed_ms, double time_sec)
{
    double distance = speed_ms*time_sec;
    return distance;
}

// To send the grippper data to the server
void MotorControl::send_gripper_data( int gripper_value )
{
    QString TextStream;
    ReadWriteXml writexml;
    TextStream = writexml.XmlCreateString("GripperValue", gripper_value);
    motor_control.write(TextStream.toLatin1());
    qDebug() << TextStream;
}

// To send data for linear guide to the server
void MotorControl::send_linear_guide_data(bool up, bool down, int value)
{
    QString TextStream;
    ReadWriteXml writexml;
    if(up){
        TextStream = writexml.XmlCreateString("UpValue", value);
    }
    else if(down){
        TextStream = writexml.XmlCreateString("DownValue", value);
    }
    motor_control.write(TextStream.toLatin1());
    qDebug() << TextStream;
}

void MotorControl::move_to_pos(double target_X, double target_Y)
{
    const double error_margin = 0.1; // Error margin to stop the rover as required

    // Assuming the initial position as (0,0)
    double initial_pos_x = current_pos_x;
    double initial_pos_y = current_pos_y;
    // Calculate the distance and heading angle between the initial position and the target position
    double dist = std::sqrt(std::pow((target_X - initial_pos_x), 2) + std::pow((target_Y - initial_pos_y), 2));
    double theta = std::atan2((target_Y - initial_pos_y), (target_X - initial_pos_x));

    // Move the rover towards the target position
    while (dist >= error_margin) {
        // set the spped and time
        double movement_speed =0.1;
        double movement_time = 0.1;
        double movement_distance = estimate_movement_distance(movement_speed,movement_time);

        // Move the rover towards the target position
        mecanum_Drive(std::cos(theta), std::sin(theta), 0.0, LeftFront, RightFront, LeftBack, RightBack);
        send_pos_data();

        // Delay for a few milliseconds to allow the rover to move
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // Update the current position based on the estimated movement distance and angle
        current_pos_x += movement_distance * std::cos(theta);
        current_pos_y += movement_distance * std::sin(theta);

        // Recalculate the distance and angle after each movement
        dist = std::sqrt(std::pow((target_X - current_pos_x), 2) + std::pow((target_Y - current_pos_y), 2));
        theta = std::atan2((target_Y - current_pos_y), (target_X - current_pos_x));
    }
    // Stop the rover as it reached the target position
    mecanum_Drive(0.0, 0.0, 0.0, LeftFront, RightFront,LeftBack, RightBack);
    send_pos_data();
}

// Function to record current movement of the vehicle
void MotorControl::record_current_movement(double Vx_ms_rec, double Vy_ms_rec, double theta_rec, double elapsed_time, int movement_type, int gripper_value, bool linearGuideDown, bool linearGuideUp, int Value)
{
    RecordedMovement recorded_movement;
    recorded_movement.leftFrontRec = LeftFront;
    recorded_movement.rightFrontRec = RightFront;
    recorded_movement.leftBackRec = LeftBack;
    recorded_movement.rightBackRec = RightBack;
    recorded_movement.Vx_ms_rec = Vx_ms_rec;
    recorded_movement.Vy_ms_rec = Vy_ms_rec;
    recorded_movement.theta_rec = theta_rec;
    recorded_movement.elapsed_time=elapsed_time;
    recorded_movement.movement_type= movement_type;
    recorded_movement.gripper_value = gripper_value;
    recorded_movement.linearGuideDown = linearGuideDown;
    recorded_movement.linearGuideUp = linearGuideUp;
    recorded_movement.Value = Value;
    recorded_movements.push_back(recorded_movement);
}

void MotorControl::playback_recorded_movements()
{

    QString TextStream;
    ReadWriteXml writexml;
    for (RecordedMovement& recorded_movement : recorded_movements) {
        if(recorded_movement.movement_type == 1)
        {
            // Send the stored motor commands for playback
            mecanum_Drive(
                        recorded_movement.Vx_ms_rec,
                        recorded_movement.Vy_ms_rec,
                        recorded_movement.theta_rec,
                        recorded_movement.leftFrontRec,
                        recorded_movement.rightFrontRec,
                        recorded_movement.leftBackRec,
                        recorded_movement.rightBackRec
                        );
            // delay between different movements
            QThread::msleep(static_cast<unsigned long>(recorded_movement.elapsed_time * 1000));

            // Prepare and send recorded data to the server
            TextStream = writexml.WriteXml("DriveMode",
                                           "LeftFront", recorded_movement.leftFrontRec,
                                           "RightFront", recorded_movement.rightFrontRec,
                                           "LeftBack", recorded_movement.leftBackRec,
                                           "RightBack", recorded_movement.rightBackRec
                                           );
            motor_control.write(TextStream.toLatin1());
            motor_control.flush(); // TO ensure the message is sent immediately
            qDebug() << TextStream;
        }


        // Send gripper data if it's recorded for this movement
        else if (recorded_movement.movement_type == 2) {
            // delay between different movements
            QThread::msleep(static_cast<unsigned long>(recorded_movement.elapsed_time * 1000));
            send_gripper_data(recorded_movement.gripper_value);
            motor_control.flush();
        }

        // Send linear guide data if it's recorded for this movement
        else if (recorded_movement.movement_type == 3) {
            //delay between different movements
            QThread::msleep(static_cast<unsigned long>(recorded_movement.elapsed_time * 1000));
            send_linear_guide_data(recorded_movement.linearGuideDown, recorded_movement.linearGuideUp, recorded_movement.Value);
            motor_control.flush();
        }
    }
}

// Function to establish the connection
void MotorControl::on_qConnectButton_clicked()
{
    motor_control.connectToHost(ui->qServerIpLine->text(),ui->qServerPortSpin->value());
    ui->statusbar->showMessage("Attempting Connection");
    ui->statusbar->setStyleSheet("QStatusBar {background:yellow; font:bold}");
}

// Function to close the connection
void MotorControl::on_qDisconnectButton_clicked()
{
    motor_control.close();
}


void MotorControl::on_qDirectModeButton_clicked()
{
    int motor_id, cmd, type, value;
    QString TextStream;
    motor_id = ui->qMotorIdSpin->value();
    cmd = ui->qInstructionSpin->value();
    type =ui->qTypeSpin->value();
    value =ui->qValueSpin->value();
    ReadWriteXml writexml;
    TextStream = writexml.WriteXml("DirectMode","MotorId",motor_id,"Command",cmd,"Type",type,"Value",value);
    motor_control.write(TextStream.toLatin1());
}

// Function to go to specific position
void MotorControl::on_qTargetPositionButton_clicked()
{

    double target_x = ui->qPositionLineEditX->text().toDouble();
    double target_y = ui->qPositionLineEditY->text().toDouble();

    move_to_pos(target_x,target_y);
}


void MotorControl::on_qTargetCurrentButton_clicked()
{
    int target_current = ui->qTargetCurrentSpin->value();
    QString txt = QString("Torque Value=%1").arg(target_current);
    motor_control.write(txt.toLatin1());
}

// Function to handle when the connection with server established
void MotorControl::OnConnected()
{
    //ui->qConnectionGroup->setEnabled(false);
    ui->qConnectButton->setEnabled(false);
    ui->qServerIpLine->setEnabled(false);
    ui->qServerIP->setEnabled(false);
    ui->qServerPort->setEnabled(false);
    ui->qServerPortSpin->setEnabled(false);
    ui->qMotorDataGroup->setEnabled(true);
    ui->statusbar->showMessage("Connected");
    ui->statusbar->setStyleSheet("QStatusBar {background:green;font:bold}");
}

// Function to handle when the server disconnected
void MotorControl::OnDisconnected()
{
    motor_control.close();
    ui->qConnectButton->setEnabled(true);
    ui->qServerIpLine->setEnabled(true);
    ui->qServerIP->setEnabled(true);
    ui->qServerPort->setEnabled(true);
    ui->qServerPortSpin->setEnabled(true);
    ui->qMotorDataGroup->setEnabled(false);
    ui->statusbar->showMessage("Disconnected");
    ui->statusbar->setStyleSheet("QStatusBar {background:orange;font:bold}");
}

// Function to handle error on conncetion
void MotorControl::OnError(QAbstractSocket::SocketError error)
{
    ui->qConnectionGroup->setEnabled(true);
    ui->qMotorDataGroup->setEnabled(false);
    ui->statusbar->showMessage("Error: "+motor_control.errorString());
    ui->statusbar->setStyleSheet("QStatusBar {background:red;font:bold}");
}

void MotorControl::OnDataReceived()
{

}


void MotorControl::on_qTargetVelocityButton_clicked()
{
    int velocity = ui->qTargetVelocitySpin->value();
    QString txt = QString("Target Velocity=%1").arg(velocity);
    motor_control.write(txt.toLatin1());
}

// Function to move the vehicle to a specific data
void MotorControl::on_qMovingModeButton_clicked()
{
    double Vx_mm_s, Vy_mm_s, theta_rad_s;
    double LeftFront, RightFront, LeftBack, RightBack;
    QString TextStream;
    Vx_mm_s = ui->qVerticalVxSpin->value();
    Vy_mm_s = ui->qHorizontalVySpin->value();
    theta_rad_s = ui->qRotationThetaSpin->value();
    mecanum_Drive(Vx_mm_s,Vy_mm_s,theta_rad_s,LeftFront,RightFront,LeftBack,RightBack);

    ReadWriteXml writexml;
    TextStream = writexml.WriteXml("MovingMode","LeftFront",LeftFront,"RightFront",RightFront,"LeftBack",LeftBack,"RightBack",RightBack);
    motor_control.write(TextStream.toLatin1());
}
// Function to reset the motor if needed
void MotorControl::on_qResetButton_clicked()
{
    QString TextStream = "</Reset>";
    motor_control.write(TextStream.toLatin1());
}


// Function to open the Driving mode window
void MotorControl::on_qDirectDriveButton_clicked()
{
    DriveMode newwidget(this);
    //newwidget.setWindowIconText("New Window");
    newwidget.show();
    newwidget.exec();
}

// Function to open the joystick window
void MotorControl::on_qJoyStickButton_clicked()
{
    JoyStickMode JoyStickWidget(this);
    JoyStickWidget.show();
    JoyStickWidget.exec();
}


