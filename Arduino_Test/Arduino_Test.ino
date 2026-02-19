#include <Arduino.h>
#define Neg_1 D2
#define Neg_2 D3
#define Neg_3 D4
#define Neg_4 D5
#define Neg_5 D6
#define Neg_6 D7
#define Neg_7 D8
#define Neg_8 D9
#define Neg_9 D10
#define Neg_10 D11
#define Neg_11 D12
#define Neg_12 D13

#define Pos_1 D44
#define Pos_2 D45
#define Pos_3 D46


void cyle_neg();

void setup() {
  Serial.begin(115200);

  pinMode(Neg_1, OUTPUT);
  pinMode(Neg_2, OUTPUT);
  pinMode(Neg_3, OUTPUT);
  pinMode(Neg_4, OUTPUT);
  pinMode(Neg_5, OUTPUT);
  pinMode(Neg_6, OUTPUT);
  pinMode(Neg_7, OUTPUT);
  pinMode(Neg_8, OUTPUT);
  pinMode(Neg_9, OUTPUT);
  pinMode(Neg_10, OUTPUT);
  pinMode(Neg_11, OUTPUT);
  pinMode(Neg_12, OUTPUT);
  pinMode(Pos_1, OUTPUT);
  pinMode(Pos_2, OUTPUT);
  pinMode(Pos_3, OUTPUT);
}

void loop() {
  Serial.println("Setting Pos 1 HIGH");
  digitalWrite(Pos_1, HIGH);
  cyle_neg();
  digitalWrite(Pos_1, LOW);

  Serial.println("Setting Pos 2 HIGH");
  digitalWrite(Pos_2, HIGH);
  cyle_neg();
  digitalWrite(Pos_2, LOW);

  Serial.println("Setting Pos 3 HIGH");
  digitalWrite(Pos_3, HIGH);
  cyle_neg();
  digitalWrite(Pos_3, LOW);
  delay(1000);
}

void cyle_neg(){

  Serial.println("Setting Neg 1 HIGH");
  digitalWrite(Neg_1, HIGH);
  delay(1000);
  digitalWrite(Neg_1, LOW);

  Serial.println("Setting Neg 2 HIGH");
  digitalWrite(Neg_2, HIGH);
  delay(1000);
  digitalWrite(Neg_2, LOW);

  Serial.println("Setting Neg 3 HIGH");
  digitalWrite(Neg_3, HIGH);
  delay(1000);
  digitalWrite(Neg_3, LOW);

  Serial.println("Setting Neg 4 HIGH");
  digitalWrite(Neg_4, HIGH);
  delay(1000);
  digitalWrite(Neg_4, LOW);

  Serial.println("Setting Neg 5 HIGH");
  digitalWrite(Neg_5, HIGH);
  delay(1000);
  digitalWrite(Neg_5, LOW);

  Serial.println("Setting Neg 6 HIGH");
  digitalWrite(Neg_6, HIGH);
  delay(1000);
  digitalWrite(Neg_6, LOW);

  Serial.println("Setting Neg 7 HIGH");
  digitalWrite(Neg_7, HIGH);
  delay(1000);
  digitalWrite(Neg_7, LOW);

  Serial.println("Setting Neg 8 HIGH");
  digitalWrite(Neg_8, HIGH);
  delay(1000);
  digitalWrite(Neg_8, LOW);

  Serial.println("Setting Neg 9 HIGH");
  digitalWrite(Neg_9, HIGH);
  delay(1000);
  digitalWrite(Neg_9, LOW);

  Serial.println("Setting Neg 10 HIGH");
  digitalWrite(Neg_10, HIGH);
  delay(1000);
  digitalWrite(Neg_10, LOW);

  Serial.println("Setting Neg 11 HIGH");
  digitalWrite(Neg_11, HIGH);
  delay(1000);
  digitalWrite(Neg_11, LOW);

  Serial.println("Setting Neg 12 HIGH");
  digitalWrite(Neg_12, HIGH);
  delay(1000);
  digitalWrite(Neg_12, LOW);
}