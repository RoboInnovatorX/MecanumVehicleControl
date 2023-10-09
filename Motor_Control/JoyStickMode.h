#ifndef JOYSTICKMODE_H
#define JOYSTICKMODE_H

#include <QDialog>
#include "MotorControl.h"
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QSlider>
#include <QPushButton>

namespace Ui {
class JoyStickMode;
}

class JoyStickMode : public QDialog
{
    Q_OBJECT

public:
    explicit JoyStickMode(MotorControl* b, QWidget *parent = nullptr);
    ~JoyStickMode();

private:
    Ui::JoyStickMode *ui;               // Pointer to the UI components for the JoyStick mode
    MotorControl* new_motor_control;    // Pointer to a MotorControl instance
    QWidget *m_centralWidget;           // Pointer to the central widget
    QPoint m_center;                    // Center point of the joystick
    int m_radius;                       // Radius of the joystick's circular area
    int m_stickRadius;                  // Radius of the joystick stick
    bool m_isPressed;                   // Flag indicating if the joystick is pressed
    QPointF m_currentPos;               // Current position of the joystick
    QLabel *m_directionLabel;           // Pointer to a label showing the joystick direction
    QSlider* rotationSlider;            // Pointer to a slider for rotation control
    QPushButton* stopButton;            // Pointer to a button for stopping the vehicle
    QPushButton* resetSlider;           // Pointer to a button for resetting the slider
    double speed_ms = 0.005;            // Linear speed in meters per second
    double Null = 0;                    // Null value (possibly a default value)

    void paintEvent(QPaintEvent *event) override;  // Function for handling paint events
    void mousePressEvent(QMouseEvent *event) override;  // Function for handling mouse press events
    void mouseMoveEvent(QMouseEvent *event) override;   // Function for handling mouse move events
    void mouseReleaseEvent(QMouseEvent *event) override; // Function for handling mouse release events

    void moveForward();                 // Function for moving the vehicle forward
    void moveBackward();                // Function for moving the vehicle backward
    void moveLeft();                    // Function for moving the vehicle left
    void moveRight();                   // Function for moving the vehicle right
    void moveLeftForward();             // Function for moving the vehicle diagonally left-forward
    void moveRightForward();            // Function for moving the vehicle diagonally right-forward
    void moveLeftBackward();            // Function for moving the vehicle diagonally left-backward
    void moveRightBackward();           // Function for moving the vehicle diagonally right-backward
    void stopVehicle();                 // Function for stopping the vehicle

public slots:
    void directionChanged(QPointF direction);    // Slot for handling changes in direction
    void handleRotationSliderValueChanged(int value);   // Slot for handling rotation slider changes
    void stopButtonClicked();           // Slot for handling stop button clicks
    void resetSliderClicked();          // Slot for handling reset slider button clicks

};

#endif // JOYSTICKMODE_H
