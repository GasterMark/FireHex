#include <TroykaMQ.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define PIN_MQ2         A0
#define PIN_MQ2_HEATER  12

#define DHTPIN 10
#define DHTTYPE    DHT11

#define  flamePin  A4

MQ2 mq2(PIN_MQ2, PIN_MQ2_HEATER);

DHT_Unified dht(DHTPIN, DHTTYPE);

int  flame;

uint32_t delayMS;

void setup() 
{
   Serial.begin(9600);
   pinMode(flamePin, INPUT);

  mq2.heaterPwrHigh();
  Serial.println("Heated sensor");

  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));

  delayMS = sensor.min_delay / 1000;
}

void DHT(){
  
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}

void MQ2()
{
  if (!mq2.isCalibrated() && mq2.heatingCompleted()) {
    mq2.calibrate();
    Serial.print("Ro = ");
    Serial.println(mq2.getRo());
  }
  
  if (mq2.isCalibrated() && mq2.heatingCompleted()) {
    Serial.print("Ratio: ");
    Serial.print(mq2.readRatio());
    Serial.print("\tLPG: ");
    Serial.print(mq2.readLPG());
    Serial.print(" ppm");
    Serial.print("\tMethane: ");
    Serial.print(mq2.readMethane());
    Serial.print(" ppm");
    Serial.print("\tSmoke: ");
    Serial.print(mq2.readSmoke());
    Serial.print(" ppm");
    Serial.print("\tHydrogen: ");
    Serial.print(mq2.readHydrogen());
    Serial.println(" ppm");
  }
}

void flamesensor()
{
   Serial.print("Flame Sensor - ");

   if (flame == HIGH)
      Serial.println("Fire");
   else
      Serial.println("Peace");
}

void loop() 
{
  MQ2();
  DHT();
  flamesensor();
  delay(delayMS);
}
