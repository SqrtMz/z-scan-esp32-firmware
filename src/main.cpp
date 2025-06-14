#include <Arduino.h>
#include <AccelStepper.h>

int ena_pin = 25;
int dir_pin = 26;
int pul_pin = 27;

int lim_switch_start_pin = 32;
int lim_switch_end_pin = 34;

int ref_photo_diode_pin = 33;
float pot_value = 0.0;

String command = "";

AccelStepper stepper(AccelStepper::DRIVER, pul_pin, dir_pin);

String get_command() {

	for (size_t i = 0; i < 20; i++) {

		if (Serial.available()) {

			char m = Serial.read();
			command += String(m);

		} else { break; }
	}

	return command;
}

void go_to_start() {

	while (!digitalRead(lim_switch_start_pin)) {
		stepper.setSpeed(-750);
		stepper.runSpeed();
	}

	stepper.setSpeed(0);
	stepper.runSpeed();
	stepper.setCurrentPosition(0);
}

void go_to_end() {

	while (!digitalRead(lim_switch_end_pin)) {
		stepper.setSpeed(750);
		stepper.runSpeed();
	}

	stepper.setSpeed(0);
	stepper.runSpeed();
}

void setup() {

	Serial.begin(115200);
	pinMode(ref_photo_diode_pin, INPUT);

	pinMode(dir_pin, OUTPUT);
	pinMode(pul_pin, OUTPUT);

	pinMode(ena_pin, OUTPUT);
	digitalWrite(ena_pin, LOW); // LOW unlock - HIGH lock

	pinMode(lim_switch_start_pin, INPUT_PULLDOWN);
	pinMode(lim_switch_end_pin, INPUT_PULLDOWN);

	stepper.setMaxSpeed(1000);

	go_to_start();
	
}

void loop() {

	pot_value = analogRead(ref_photo_diode_pin);

	Serial.print("Start limit switch: ");
	Serial.print(digitalRead(lim_switch_start_pin));
	Serial.print(" || ");
	Serial.print("End limit switch: ");
	Serial.print(digitalRead(lim_switch_end_pin));
	Serial.print(" || ");

	command = get_command();

	if (Serial.available()) { Serial.println(command); }
	if (!Serial.available()) { Serial.println(pot_value); }


	// Command section
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

	} else if (command == "go_to_start") {

		go_to_start();

	} else if (command == "go_to_end") {

		go_to_end();

	}

	delay(1);

	command = "";
}