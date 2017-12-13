const int pingL = 7;
const int pingM = 8;
const int pingR = 9;

void setup() {
  //initialize serial communication
  Serial.begin (9600);
}

void loop(){
  //establishe variables for duration of the ping
  long duration, cm;
  
  //Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingL,OUTPUT);
  digitalWrite(pingL,LOW);
  delayMicroseconds(2);
  digitalWrite(pingL,HIGH);
  delayMicroseconds(5);
  digitalWrite(pingL,LOW);
  
   pinMode(pingM,OUTPUT);
  digitalWrite(pingM,LOW);
  delayMicroseconds(2);
  digitalWrite(pingM,HIGH);
  delayMicroseconds(5);
  digitalWrite(pingM,LOW);
  
   pinMode(pingR,OUTPUT);
  digitalWrite(pingR,LOW);
  delayMicroseconds(2);
  digitalWrite(pingR,HIGH);
  delayMicroseconds(5);
  digitalWrite(pingR,LOW);
  
  //Same pin is ued to read and receive
  pinMode(pingL,INPUT);
  duration = pulseIn(pingL,HIGH);
  
  pinMode(pingM,INPUT);
  duration = pulseIn(pingM,HIGH);
  
  pinMode(pingR,INPUT);
  duration = pulseIn(pingR,HIGH);
  
  //convert the time into a distance
  cm = SecondsToCentimeters(duration);
  
  Serial.print(cm);
  delay(100);

}

long SecondsToCentimeters (long microseconds) {
  //calculation in the documentation
  return microseconds / 29 / 2;
}
