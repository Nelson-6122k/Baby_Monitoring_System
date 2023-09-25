//Libraries
#include <DHT.h>
//Constants
#define DHTPIN 2           /*Temperature & Humidity Sensor O/P pin*/
#define DHTTYPE DHT11      // DHT11
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor for normal 16mhz Arduino
//Variables
float hum;                 //Stores humidity value
float temp;                //Stores temperature value
const int sensor_pin = A2; /*Moisture Sensor O/P pin*/
const int led = 7;
const int s_sensor = 4; /*Sound Sensor O/P pin*/
const int m_sensor = 3; /*Motion Sensor O/p pin*/

void setup() {          // Void setup is ran only once after each powerup or reset of the Arduino board.
  Serial.begin(9600);
  dht.begin();
  pinMode(led, OUTPUT);      // Led is determined as an output here.
  pinMode(s_sensor, INPUT);  // PIR motion sensor is determined as an input here.
  pinMode(m_sensor, INPUT);
}
void (*resetFunc)(void) = 0;

void loop() {
  static int j = 0;
  static int i = 0;

  if (check(m_sensor)) {
    if (i > 0) {
      Serial.println("Motion Detection stopped (to avoid false detection!)");
    }
    i = 0;
  }
  if (check(s_sensor)) {
    if (j > 0) {
      Serial.println("Cry Detection stopped (to avoid false detection!)");
    }
    j = 0;
  }
  if (!check(s_sensor) && j <= 10) {
    j++;
    Serial.print("Crying detected :");
    Serial.println(j);
    if (j > 10) {
      Serial.println("calling parent");
      digitalWrite(led, HIGH);
      j = 0;
      delay(15000);
      digitalWrite(led, LOW);
      resetFunc();
    }
  }
  if (!check(m_sensor) && i <= 10) {
    i++;
    Serial.print("motion detected :");
    Serial.println(i);
    if (i > 10) {
      Serial.println("calling parent");
      digitalWrite(led, HIGH);
      i = 0;
      delay(15000);
      digitalWrite(led, LOW);
      resetFunc();
    }
  }

  float converted = 0.00;
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.print("Celsius = ");
  Serial.print(temp);
  //Print degree symbol
  Serial.println("°C");
  /*Fahrenheit
    //T(°F) = T(°C) × 9/5 + 32
    converted = ( temp * 1.8 ) + 32;
    Serial.print("Fahrenheit = ");
    Serial.print(converted);
    //Print degree symbol
    Serial.println("°F");*/
  Serial.print("Humidity = ");
  Serial.println(hum);
  //if temp and hum exceed the given level LED blinks ::
  if (temp > 39.00 || hum > 94.00) {
    digitalWrite(led, HIGH);
    Serial.print("Calling parent");
    delay(15000);
    digitalWrite(led, LOW);
    resetFunc();
  }
  // Moisture detection ::--
  float moisture_percentage;
  int sensor_analog;
  sensor_analog = analogRead(sensor_pin);
  moisture_percentage = (100 - ((sensor_analog / 1023.00) * 100));
  Serial.print("Moisture Percentage = ");
  Serial.print(moisture_percentage);
  Serial.print("%\n\n");
  if (moisture_percentage >= 35.00) {
    digitalWrite(led, HIGH);
    Serial.println("Calling parent");
    delay(15000);
    digitalWrite(led, LOW);
    resetFunc();
  }
}
bool check(int pin) {
  uint32_t start = millis();
  while (!digitalRead(pin)) {
    if (millis() > start + 8000)
      return true;
  }
  return false;
}
