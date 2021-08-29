#include <stdio.h>
#include <cstring>
#include "recognizer.h"

typedef enum { IDLE, IN_CALL } StateType;

// 11, 12, 13, 14 = pins used for ring selectors
//
// Raspberry Pi Model B Rev 1
// 
// 11 -> 26 (header)
// 12 -> 19 (header)
// 13 -> 21 (header)
// 14 -> 23 (header)
static const int RING_PHONE_1_PIN = 11;
static const int RING_PHONE_2_PIN = 12;
static const int RING_PHONE_3_PIN = 13;
static const int RING_PHONE_4_PIN = 14;

// 6 -> 22 (header)
static const int LINE_SENSE_PIN = 6;

static void ring(int ringPhonePin) {
  digitalWrite(ringPhonePin, HIGH);
  delay(3000);
  digitalWrite(ringPhonePin, LOW);
}

static void processNumber(const char* phoneNumber) {
  printf("Processing number %s\n", phoneNumber);

  if (strcmp(phoneNumber, "22")) {
    ring(RING_PHONE_1_PIN);
  } else if (strcmp(phoneNumber, "24")) {
    ring(RING_PHONE_2_PIN);
  } else if (strcmp(phoneNumber, "26")) {
    ring(RING_PHONE_3_PIN);
  } else if (strcmp(phoneNumber, "28")) {
    ring(RING_PHONE_4_PIN);
  } else {
    printf("Unknown number %s\n", phoneNumber);
  }
}

int main() {
  wiringPiSetup();

  pinMode(RING_PHONE_1_PIN, OUTPUT);
  pinMode(RING_PHONE_2_PIN, OUTPUT);
  pinMode(RING_PHONE_3_PIN, OUTPUT);
  pinMode(RING_PHONE_4_PIN, OUTPUT);

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
