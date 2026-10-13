
#include <Arduino.h>
#include <SoftwareSerial.h>

// =====================================================
// HC-05 BLUETOOTH
// =====================================================

// Arduino D11 = RX <- HC-05 TX
// Arduino D12 = TX -> HC-05 RX through voltage divider
SoftwareSerial bluetoothSerial(11, 12);

// =====================================================
// L298N MOTOR DRIVER
// =====================================================

// RIGHT SIDE MOTORS
const int ENA = 5;
const int IN1 = 7;
const int IN2 = 8;

// LEFT SIDE MOTORS
const int ENB = 6;
const int IN3 = 9;
const int IN4 = 10;

// =====================================================
// SPEED
// =====================================================

int speed = 180;

// Turning speed reduction
int speed_reducing_factor = 3;

// =====================================================
// FUNCTION DECLARATIONS
// =====================================================

void Forward();
void Backward();
void TurnLeft();
void TurnRight();
void ForwardLeft();
void BackwardLeft();
void ForwardRight();
void BackwardRight();
void Stop();

// =====================================================
// SETUP
// =====================================================

void setup()
{
    // L298N pins
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Stop motors at startup
    Stop();

    // Bluetooth
    bluetoothSerial.begin(9600);

    // USB Serial Monitor
    Serial.begin(9600);

    Serial.println("================================");
    Serial.println(" Arduino UNO Bluetooth Car");
    Serial.println(" L298N + HC-05");
    Serial.println("================================");
    Serial.println("Ready!");
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
    if (bluetoothSerial.available() > 0)
    {
        char command = bluetoothSerial.read();

        Serial.print("Command: ");
        Serial.println(command);

        // =================================================
        // MOVEMENT
        // =================================================

        if (command == 'F')
        {
            Forward();
        }
        else if (command == 'B')
        {
            Backward();
        }
        else if (command == 'R')
        {
            TurnRight();
        }
        else if (command == 'L')
        {
            TurnLeft();
        }
        else if (command == 'G')
        {
            ForwardLeft();
        }
        else if (command == 'H')
        {
            BackwardLeft();
        }
        else if (command == 'I')
        {
            ForwardRight();
        }
        else if (command == 'J')
        {
            BackwardRight();
        }
        else if (command == 'S')
        {
            Stop();
        }

        // =================================================
        // SPEED CONTROL
        // =================================================

        else if (command == '0')
        {
            speed = 120;
        }
        else if (command == '1')
        {
            speed = 135;
        }
        else if (command == '2')
        {
            speed = 150;
        }
        else if (command == '3')
        {
            speed = 165;
        }
        else if (command == '4')
        {
            speed = 180;
        }
        else if (command == '5')
        {
            speed = 195;
        }
        else if (command == '6')
        {
            speed = 210;
        }
        else if (command == '7')
        {
            speed = 225;
        }
        else if (command == '8')
        {
            speed = 240;
        }
        else if (command == '9')
        {
            speed = 255;
        }
        else if (command == 'q')
        {
            speed = 255;
        }
    }
}

// =====================================================
// FORWARD
// =====================================================

void Forward()
{
    // Right motor
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Left motor
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
}

// =====================================================
// BACKWARD
// =====================================================

void Backward()
{
    // Right motor
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    // Left motor
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
}

// =====================================================
// TURN LEFT
// =====================================================

void TurnLeft()
{
    // Right motor forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Left motor backward
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
}

// =====================================================
// TURN RIGHT
// =====================================================

void TurnRight()
{
    // Right motor backward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    // Left motor forward
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
}

// =====================================================
// FORWARD LEFT
// =====================================================

void ForwardLeft()
{
    // Right motor full speed
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Left motor slow
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, speed);
    analogWrite(ENB, speed / speed_reducing_factor);
}

// =====================================================
// BACKWARD LEFT
// =====================================================

void BackwardLeft()
{
    // Right motor full speed
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    // Left motor slow
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, speed);
    analogWrite(ENB, speed / speed_reducing_factor);
}

// =====================================================
// FORWARD RIGHT
// =====================================================

void ForwardRight()
{
    // Right motor slow
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Left motor full speed
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, speed / speed_reducing_factor);
    analogWrite(ENB, speed);
}

// =====================================================
// BACKWARD RIGHT
// =====================================================

void BackwardRight()
{
    // Right motor slow
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    // Left motor full speed
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, speed / speed_reducing_factor);
    analogWrite(ENB, speed);
}

// =====================================================
// STOP
// =====================================================

void Stop()
{
    // Stop right motor
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    // Stop left motor
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}
