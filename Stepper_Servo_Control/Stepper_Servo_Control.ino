#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>  
#include <iostream>
#include <sstream>
#include <AccelStepper.h>
#include "ReadWriteXml.h"

#define WIFI_NETWORK "MyNetwork"
#define WIFI_PASSWORD "abcd1234"
#define WIFI_TIMEOUT_MS 20000

const char* serverIP = "192.168.100.23";  // this is my server IP
const int serverPort = 52001;

WiFiClient client;
ReadWriteXml myxml;
Servo servo;
const int servoPin = 26;
const int stepPin = 33;
const int dirPin = 27;
const int enablePin = 12;
const int initialPosition = 120;
const int upswitch = 34;
const int downswitch = 35;
// Set the number of steps to rotate
long steps = 100;

// Upper and lower limit state
int upperlimit = 0;
int lowerlimit = 0;

// Create an instance of AccelStepper for the stepper motor
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void connectToWifi() {
  Serial.print("Connecting to Wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  unsigned long startAttempTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttempTime < WIFI_TIMEOUT_MS) {
    Serial.print(".");
    delay(100);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect...");
  } else {
    Serial.print("Connected to:");
    Serial.println(WiFi.localIP());
  }
}

void connectToServer() {

  if (!client.connect(serverIP, serverPort)) {
    Serial.println("Connection failed!");
    return;
  }

  Serial.println("Connected to server!");
  // Wait for the server to send data
  while (client.connected() && !client.available()) {
    delay(100);
  }
}

void stepperMoveUP() {
  digitalWrite(dirPin, HIGH);  //Move in particular direction
  for (int x = 0; x < 100; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
}

void stepperMoveDown() {
  digitalWrite(dirPin, LOW);  // changes the direction
  for (int x = 0; x < 100; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
}

void setup() {
  Serial.begin(115200);

  servo.attach(servoPin);
  servo.write(initialPosition);

  // Set the pin modes for the step, dir, and enable pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(upswitch, INPUT);
  pinMode(downswitch, INPUT);

  // Enable the stepper motor driver
  digitalWrite(enablePin, LOW);

  // Set the initial speed and acceleration for the stepper motor
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);

  connectToWifi();
}

void loop() {
  // put your main code here, to run repeatedly:
  connectToServer();
  // Send data to the server
  // String message = "Hello, server!"; // Replace this with your desired message
  // client.print(message);

  // Wait for the server to respond (optional)
  while (client.available()) {
    String response = client.readStringUntil('\n');
    // Serial.print("Received from server: ");
    // Serial.println(response);

    bool A = myxml.containsSpecificString(response, "GripperValue");
    bool B = myxml.containsSpecificString(response, "UpValue");
    bool C = myxml.containsSpecificString(response, "DownValue");
    if (A) {
      int servoPos = myxml.XmlGetInt(response, "GripperValue");
      servo.write(servoPos);
      Serial.println(servoPos);
    }
    if (B) {
      if (upperlimit != HIGH) {
        stepperMoveDown();
        Serial.println("Stepper going UP");
      }
    }
    if (C) {
      if (lowerlimit != HIGH) {
        stepperMoveUP();
        Serial.println("Stepper going Down");
      }
    }
  }

  //Knowing the current state of upper and lower limit
  upperlimit = digitalRead(upswitch);
  lowerlimit = digitalRead(downswitch);

  // Run the stepper motor
  stepper.run();

  // Close the connection
  // client.stop();

  // Wait for a moment before repeating
  // delay(10);
}
