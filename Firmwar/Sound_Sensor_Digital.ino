/*
microphone sketch

SparkFun breakout board for Electret Microphone is connected to analog pin 0
*/

const int ledPin1 = 1;
const int ledPin2 = 2;
const int ledPin3 = 0;
const int Button_pin = 3;
const int Mic_ADC = A2;
int mode;
const int middleValue = 512;      //the middle of the range of analog values
const int numberOfSamples = 128;  //how many readings will be taken each time

int sample;                       //the value read from microphone each time
long signal;                      //the reading once you have removed DC offset
long averageReading;              //the average of that loop of readings

long runningAverage = 0;          //the running average of calculated values
const int averagedOver = 16;     //how quickly new values affect running average
		//bigger numbers mean slower
//threshold levels for the LED´s

const int threshold_LED_1_A = 100;
const int threshold_LED_2_A = 800;
const int threshold_LED_1_B = 150;
const int threshold_LED_2_B = 900;
const int threshold_LED_1_C = 200;
const int threshold_LED_2_C = 1000;
int threshold_LED_EFT_2;
int threshold_LED_EFT_1;
int counter_of_2millis = 0;

#include <EEPROM.h>

void setup() {
	pinMode(ledPin1, OUTPUT);
	pinMode(ledPin2, OUTPUT);
	pinMode(ledPin3, OUTPUT);
	pinMode(Button_pin, INPUT);
	set_numbers();
}

void loop() {
	long sumOfSquares = 0;
	for (int i = 0; i < numberOfSamples; i++) { //take many readings and average them
		sample = analogRead(Mic_ADC);               //take a reading
		signal = (sample - middleValue);      //work out its offset from the center
		signal *= signal;                     //square it to make all values positive
		sumOfSquares += signal;               //add to the total
	}
	averageReading = sumOfSquares / numberOfSamples;     //calculate running average
	runningAverage = (((averagedOver - 1) * runningAverage) + averageReading) / averagedOver;
	//mode 1
	if (EEPROM.read(0x00) == 0) {
		digitalWrite(ledPin1, HIGH);
		if (runningAverage > threshold_LED_EFT_1) {
			digitalWrite(ledPin2, HIGH);
		}
		else {
			digitalWrite(ledPin2, LOW);
		}

		if (runningAverage > threshold_LED_EFT_2) {
			digitalWrite(ledPin3, HIGH);
		}
		else {
			digitalWrite(ledPin3, LOW);
		}

	}
	//mode 2;
	if (EEPROM.read(0x00) == 0x01) {
		if (runningAverage < threshold_LED_EFT_1) {
			digitalWrite(ledPin1, HIGH);
		}
		else {
			digitalWrite(ledPin1, LOW);
		}
		if (runningAverage > threshold_LED_EFT_1&& runningAverage < threshold_LED_EFT_2) {
			digitalWrite(ledPin2, HIGH);
		}
		else {
			digitalWrite(ledPin2, LOW);
		}

		if (runningAverage > threshold_LED_EFT_2) {
			digitalWrite(ledPin3, HIGH);
		}
		else {
			digitalWrite(ledPin3, LOW);
		}
	}

	if (digitalRead(Button_pin) == HIGH) {
		counter_of_2millis = 0;
		while (digitalRead(Button_pin) == HIGH) {
			delay(2);
			counter_of_2millis = counter_of_2millis + 1;
		}
		if (counter_of_2millis < 100) {
			if (EEPROM.read(0x00) == 0) {
				EEPROM.write(0x00, 1);
			}
			else {
				EEPROM.write(0x00, 0);
			}
		}
		else {
			if (EEPROM.read(0x01) == 0) {
				EEPROM.write(0x01, 1);
			}
			else {
				if (EEPROM.read(0x01) == 1) {
					EEPROM.write(0x01, 2);
				}
				else {
					EEPROM.write(0x01, 0);
				}
			}
			set_numbers();
		}
	}
}
void set_numbers() {
	if (EEPROM.read(0x01) == 0) {
		threshold_LED_EFT_1 = threshold_LED_1_A;
		threshold_LED_EFT_2 = threshold_LED_2_A;
	}
	else {
		if (EEPROM.read(0x01) == 1) {
			threshold_LED_EFT_1 = threshold_LED_1_B;
			threshold_LED_EFT_2 = threshold_LED_2_B;
		}
		else {
			threshold_LED_EFT_1 = threshold_LED_1_C;
			threshold_LED_EFT_2 = threshold_LED_2_C;
		}
	}

}
