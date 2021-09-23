#include <stdio.h>
#include "recognizer.h"


PhoneNumberRecognizer::PhoneNumberRecognizer(int lineSensePin) : 
  _lineSensePin(lineSensePin),
  _currentDigit(0),
  _pulseCount(0),
  _timePinChanged(0),
  _previousPinValue(HIGH),
  _state(ON_HOOK) {

    pinMode(lineSensePin, INPUT);
}

PhoneNumberRecognizer::StateType PhoneNumberRecognizer::detect() {
  unsigned int now = millis();

  if (_timePinChanged == 0) {
    _timePinChanged = now;
    return _state;
  }

  int currentPinValue = digitalRead(_lineSensePin);

  if (currentPinValue != _previousPinValue) {
    if (now - _timePinChanged < DEBOUNCE_TIME_MILIS) {
      return _state;
    }
    
    _timePinChanged = now;
    _previousPinValue = currentPinValue;

#if 0
    printf("Input pin value changed from ");
    if (currentPinValue == HIGH) {
      printf("LOW to HIGH");
    } else {
      printf("HIGH to LOW");
    }
    //showTimeDifference ();
    printf("\n");    
#endif      
  }

  unsigned int timeSincePinChange = now - _timePinChanged;

  if (currentPinValue == HIGH) {
    handlePinHigh(timeSincePinChange);
  } else {
    handlePinLow(timeSincePinChange);
  }

  return _state;
}

void PhoneNumberRecognizer::handlePinHigh(const int& timeSincePinChange) {
  switch (_state) {

    case ON_HOOK:
      break;

    case OFF_HOOK:
      newState(DIALING);
      _currentDigit = 0;
      _pulseCount = 0;
      break;      

    case DIALING:
      if (timeSincePinChange >= ON_HOOK_TIME_MILIS) {
        newState(ON_HOOK);
      } else {
        newState(GOT_PULSE);
      }
      break;

    case GOT_PULSE:
    case COMPLETED:
      if (timeSincePinChange >= ON_HOOK_TIME_MILIS) {
        newState(ON_HOOK);
      }
      break;
      
    }  
  }

void PhoneNumberRecognizer::handlePinLow (const int& timeSincePinChange) {
  switch (_state) {

    case ON_HOOK:
      newState(OFF_HOOK);
      _currentDigit = 0;
      break;  

    case OFF_HOOK:
      break;

    case GOT_PULSE:
      ++_pulseCount;
      newState(DIALING);
      break;

    case DIALING:
      if (timeSincePinChange >= BETWEEN_DIGITS_TIME_MILIS && _pulseCount) {
        //printf("Finished a digit\n");
        //printf("Count = %d\n", _pulseCount);
        if (_currentDigit < NUMBER_OF_DIGITS) {
          if (_pulseCount >= 1 && _pulseCount <= 10) {
            if (_pulseCount == 10) {
              _pulseCount = 0;
            }
            _number[_currentDigit] = _pulseCount | '0';
            ++_currentDigit;
            _number[_currentDigit] = 0;
          }
        }

        if (_currentDigit == NUMBER_OF_DIGITS) {
          //printf("Number acquired! Got: %s\n", _number);
          newState(COMPLETED);
        }
          
        _pulseCount = 0;
      }
      break;
        
      case COMPLETED:
        break;

    }
  }

void PhoneNumberRecognizer::newState(const StateType& which) {
  if (_state == which) {
    return;
  }

#if 0
  printf("Changing state from ");
  showState(_state);
  printf(" to ");
  showState(which);

  //showTimeDifference();
  printf("\n");
#endif  
  
  _state = which;
}

void PhoneNumberRecognizer::showState (const StateType& which) {
  /*switch (which) {
    case ON_HOOK:   printf("ON_HOOK");   break;
    case OFF_HOOK:  printf("OFF_HOOK");  break;
    case DIALING:   printf("DIALING");   break;
    case GOT_PULSE: printf("GOT_PULSE"); break;
    case COMPLETED: printf("COMPLETED"); break;
    default:        printf("UNKNOWN");   break;
  }*/
}

