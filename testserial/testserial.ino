void setup() {
  // put your setup code here, to run once:
Serial.begin(230400);
SerialUSB.begin (230400);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(SerialUSB.readString());
SerialUSB.println(Serial.readString());
}
