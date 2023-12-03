#include <Servo.h>

#define laserPin 7
#define xPin 5
#define yPin 6

Servo servo_x;
Servo servo_y;

int angle_x, angle_y, x_data, y_data;
int sensitivity_x = 45; 
int sensitivity_y = 45; 

String pyData;

void attach_servo() {
  servo_x.attach(xPin);
  servo_y.attach(yPin);
}

void setup() {
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, HIGH);
  
  attach_servo();
  servo_x.write(95);
  servo_y.write(-30);
  
  Serial.begin(2000000);
  Serial.setTimeout(1);
}

void loop() {
  while(!Serial.available());
  
  pyData = Serial.readString();
  x_data = pyData.substring(0, pyData.indexOf(" ")).toInt();
  y_data = pyData.substring(pyData.indexOf(" ")+1).toInt();
    
  if ((x_data + y_data) <= 3000) {
    // Argumen ke-3 harus dikalibrasi ulang setiap kali alat dibongkar pasang
    angle_x = map(x_data, 0, 1920, 55, 1920/sensitivity_x + 55);
    angle_y = map(y_data, 0, 1080, 10, 1080/sensitivity_y + 10);

    servo_x.write(angle_x);
    servo_y.write(angle_y);
  }
  else {
    // Laser Functionality
    switch (x_data + y_data) {
      case 19998:   // Toggle laser
        if (digitalRead(laserPin) == HIGH) {
          digitalWrite(laserPin, LOW);
        }
        else {
          digitalWrite(laserPin, HIGH);
        }
        break;
      case 19996: // Turn off laser
        digitalWrite(laserPin, LOW);
        break;
      case 19997: // Turn on laser
        digitalWrite(laserPin, HIGH);
        break;
    }
  }
  Serial.print(angle_x);
  Serial.print(angle_y);
  Serial.flush();
}
