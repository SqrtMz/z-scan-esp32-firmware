#include <Arduino.h>
#include <DHT.h>

DHT dht(21, DHT22);

void setup() {

	pinMode(17, OUTPUT);
	digitalWrite(17, HIGH);

	Serial.begin(9600);

	dht.begin();

	Serial.println("Ready");
}

void loop() {

	float temp = dht.readTemperature();
	float hum = dht.readHumidity();

	Serial.print("The temperature is: ");
	Serial.println(temp);

	Serial.print("The humidity is: ");
	Serial.println(hum);

	Serial.println("=======================");

	delay(1000);
}