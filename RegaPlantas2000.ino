/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/

int pumpRelayPin = 2;
int moistRelayPin = 4;
int buttonPin = 3;
int errorLED = 5;
int moistPin = A0;
int maxWaterTime = 15000; // 15 seconds
int sensorMissRead = 0;

int moistOffset = 615;
int moistErrorRead = 715;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode(pumpRelayPin, OUTPUT);
  pinMode(moistRelayPin, OUTPUT);
  pinMode(errorLED, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(moistPin, INPUT);
  Serial.begin(9600);
  Serial.println("Rega plantas 2000");
}

// the loop routine runs over and over again forever:
void loop() {

  int moistReading = 0;
  int currentWaterTime = 0;

  digitalWrite(moistRelayPin, HIGH);
  Serial.println("Checking moist");
  delay(1000);

  //get and average the sensor data

  int avgCounter = 0;

  //loop 10 times for averaging
  for(int i = 0; i < 10; i++){
    avgCounter+= analogRead(moistPin);
    delay(10);
  }

  //divide by 10 to get the average
  moistReading = avgCounter /= 10;

  //print that data to the serial line
  Serial.print("Moist: ");
  Serial.println(moistReading);

  while(1) {
    
    if((moistReading > moistOffset) && (moistReading < moistErrorRead)) {
      digitalWrite(pumpRelayPin, HIGH);
      Serial.println("Pump ON");
      currentWaterTime += 50;
      Serial.print("currentWaterTime: ");
      Serial.println(currentWaterTime);
      if(currentWaterTime > maxWaterTime) {
        sensorMissRead++;
        break;
      }
    } else {
      sensorMissRead = 0;
      break;
    }
    moistReading = analogRead(moistPin);
    Serial.print("Moist level: ");
    Serial.println(moistReading);
    delay(10);
  }
  
  digitalWrite(pumpRelayPin, LOW);
  
  currentWaterTime = 0;
  if(sensorMissRead >= 3) {
    Serial.println("Error with the sensor! Needs manual reset!");
    while(1) {
      digitalWrite(errorLED, HIGH);
      Serial.print("Value of the sensor: ");
      Serial.println(analogRead(moistPin));
      delay(1000);
      digitalWrite(errorLED, LOW);
      Serial.print("Value of the sensor: ");
      Serial.println(analogRead(moistPin));
      delay(1000);
    }
  }
  
  digitalWrite(moistRelayPin, LOW);
  Serial.println("Moist checked, no need to water. Going to check again in 5 min.\nPress button to force watering.");
  int buttonState = 0;

  for(int i = 0; i < 500; i++) { // wait 10*30 = 300 seconds (5min)
    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);
  
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
      // turn pump on:
      digitalWrite(pumpRelayPin, HIGH);
      Serial.println("Forcing water!");
    } else {
      // turn pump off:
      digitalWrite(pumpRelayPin, LOW);
    }
    delay(10); // wait 10 ms
  }
  digitalWrite(pumpRelayPin, LOW);
}
