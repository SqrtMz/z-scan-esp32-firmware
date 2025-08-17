#include <Arduino.h>

void read_incoming_data(char *incoming_data, String *commands) {

	int i = 0;
	while (Serial.available()) {

		char c = Serial.read();
		
		incoming_data[i] = c;

		i++;
	}

	i = 0;
	int j = 0;
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