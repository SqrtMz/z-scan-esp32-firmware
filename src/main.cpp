#include <Arduino.h>

int pot_pin = 25;
float pot_value = 0.0;
float sum_pot = 0.0;
float mean_pot = 0.0;
float mean_pot_value = 0.0;

String command = "";

void setup() {

	Serial.begin(115200);
	pinMode(pot_pin, INPUT);
}

void loop() {

	for (size_t i = 0; i < 10; i++) {

		if (Serial.available()) {

			char m = Serial.read();
			command += String(m);
		}
	}

	sum_pot = 0.0;

	for (size_t i = 0; i < 100; i++)
	{
		pot_value = analogRead(pot_pin);
		sum_pot += pot_value;
	}
	
	mean_pot_value = sum_pot / 100;

	// Serial.println(command);
	// Serial.println("==========================");
	Serial.println(mean_pot_value);
	// Serial.println("==========================");

	// delay(1000);
	command = "";
}