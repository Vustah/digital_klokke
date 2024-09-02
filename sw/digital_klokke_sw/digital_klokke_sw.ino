#include "Button2.h"

#define Second_ones 8
#define Second_tens 6
#define Minute_ones 9
#define Minute_tens 7
#define increase_time A0
#define decrease_time A1
#define adjust A2
#define config_led 10
#define increase_pin A0
#define decrease_pin A1
#define adjust_pin A2

Button2 increase_btn;
Button2 decrease_btn;
Button2 adjust_btn;


void calcBinaryValue(int number, int* retNumber);

int SegBit[4] = { 2, 5, 4, 3 };  //A,B,C,D

enum SegmentState { m_ones,
                    m_tens,
                    s_ones,
                    s_tens
};
SegmentState segment_state = s_ones;
// unsigned char segment_state = s_ones;

int second_counter = 0;
int minute_counter = 54;
int hour_counter = 18;
int prev_time;
int current_time;



void setup() {
  // pinMode(A, OUTPUT);
  // pinMode(B, OUTPUT);
  // pinMode(C, OUTPUT);
  // pinMode(D, OUTPUT);
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
  write_time(hour_counter, minute_counter);

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
    if (time_difference!=1000){
      acumulated_time_difference += time_difference-1000;
    }
    if(acumulated_time_difference>1000){
      second_counter-=1;
    }else if(acumulated_time_difference<-1000){
      second_counter+=1;
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


int minute_ones_display, minute_tens_display;
int hour_ones_display, hour_tens_display;


void send_number_to_converter(int number[4]){
  for (int i = 0; i < 4; i++) {
    digitalWrite(SegBit[i], number[i]);
  }
}


void write_time(int hour, int minute) {
  minute_ones_display = minute % 10;
  minute_tens_display = minute / 10;
  hour_ones_display = hour % 10;
  hour_tens_display = hour / 10;

  // Turn off all the displays
  digitalWrite(Second_ones, LOW);
  digitalWrite(Second_tens, LOW);
  digitalWrite(Minute_ones, LOW);
  digitalWrite(Minute_tens, LOW);

  int SegWrite[4];
  
  // Turn on the right segment with right value. 
  switch (segment_state) {
    case s_ones:
      calcBinaryValue(minute_ones_display, &SegWrite[0]);
      send_number_to_converter(SegWrite);
      digitalWrite(Second_ones, HIGH);

      segment_state = s_tens;
      break;
    case s_tens:
      calcBinaryValue(minute_tens_display, &SegWrite[0]);
      send_number_to_converter(SegWrite);
      digitalWrite(Second_tens, HIGH);

      segment_state = m_ones;
      break;
    case m_ones:
      calcBinaryValue(hour_ones_display, &SegWrite[0]);
      send_number_to_converter(SegWrite);
      digitalWrite(Minute_ones, HIGH);

      segment_state = m_tens;
      break;
    case m_tens:
      calcBinaryValue(hour_tens_display, &SegWrite[0]);
      send_number_to_converter(SegWrite);
      digitalWrite(Minute_tens, HIGH);

      segment_state = s_ones;
      break;
    default:
      segment_state = s_ones;
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