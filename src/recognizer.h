
#include <wiringPi.h>

class PhoneNumberRecognizer {
public:
    typedef enum { ON_HOOK, OFF_HOOK, DIALING, GOT_PULSE, COMPLETED  } StateType;

    PhoneNumberRecognizer(int lineSensePin);

    StateType detect();

    const char* number() const { return _number; }

private:
    static const int DEBOUNCE_TIME_MILIS = 5;
    static const int ON_HOOK_TIME_MILIS = 1000;
    static const int BETWEEN_DIGITS_TIME_MILIS = 500;
    static const int NUMBER_OF_DIGITS = 2;

    char _number[NUMBER_OF_DIGITS + 1];

    int _lineSensePin;
    int _currentDigit;
    int _pulseCount;
    unsigned int _timePinChanged;
    int _previousPinValue;

    StateType _state;

private:
    void handlePinHigh(const int& timeSincePinChange);
    void handlePinLow (const int& timeSincePinChange);
    void newState(const StateType& which);    
    void showState (const StateType& which);
};
