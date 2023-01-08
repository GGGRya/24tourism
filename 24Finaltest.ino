//data
float temp0[] = { -1, 2, 4, 4.5, 8, 11, 14, 16, 21, 22, 26, 27, 28, 28.5, 27, 25, 23, 22, 18, 14, 10, 6, 5, 3};
float diff[] = {10, 10, 10.1, 11, 11.5, 12, 12.5, 12.5, 12.6, 12.7, 12.5, 10.2, 9.9, 9, 9.5, 10, 10.1, 10.2, 10.2, 10.4, 10, 10, 10, 10};
float temp[] = { -3, -2.5, -1, 2, 4.5, 7, 12, 14.5, 17.5, 20, 22, 25, 25.5, 26, 23, 21, 19, 16, 13, 10, 8, 2.5, 0, -1.5};
float humid[] = {58, 56, 57, 56.5, 56, 55, 55.5, 56, 57.5, 59, 62, 69.5, 75, 77.5, 77, 76, 73, 70, 68, 66, 65, 61, 60.5, 60};
float samp[] = {7571, 8190, 4095, 8263, 4333, 3572, 4857, 320, 1525, 3892, 3812, 7983, 5708, 6287, 7445, 7485, 7624, 2715, 4172, 7724, 2346, 480, 999, 2935};
float period[] = {1.971, 1.733, 0.829, 1.092, 1.505, 0.495, 0.79, 0.33, 0.651, 0.331, 0.347, 0.878, 0.95, 0.85, 1.421, 0.958, 0.982, 0.335, 0.655, 1.752, 1.006, 0.643, 1.05, 0.435};
float dt[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float dh[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float d[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float t;
float h;
float s;
float p;
float min = 9999;
int minIndex = 0;
int si=1;

//temp & humid modular
#include <dht.h>
#define dataPin 7
dht DHT;

//ultra sonic sensor
int trig = 3;
int echo = 2;
int duration;
int distance;

//touch sensor
int httm = 4;
int lastState = LOW;
int currentState;

//OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

//water pump
const int RELAY_PIN = 13;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(httm, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(115200);

  //OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {

  int readData = DHT.read22(dataPin);
  currentState = digitalRead(httm);

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  const unsigned long duration = pulseIn(echo, HIGH);
  int distance= duration*0.034/2;
  float dist = map(distance, 0, 150, 15, 0);

  if (lastState == LOW && currentState == LOW)
  {
    t = DHT.temperature;
    h = DHT.humidity;
    for (int i = 0; i < 24; i++) {
      dt[i] = abs(t - temp[i]) / 0.4;
      dh[i] = abs(h - humid[i]);
      d[i] = dt[i] + dh[i];
      if (d[i] < min) {
        min = d[i];
        minIndex = i;
      }
    }
    s=samp[minIndex];
    p=period[minIndex];
        digitalWrite(RELAY_PIN, HIGH);
  }
  if (lastState == LOW && currentState == HIGH)
  {
    t = temp[minIndex];
    h = humid[minIndex];
    digitalWrite(RELAY_PIN, LOW);
  }
  if (lastState == HIGH && currentState == LOW)
  {
    t = DHT.temperature;
    h = DHT.humidity;
        digitalWrite(RELAY_PIN, HIGH);
  }
  lastState = currentState;

  Serial.print(t);
  Serial.print(" ");
  Serial.print(h);
  Serial.print(" ");
  Serial.print(dist);
  Serial.print(" ");  
  Serial.print(samp[minIndex]);
  Serial.print(" ");
  Serial.println(period[minIndex]);
  //Serial.println(minIndex);

  display.clearDisplay();
  display.setCursor(10, 10);
      if (minIndex == 0)
        display.println("Minor Cold");
      else if (minIndex == 1)
        display.println("Major Cold");
      else if  (minIndex == 2)
        display.println("Beginning of Spring");
      else if  (minIndex == 3)
        display.println("Rain Water");
      else if  (minIndex == 4)
        display.println("Awakening of Insects");
      else if  (minIndex == 5)
        display.println("Spring Equinox");
      else if  (minIndex == 6)
        display.println("Pure Brightness");
      else if  (minIndex == 7)
        display.println("Grain Rain");
      else if  (minIndex == 8)
        display.println("Beginning of Summer");
      else if  (minIndex == 9)
        display.println("Grain Buds");
      else if  (minIndex == 10)
        display.println("Grain in Ear");
      else if  (minIndex == 11)
        display.println("Summer Solstice");
      else if  (minIndex == 12)
        display.println("Minor Heat");
      else if  (minIndex == 13)
        display.println("Major heat");
      else if  (minIndex == 14)
        display.println("Beginning of Autumn");
      else if  (minIndex == 15)
        display.println("End of Heat");
      else if  (minIndex == 16)
        display.println("White Dew");
      else if  (minIndex == 17)
        display.println("Autumn Equinox");
      else if  (minIndex == 18)
        display.println("Cold Dew");
      else if  (minIndex == 19)
        display.println("Frost's Descent");
      else if  (minIndex == 20)
        display.println("Beginning of Winter");
      else if  (minIndex == 21)
        display.println("Minor Snow");
      else if  (minIndex == 22)
        display.println("Major Snow");
      else if  (minIndex == 23)
  display.println("Winter Solstice");
  display.display();
  delay(10);

}
