#include <Arduino.h>
#include <AccelStepper.h>

int ena_pin = 25;
int dir_pin = 26;
int pul_pin = 27;

int lim_switch_start_pin = 32;
int lim_switch_end_pin = 34;

int pot_pin = 33;
float pot_value = 0.0;
float sum_pot = 0.0;
float mean_pot = 0.0;
float mean_pot_value = 0.0;

String command = "";

AccelStepper stepper(AccelStepper::DRIVER, pul_pin, dir_pin);

void reset_position() {

}

void setup() {

	Serial.begin(115200);
	pinMode(pot_pin, INPUT);

	pinMode(dir_pin, OUTPUT);
	pinMode(pul_pin, OUTPUT);

	pinMode(ena_pin, OUTPUT);
	digitalWrite(ena_pin, LOW); // LOW unlock - HIGH lock

	pinMode(lim_switch_start_pin, INPUT_PULLDOWN);
	pinMode(lim_switch_end_pin, INPUT_PULLDOWN);

	stepper.setMaxSpeed(400);
	stepper.setSpeed(map(mean_pot_value, 0, 4095, -500, 500));
}

void loop() {

	if (digitalRead(lim_switch_start_pin)) { stepper.setSpeed(0); }
	else if (digitalRead(lim_switch_end_pin)) { stepper.setSpeed(0); }
	else { stepper.setSpeed(map(mean_pot_value, 0, 4095, -500, 500)); }

	stepper.runSpeed();

	Serial.print("Start limit switch: ");
	Serial.print(digitalRead(lim_switch_start_pin));
	Serial.print(" || ");
	Serial.print("End limit switch: ");
	Serial.print(digitalRead(lim_switch_end_pin));
	Serial.print(" || ");

	for (size_t i = 0; i < 20; i++) {

		if (Serial.available()) {

			char m = Serial.read();
			command += String(m);

		} else { break; }
	}

	if (Serial.available()) { Serial.println(command); }

	sum_pot = 0.0;

	for (size_t i = 0; i < 100; i++)
	{
		pot_value = analogRead(pot_pin);
		sum_pot += pot_value;
	}
	
	mean_pot_value = sum_pot / 100;

	if (!Serial.available()) { Serial.println(mean_pot_value); }


	if (command == "write") {

		for (size_t i = 0; i < 1000; i++)
		{
			Serial.println("WRITING");
		}

	} else if (command == "read") {

		for (size_t i = 0; i < 1000; i++)
		{
			Serial.println("READING");
		}

	} else if (command == "reset_position") { reset_position(); }

	delay(1);

	command = "";
}