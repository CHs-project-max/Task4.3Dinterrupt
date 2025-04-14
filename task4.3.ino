#include "DHT.h"
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
#define USING_TIMER_TC3 true  
#include <SAMD_ISR_Timer.h>
#include <SAMDTimerInterrupt.h>

#define SELECTED_TIMER      TIMER_TC3
#define TIMER_INTERVAL_1S 1000L 
#define TIMER_INTERVAL_500ms 500L 

#define DHTPIN  2
#define DHTTYPE DHT11
#define ECHO 8
#define TRIG 9
#define led1  5
#define led2  6
#define led3  7

#define Triggersensor1 3
#define Triggersensor2 4


SAMDTimer ITimer(TIMER_TC3);
SAMD_ISR_Timer ISR_Timer;


DHT dht(DHTPIN, DHTTYPE);
uint8_t led1_state = LOW;
uint8_t led2_state = LOW;
uint8_t led3_state = LOW;

double t;
long distance;

void TimerHandler() {
  // checks if any of the software timers have reached their scheduled 
  // interval and calls the associated callback function such as toggleBlueLed() 
  // that is scheduled to be called every 1000ms (and checked every 500ms)
  ISR_Timer.run();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(Triggersensor1, OUTPUT);
  pinMode(Triggersensor2, OUTPUT);
  pinMode(10, INPUT_PULLDOWN);
  pinMode(11, INPUT_PULLDOWN);
  pinMode(13, INPUT_PULLDOWN);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(13), toggle_led1 , RISING);
  attachInterrupt(digitalPinToInterrupt(11), toggle_led2 , RISING);
  attachInterrupt(digitalPinToInterrupt(10), LOG_message, RISING);
  ITimer.attachInterruptInterval_MS(TIMER_INTERVAL_500ms, TimerHandler);
  ISR_Timer.setInterval(TIMER_INTERVAL_500ms,  ReadHCS04);
  ISR_Timer.setInterval(TIMER_INTERVAL_1S,  toggle_led3);
  Serial.print(F("\nStarting TimerInterruptLEDDemo on ")); Serial.println(BOARD_NAME);
  Serial.println(SAMD_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));
}
void loop() {
  // put your main code here, to run repeatedly:;
  double t = dht.readTemperature();
  if (t >= 25)
  {
    digitalWrite(Triggersensor1, HIGH);
    delay(100);
    digitalWrite(Triggersensor1, LOW);
  }
  Serial.print(t);
  Serial.println(" C");
  delay(2000);
}

void ReadHCS04()
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  distance = pulseIn(ECHO, HIGH)*0.034/2;
  Serial.print(distance);
  Serial.println(" CM");
  if(distance < 20)
  {
    digitalWrite(Triggersensor2, HIGH);
    delay(100);
    digitalWrite(Triggersensor2, LOW);
  }
}
void toggle_led1()
{
  Serial.println("LED1 is toggled ");
  led1_state=!led1_state;
  digitalWrite(led1 , led1_state);
}

void toggle_led2()
{
  Serial.println("LED2 is toggled ");
  led2_state = !led2_state;
  digitalWrite(led2 , led2_state);
}

void toggle_led3()
{
  Serial.println("LED3 is toggled");
  led3_state = !led3_state;
  digitalWrite(led3, led3_state);
}

void LOG_message()
{
  Serial.println("This is a logging message");
}