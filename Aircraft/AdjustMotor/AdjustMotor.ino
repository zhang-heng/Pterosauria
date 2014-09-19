#include <Servo.h>
int ServoPins[4];
int ServoMin[4];
Servo Servos[4];

void setup() {
  ServoPins[0] = 5;
  ServoPins[1] = 9;
  ServoPins[2] = 6;
  ServoPins[3] = 10;
  ServoMin[0] = 750;
  ServoMin[1] = 750;
  ServoMin[2] = 750;
  ServoMin[3] = 750;
  for (int i = 0; i < 4; i++) {
    Servos[i].attach(ServoPins[i]);
    Servos[i].writeMicroseconds(700);
  }
  delay(5000);
  Serial.begin(115200);
}

void loop() {
  switch (Serial.read()) {
    case '1':
      ServoMin[0]++;break;
    case 'q':
      ServoMin[0]--;;break;
    case '2':
      ServoMin[1]++;;break;
    case 'w':
      ServoMin[1]--;;break;
    case '3':
      ServoMin[2]++;;break;
    case 'e':
      ServoMin[2]--;;break;
    case '4':
      ServoMin[3]++;;break;
    case 'r':
      ServoMin[3]--;;break;
  }
  for (int i = 0; i < 4; i++) {
    Servos[i].writeMicroseconds(ServoMin[i]);
    Serial.print(ServoMin[i]);
    Serial.print("\t");
  }
  Serial.print("\n");
  delay(10);
}
