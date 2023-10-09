#include "JoyStickMode.h"
#include "ui_JoyStickMode.h"

JoyStickMode::JoyStickMode(MotorControl* b, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoyStickMode),
    new_motor_control(b)
{
    ui->setupUi(this);
    QDialog::setWindowTitle("JoyStick Control");
    setFixedSize(500, 500);
    m_center = QPoint(width() / 2, height() / 2);
    m_radius = qMin(width(), height()) / 3;
    m_stickRadius = m_radius / 3;
    m_isPressed = false;
    m_currentPos = QPointF(0, 0);

    m_directionLabel = new QLabel(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_directionLabel, 0, Qt::AlignBottom | Qt::AlignRight);
    setLayout(layout);

    // Create the rotation slider
    rotationSlider = new QSlider(Qt::Horizontal, this);
    rotationSlider->setRange(-180, 180);
    rotationSlider->setValue(0);
    rotationSlider->setFixedSize(200,30);
    //rotationSlider->setContentsMargins(100,0,0,0);

    // Create the stop button
    stopButton = new QPushButton("Stop", this);
    stopButton->setFixedSize(100,30);
    //stopButton->setContentsMargins(200,0,0,0);

    //Create Reset Button
    resetSlider = new QPushButton("Reset-Slider",this);
    resetSlider->setFixedSize(200,30);

    // Connect signals and slots
    connect(rotationSlider, &QSlider::valueChanged, this, &JoyStickMode::handleRotationSliderValueChanged);
    connect(stopButton, &QPushButton::clicked, this, &JoyStickMode::stopButtonClicked);
    connect(resetSlider,&QPushButton::clicked,this,&JoyStickMode::resetSliderClicked);

    // Add the slider and button to the layout
    QVBoxLayout* layout2 = qobject_cast<QVBoxLayout*>(this->layout());
    layout->addWidget(rotationSlider);
    layout->addWidget(stopButton);
    layout->addWidget(resetSlider);

    //layout->setContentsMargins(150,50,0,0);

}

JoyStickMode::~JoyStickMode()
{
    delete ui;
}

void JoyStickMode::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw outer circle
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::cyan);
    painter.drawEllipse(m_center, m_radius, m_radius);

    // draw inner circle
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::red);
    QPoint stickPos(m_center.x() + m_currentPos.x() * m_radius, m_center.y() + m_currentPos.y() * m_radius);
    painter.drawEllipse(stickPos, m_stickRadius, m_stickRadius);
}

void JoyStickMode::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        m_currentPos = QPointF(0, 0);
        update();
    }
}

void JoyStickMode::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed) {
        QPointF pos = event->pos() - m_center;
        qreal distance = QLineF(m_center, event->pos()).length();
        if (distance > m_radius) {
            pos *= m_radius / distance;
            distance = m_radius;
        }
        m_currentPos = pos / m_radius;
        update();
        emit directionChanged(m_currentPos);
    }
}

void JoyStickMode::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPressed = false;
        m_currentPos = QPointF(0, 0);
        update();
        emit directionChanged(m_currentPos);
    }
}

void JoyStickMode::moveForward()
{
    new_motor_control->mecanum_Drive(speed_ms,Null,Null, new_motor_control->LeftFront, new_motor_control->RightFront, new_motor_control->LeftBack,  new_motor_control->RightBack);
    new_motor_control->send_drive_data();
}

void JoyStickMode::moveBackward()
{
    new_motor_control->mecanum_Drive(-speed_ms,Null,Null, new_motor_control->LeftFront, new_motor_control->RightFront, new_motor_control->LeftBack,  new_motor_control->RightBack);
    new_motor_control->send_drive_data();
}

void JoyStickMode::moveLeft()
{
    new_motor_control->mecanum_Drive(Null,-speed_ms,Null, new_motor_control->LeftFront, new_motor_control->RightFront, new_motor_control->LeftBack,  new_motor_control->RightBack);
    new_motor_control->send_drive_data();
}

void JoyStickMode::moveRight()
{
    new_motor_control->mecanum_Drive(Null,speed_ms,Null, new_motor_control->LeftFront, new_motor_control->RightFront, new_motor_control->LeftBack,  new_motor_control->RightBack);
    new_motor_control->send_drive_data();
}

void JoyStickMode::moveLeftForward()
{
    new_motor_control->mecanum_Drive(speed_ms,speed_ms,Null,new_motor_control->LeftFront, new_motor_control->RightFront, new_motor_control->LeftBack,  new_motor_control->RightBack);
    new_motor_control->send_drive_data();
}

void JoyStickMode::moveRightForward()
{
    new_motor_control->mecanum_Drive(speed_ms,-speed_ms,Null,new_motor_control->LeftFront, new_motor_control->RightFront, new_motor_control->LeftBack,  new_motor_control->RightBack);
    new_motor_control->send_drive_data();
}

void JoyStickMode::moveLeftBackward()
{
    new_motor_control->mecanum_Drive(-speed_ms,speed_ms,Null,new_motor_control->LeftFront, new_motor_control->RightFront, new_motor_control->LeftBack,  new_motor_control->RightBack);
    new_motor_control->send_drive_data();
}

void JoyStickMode::moveRightBackward()
{
    new_motor_control->mecanum_Drive(-speed_ms,-speed_ms,Null,new_motor_control->LeftFront, new_motor_control->RightFront, new_motor_control->LeftBack,  new_motor_control->RightBack);
    new_motor_control->send_drive_data();
}

void JoyStickMode::stopVehicle()
{
    new_motor_control->mecanum_Drive(Null, Null, Null,new_motor_control->LeftFront, new_motor_control->RightFront, new_motor_control->LeftBack,  new_motor_control->RightBack);
    new_motor_control->send_drive_data();
}

void JoyStickMode::directionChanged(QPointF direction)
{
    double x = direction.x();
    double y = direction.y();
    int decimalPlaces = 3; // Adjust the number of decimal places as needed

    QString text = QString("Direction: (%1, %2)").arg(QString::number(x, 'f', decimalPlaces)).arg(QString::number(y, 'f', decimalPlaces));
    m_directionLabel->setText(text);

    // m_directionLabel->setText(QString("Dir: (%1, %2)").arg(direction.x()).arg(direction.y()));
    m_directionLabel->setStyleSheet("QLabel {background-color : lightGray;}");

}

void JoyStickMode::handleRotationSliderValueChanged(int value)
{

}

void JoyStickMode::stopButtonClicked()
{

}

void JoyStickMode::resetSliderClicked()
{

}
