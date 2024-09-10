#include "Button2.h"

#define Second_ones 7
#define Second_tens 9
#define Minute_ones 6
#define Minute_tens 8
#define config_led 10
#define increase_time A0
#define decrease_time A1
#define adjust A2
#define increase_pin A0
#define decrease_pin A1
#define adjust_pin A2

Button2 increase_btn;
Button2 decrease_btn;
Button2 adjust_btn;


void calcBinaryValue(int number, int* retNumber);

int SegBit[4] = { 2, 5, 4, 3 };  //A,B,C,D

enum SegmentState { h_ones,
                    h_tens,
                    m_ones,
                    m_tens
};
SegmentState segment_state = m_ones;
// unsigned char segment_state = m_ones;

int second_counter = 0;
int minute_counter = 27;
int hour_counter = 17;
int prev_time;
int current_time;
bool config_led_state = true;


void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(SegBit[i], OUTPUT);
  }
  pinMode(Second_ones, OUTPUT);
  pinMode(Second_tens, OUTPUT);
  pinMode(Minute_ones, OUTPUT);
  pinMode(Minute_tens, OUTPUT);
  pinMode(increase_time, INPUT);
  pinMode(decrease_time, INPUT);
  pinMode(adjust, INPUT);
  pinMode(config_led, OUTPUT);

  prev_time = millis();
  Serial.begin(115200);

  increase_btn.begin(increase_pin);
  decrease_btn.begin(decrease_pin);
  adjust_btn.begin(adjust_pin);
}

int time_difference;
int acumulated_time_difference;

void loop() {
  track_time();
}

void track_time(){
write_time(hour_counter, minute_counter, second_counter);

  delay(5);
  current_time = millis();
  time_difference = current_time - prev_time;

  if (time_difference >= 999) {
    prev_time = current_time;
    second_counter += 1;

    //tick seconds
    if (second_counter >= 60) {
      minute_counter += 1;
      second_counter = 0;
    }
    //tick minutes
    if (minute_counter >= 60) {
      hour_counter += 1;
      minute_counter = 0;
    }
    //tick hours
    if (hour_counter >= 24) {
      hour_counter = 0;
    }

    //Adjust timing due to incorrections in timing.
    if (time_difference != 1000) {
      acumulated_time_difference += time_difference - 1000;
    }
    if (acumulated_time_difference > 1000 && second_counter > 0) {
      second_counter += 1;
      acumulated_time_difference -= 1000;
    } else if (acumulated_time_difference < -1000 && second_counter < 59) {
      second_counter -= 1;
      acumulated_time_difference += 1000;
    }
    Serial.print(time_difference);
    Serial.print(" ");
    Serial.print(acumulated_time_difference);
    Serial.print(" ");
    Serial.print(hour_counter);
    Serial.print(" ");
    Serial.print(minute_counter);
    Serial.print(" ");
    Serial.println(second_counter);
  }
}

void seconds_counter() {
  config_led_state = !(config_led_state);
  if (config_led_state) {
    analogWrite(config_led, 240);
  } else {
    analogWrite(config_led, 255);
  }
}

int minute_ones_display, minute_tens_display;
int hour_ones_display, hour_tens_display;

void send_number_to_converter(int number[4]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(SegBit[i], number[i]);
  }
}


void write_time(int hour, int minute, int second) {
  minute_ones_display = minute % 10;
  minute_tens_display = minute / 10;
  hour_ones_display = hour % 10;
  hour_tens_display = hour / 10;

  // Turn off all the displays
  digitalWrite(Second_ones, LOW);
  digitalWrite(Second_tens, LOW);
  digitalWrite(Minute_ones, LOW);
  digitalWrite(Minute_tens, LOW);
  analogWrite(config_led, 255);

  int SegWrite[4];

  // Turn on the right segment with right value.
  switch (segment_state) {
    case m_ones:
      calcBinaryValue(minute_ones_display, &SegWrite[0]);
      send_number_to_converter(SegWrite);
      if (second > 12) {
        // seconds_counter();
        analogWrite(config_led, 0);
      }
      digitalWrite(Second_ones, HIGH);
      segment_state = m_tens;
      break;

    case m_tens:
      calcBinaryValue(minute_tens_display, &SegWrite[0]);
      send_number_to_converter(SegWrite);
      if (second > 24) {
        // seconds_counter();
        analogWrite(config_led, 0);
      }
      digitalWrite(Second_tens, HIGH);

      segment_state = h_ones;
      break;

    case h_ones:
      calcBinaryValue(hour_ones_display, &SegWrite[0]);
      send_number_to_converter(SegWrite);
      if (second > 36) {
        // seconds_counter();
        analogWrite(config_led, 0);
      }
      digitalWrite(Minute_ones, HIGH);

      segment_state = h_tens;
      break;
    case h_tens:
      calcBinaryValue(hour_tens_display, &SegWrite[0]);
      send_number_to_converter(SegWrite);
      if (second > 48) {
        // seconds_counter();
        analogWrite(config_led, 0);
      }
      digitalWrite(Minute_tens, HIGH);

      segment_state = m_ones;
      break;
    default:
      segment_state = m_ones;
      break;
  }
}

//decimal to binary functions
int rest;
int result;
int div2remainder(int number) {
  result = number / 2;
  rest = number % 2;
}

void calcBinaryValue(int number, int* retNumber) {
  int tmpNumber = number;
  for (int i = 0; i < 4; i++) {
    div2remainder(tmpNumber);
    retNumber[i] = rest;
    tmpNumber = result;
  }
}