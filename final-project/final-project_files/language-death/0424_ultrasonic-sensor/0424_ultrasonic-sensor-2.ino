#define TRIG1 2
#define ECHO1 3
#define TRIG2 4
#define ECHO2 5
#define TRIG3 6
#define ECHO3 7

void setup() {
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);
  Serial.begin(9600);
}

void loop() {
  long distance1 = readUltrasonicDistance(TRIG1, ECHO1);
  long distance2 = readUltrasonicDistance(TRIG2, ECHO2);
  long distance3 = readUltrasonicDistance(TRIG3, ECHO3);
//  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.print(" ");
//  Serial.print(" cm, Distance 2: ");
  Serial.print(distance2);
  Serial.print(" ");
//  Serial.print(" cm, Distance 3: ");
  Serial.println(distance3);
//  Serial.println(" cm");
  delay(500);
}

long readUltrasonicDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration / 29 / 2;
}
