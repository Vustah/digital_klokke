// #define A 2
// #define B 5
// #define C 4
// #define D 3
#define Second_ones 8
#define Second_tens 6
#define Minute_ones 9
#define Minute_tens 7
#define increase_time A0
#define decrease_time A1
#define adjust A2
#define config_led 10

void calcBinaryValue(int number, int* retNumber);

int SegBit[4] = { 2, 5, 4, 3 };  //A,B,C,D
enum SegmentState { m_ones,
                    m_tens,
                    s_ones,
                    s_tens
};
SegmentState segment_state = s_ones;
// unsigned char segment_state = s_ones;

void setup() {
  // put your setup code here, to run once:
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
}

void loop() {
  // put your main code here, to run repeatedly:
}



void write_time(int minutes, int seconds) {
  int SegWrite[4];

  digitalWrite(Second_ones, LOW);
  digitalWrite(Second_tens, LOW);
  digitalWrite(Minute_ones, LOW);
  digitalWrite(Minute_tens, LOW);

  switch (segment_state) {
    case s_ones:
      calcBinaryValue(seconds, &SegWrite[0]);
      digitalWrite(Second_ones, HIGH);
      segment_state = s_tens;

    case s_tens:
      calcBinaryValue(seconds, &SegWrite[0]);
      digitalWrite(Second_tens, HIGH);
      segment_state = m_ones;

    case m_ones:
      calcBinaryValue(minutes, &SegWrite[0]);
      digitalWrite(Minute_ones, HIGH);
      segment_state = m_tens;

    case m_tens:
      calcBinaryValue(minutes, &SegWrite[0]);
      digitalWrite(Minute_tens, HIGH);
      segment_state = s_ones;

    default:
      segment_state = s_ones;
  }
  for (int i = 0; i < 4; i++) {
    digitalWrite(SegBit[i], SegWrite[i]);
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