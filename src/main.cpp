#include <Arduino.h>
#include <AccelStepper.h>

int ena_pin = 25;
int dir_pin = 26;
int pul_pin = 27;

int lim_switch_start_pin = 32;
int lim_switch_end_pin = 34;

int photo_diode_pin = 33;
float photo_diode_value = 0.0;

char incoming_data[100];
String commands[10];

AccelStepper stepper(AccelStepper::DRIVER, pul_pin, dir_pin);

void read_incoming_data() {

	uint i = 0;
	while (Serial.available()) {

		char c = Serial.read();
		
		incoming_data[i] = c;

		i++;
	}

	i = 0;
	uint j = 0;
	while (true) {

		char c = incoming_data[j];

		if (c == '\0') {

			break;
			
		} else if (c == ',') {

			i++;
			j++;

		} else {

			commands[i] += c;
			
			j++;

		}

	}
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

void execute() {
	int new_position = commands[1].toInt();

	stepper.moveTo(new_position);
}

void stop() {
	stepper.setSpeed(0);
	stepper.runSpeed();
}

void setup() {

	Serial.begin(115200);
	pinMode(photo_diode_pin, INPUT);

	pinMode(dir_pin, OUTPUT);
	pinMode(pul_pin, OUTPUT);

	pinMode(ena_pin, OUTPUT);
	digitalWrite(ena_pin, LOW); // LOW unlock - HIGH lock

	pinMode(lim_switch_start_pin, INPUT_PULLDOWN);
	pinMode(lim_switch_end_pin, INPUT_PULLDOWN);

	stepper.setMaxSpeed(1000);

	// go_to_start();

}

void loop() {

	if (Serial.available()) { read_incoming_data(); }
	
	photo_diode_value = analogRead(photo_diode_pin);

	if (Serial.available()) {Serial.println(commands[2]);} else {Serial.println(photo_diode_value);}

	if (commands[0] == "execute") {execute();}
	else if (commands[0] == "go_to_start") {go_to_start();}
	else if (commands[0] == "go_to_end") {go_to_end();}
	else if (commands[0] == "stop") {stop();}
	else if (commands[2] == "pan") {

		uint j = 0;
		for (size_t i = 0; i < 99; i++) {

			if (j == 3) { j = 0; Serial.println(); }
			if (j == 2) { Serial.print(commands[j]); j++; continue;}

			Serial.print(commands[j]);

			j++;
		}
		
	}

	delay(100);

	incoming_data[0] = '\0';
	commands[0] = "";
	commands[1] = "";
	commands[2] = "";
}