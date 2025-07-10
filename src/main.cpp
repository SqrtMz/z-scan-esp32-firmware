#include <Arduino.h>
#include <AccelStepper.h>

int ena_pin = 25;
int dir_pin = 26;
int pul_pin = 27;

int lim_switch_start_pin = 32;
int lim_switch_end_pin = 34;

int photo_diode_pin = 33;
float photo_diode_value = 0.0;
float sum_pot = 0.0;
float mean_pot = 0.0;
float mean_pot_value = 0.0;

String commands[10];

AccelStepper stepper(AccelStepper::DRIVER, pul_pin, dir_pin);

void read_incoming_data() {
	
	char incoming_data[100];

	for (size_t i = 0; i < 100; i++) {
		
		if (Serial.available()) {

			char m = Serial.read();
			
			incoming_data[i] = m;

		} else { break; }

	}

	String token;

	int i = 0;
	while (token = strtok(NULL, ",")) {

		token = strtok(incoming_data, ",");
		commands[i] = token;
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

	go_to_start();

}

void loop() {

	read_incoming_data();

	if (Serial.available()) {Serial.println(commands[0]);} else {Serial.println(photo_diode_value);}

	if (commands[0] == "execute") {execute();}
	else if (commands[0] == "go_to_start") {go_to_start();}
	else if (commands[0] == "go_to_end") {go_to_end();}
	else if (commands[0] == "stop") {stop();}

	String commands[10];
}