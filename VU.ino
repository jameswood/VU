const int sinePin = 3;
const int randomPin = 5;
const int serialLedPin = 6;
//pins 3, 5, 6, 9, 10, and 11
const int maxOutputVoltage = 3;
const int maxBright = 51 * maxOutputVoltage; //51 is ~1v
const int minBright = 0;
const int sinePeriod = 3;
const int randomPeriod = 1;
const bool debug = false;

unsigned long randomLastRunMillis = 0;

void setup() {
  Serial.begin(115200);
  pinMode(sinePin, OUTPUT);
  pinMode(randomPin, OUTPUT);
  pinMode(serialLedPin, OUTPUT);
}

void loop() {
  int currentSecond = millis() / 1000;
  analogWrite(sinePin, calculateSine());
  if(millis() - randomLastRunMillis > (randomPeriod * 1000)) { analogWrite(randomPin, calculateRandom()); };
  if(Serial.available() > 0) { analogWrite(serialLedPin, map(grabSerial(), 0, 100, minBright, maxBright)); };
}

int calculateSine() {
  double tempMillis = millis();
  double seed = (tempMillis / (1000.0 * sinePeriod)) * 2.0 * PI;
  int outputValue = (sin(seed) - (-1)) * (maxBright - minBright) / (1 - (-1)) + minBright;
  return outputValue;
}

int calculateRandom(){
  randomLastRunMillis = millis();
  int randomBrightness = random(minBright, maxBright);
  if (debug) Serial.println(randomBrightness);
  return randomBrightness;
}

int grabSerial(){
  String incomingString;
  while (Serial.available() > 0) {
    char incomingByte = Serial.read();
    if (debug) Serial.println(incomingByte, DEC);
    if (incomingByte != 0x13) { //carriage return
      incomingString.concat(incomingByte);
    } else {
      Serial.flush();
    }
  }
  if (debug) Serial.println("I received: " + incomingString);
  return incomingString.toInt();
}
