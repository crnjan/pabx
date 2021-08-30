#include <stdio.h>
#include <cstring>
#include "recognizer.h"

typedef enum { IDLE, IN_CALL } StateType;

// 11, 12, 13, 14 = pins used for ring selectors
//
// Raspberry Pi Model B Rev 1
// 
// 0 -> 11 (header)
// 1 -> 12 (header)
// 2 -> 13 (header)
// 3 -> 15 (header)
static const int RING_PHONE_1_PIN = 0;
static const int RING_PHONE_2_PIN = 1;
static const int RING_PHONE_3_PIN = 2;
static const int RING_PHONE_4_PIN = 3;

// 4 -> 16 (header)
static const int LINE_SENSE_PIN = 4;

// 5 -> 18 (header)
static const int RINGER_POWERED_PIN = 5;

// 8 -> 3 (header)
// 9 -> 5 (header)
static const int RING_GENERATOR_PIN = 8;
static const int RING_GENERATOR_INVERTED_PIN = 9;

static void ring(int ringPhonePin) {
  digitalWrite(RING_GENERATOR_PIN, HIGH);
  digitalWrite(RING_GENERATOR_INVERTED_PIN, HIGH);
  delay(10);

  digitalWrite(RINGER_POWERED_PIN, HIGH);
  delay(100);

  digitalWrite(ringPhonePin, HIGH);
  delay(100);

  for (int i = 0; i < 50; ++i) {
      digitalWrite(RING_GENERATOR_PIN, HIGH);
      digitalWrite(RING_GENERATOR_INVERTED_PIN, LOW);
      delay(25);
      digitalWrite(RING_GENERATOR_INVERTED_PIN, HIGH);
      digitalWrite(RING_GENERATOR_PIN, LOW);
      delay(25);
  }

  digitalWrite(RING_GENERATOR_PIN, HIGH);
  digitalWrite(RING_GENERATOR_INVERTED_PIN, HIGH);
  delay(10);

  digitalWrite(RINGER_POWERED_PIN, LOW);
  delay(100);

  digitalWrite(ringPhonePin, HIGH);
  delay(200);
}

static void processNumber(const char* phoneNumber) {
  printf("Processing number %s\n", phoneNumber);

  if (strcmp(phoneNumber, "22") == 0) {
    ring(RING_PHONE_1_PIN);
  } else if (strcmp(phoneNumber, "24") == 0) {
    ring(RING_PHONE_2_PIN);
  } else if (strcmp(phoneNumber, "26") == 0) {
    ring(RING_PHONE_3_PIN);
  } else if (strcmp(phoneNumber, "28") == 0) {
    ring(RING_PHONE_4_PIN);
  } else {
    printf("Unknown number %s\n", phoneNumber);
  }
}

int main() {
  wiringPiSetup();

  pinMode(RINGER_POWERED_PIN, OUTPUT);
  pinMode(RING_GENERATOR_INVERTED_PIN, OUTPUT);
  pinMode(RING_GENERATOR_PIN, OUTPUT);
  pinMode(RING_PHONE_1_PIN, OUTPUT);
  pinMode(RING_PHONE_2_PIN, OUTPUT);
  pinMode(RING_PHONE_3_PIN, OUTPUT);
  pinMode(RING_PHONE_4_PIN, OUTPUT);  

  digitalWrite(RINGER_POWERED_PIN, LOW);
  digitalWrite(RING_GENERATOR_INVERTED_PIN, LOW);
  digitalWrite(RING_GENERATOR_PIN, LOW);
  digitalWrite(RING_PHONE_1_PIN, LOW);
  digitalWrite(RING_PHONE_2_PIN, LOW);
  digitalWrite(RING_PHONE_3_PIN, LOW);
  digitalWrite(RING_PHONE_4_PIN, LOW);

  StateType state = IDLE;
  PhoneNumberRecognizer recognizer(LINE_SENSE_PIN);

  while (true) {
    auto result = recognizer.detect();
    switch (result) {
      case PhoneNumberRecognizer::COMPLETED:
        if (state == IDLE) {
          state = IN_CALL;
          processNumber(recognizer.number());
        }
        break;

      case PhoneNumberRecognizer::ON_HOOK:
        state = IDLE;
        break;

      default:
        break;
    }

    delay(1);
  }
  
  return 0;
}
