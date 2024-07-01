// Ultrasonic Pins (3 ultrasonic sensors)
// Front sensor
#define trigPin1 13
#define echoPin1 12
// Right sensor
#define trigPin2 11 
#define echoPin2 10 
// Back sensor
#define trigPin3 A1 
#define echoPin3 A2
// Motors Speed 
#define S 125

// Motor A
#define enA 2
#define in1 4
#define in2 3

// Motor B
#define enB 7
#define in3 5
#define in4 6

// Parking space variable
int space = 0;

// Move Functions Prototypes
void Stop();
void MoveForward();
void MoveLeft();
void MoveBack();
void MoveRight();
void MoveBackFirst();

// Ultrasonic Function Prototype
int UFun(int triger, int echo);

// Functions prototypes
void Park();
int Check();

void setup() {
  Serial.begin(9600);

  // Ultrasonic pins
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  delay(2000);
  Serial.println("Checking parking space...");
  Serial.println(space);
  MoveForward();
  delay(50);

  if (Check() == 1 && space > 35) { // space is measured by trial and error to check the right value, it varies by speed
    Serial.println("Found suitable space:");
    Serial.println(space);
    space = 0;
    Stop();
    Serial.println("Stopping...");
    delay(1000);
    Park();
    while (true); // Stop further execution
  } else {
    if (UFun(trigPin1, echoPin1) > 4 && UFun(trigPin3, echoPin3) > 4) {
      Serial.println("Moving forward...");
      Serial.print("Front Distance: ");
      Serial.println(UFun(trigPin1, echoPin1));
      Serial.print("Back Distance: ");
      Serial.println(UFun(trigPin3, echoPin3));
      MoveForward();
    } else {
      Serial.println("Stopping...");
      Stop();
    }
  }
}

void Park() { // Parking algorithm
  // Step 1: Move back first
  MoveBackFirst();
  delay(1000);
  Stop();
  delay(2000);

  // Step 2: Turn left to align
  MoveLeft();
  delay(1300);
  Stop();
  delay(1500);
  MoveBack();
  delay(1000);
  MoveRight();
  delay(1300);
  Stop();
  delay(2000);
  

  // Step 3: Move back until the car is in position
  int distanceBack;
  int distanceFront;
  int distanceDiff;
  while (true) {
    distanceBack = UFun(trigPin1, echoPin1);
    distanceFront = UFun(trigPin3, echoPin3);
    distanceDiff = distanceFront - distanceBack;
    if(distanceDiff>=3){
      MoveForward();
    }
    else if(distanceDiff<=-3){
      MoveBack();
    }
    else if(3>distanceDiff>-3){
      Stop();
      break;
    }
  }
  delay(2000);

  // Step 4: Turn right to straighten the car
  //MoveRight();
  //delay(1600);
  Stop();
}

int Check() { // Checking the suitable parking space for the car width
  int Flag = 0;
  while (UFun(trigPin2, echoPin2) > 15) {
    Serial.println("Checking right sensor...");
    Serial.println(UFun(trigPin2, echoPin2));
    MoveForward();
    space++;
    delay(20);
    Flag = 1;
  }
  Stop(); // Stop the car after checking
  return Flag;
}

int UFun(int triger, int echo) { // Ultrasonic reading 
  long duration;
  int distance;
  digitalWrite(triger, LOW);
  delayMicroseconds(2);
  digitalWrite(triger, HIGH);
  delayMicroseconds(10);
  digitalWrite(triger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration / 2 / 29.1; // Convert to centimeters
  return distance;
}

void MoveBackFirst() {
  // Turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // Set speed
  analogWrite(enA, S - 25);
  // Turn on motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  // Set speed
  analogWrite(enB, S - 25);
}

void MoveBack() {
  // Turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // Set speed
  analogWrite(enA, S + 100);
  // Turn on motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  // Set speed
  analogWrite(enB, S + 100);
}

void MoveLeft() {
  // Turn on motor A
  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
  // Set speed
  analogWrite(enA, S+50);
  // Turn on motor B
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
  // Set speed
  analogWrite(enB, S+50);
}

void MoveRight() {
  // Turn on motor A
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  // Set speed
  analogWrite(enA, S +50);
  // Turn on motor B
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  // Set speed
  analogWrite(enB, S +50);
}

void Stop() {
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void MoveForward() {
  // Turn on motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  // Set speed
  analogWrite(enA, S);
  // Turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // Set speed
  analogWrite(enB, S);
}