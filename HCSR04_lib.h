#ifndef ULTRASONIC_SENSOR_LIB
    #define ULTRASONIC_SENSOR_LIB
    #define TRIGGER  PD2
    #define ECHO     PD3
    #define ECHO_PIN PIND

    void pulseTriggerPin();
    void startTimer1();
    void stopTimer1();
    void iniPCISR();
#endif
