#include <stdio.h>
#include "recognizer.h"


int main() {
  wiringPiSetup();

  const int o1 = 4;
  pinMode(o1, INPUT);

  PhoneNumberRecognizer recognizer;

  while (true) {
    recognizer.detect();
    delay(1);
  }
  
  return 0;
}
