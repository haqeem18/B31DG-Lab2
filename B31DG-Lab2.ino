#include <Ticker.h>

// Instantiate objects
Ticker Cycle;

// GPIOs for tasks
#define LED_1_Output 4
#define Signal_2_Input 1
#define Signal_3_Input 2

// Instantiate variables
unsigned long halflambda;
unsigned int freq;

void setup() 
{
  // Serial monitor
  Serial.begin(9600);

  // Set output pin
  pinMode(LED_1_Output, OUTPUT);
  pinMode(Signal_2_Input, INPUT);

  // Set ticker interval 
  Cycle.attach_ms(2000, Task3);
}

void loop() 
{
  
}

// Task 1 execution
// Period = 20ms, Duration = 280us
void Task1()
{
  Serial.println("Executing Task 1");

  digitalWrite(LED_1_Output, HIGH);
  delayMicroseconds(200);
  digitalWrite(LED_1_Output, LOW);
  delayMicroseconds(50);  
  digitalWrite(LED_1_Output, HIGH);
  delayMicroseconds(30);
  digitalWrite(LED_1_Output, LOW);
}

void Task2()
{
  // Measure the HIGH period of the square wave
  halflambda = pulseIn(Signal_2_Input, HIGH);
  
  // Calculating frequency of the signal
  freq = 1 / (2*halflambda*0.000001);

  // Print frequency of signal within 333Hz and 1000Hz
  if ((freq >= 333) && (freq <= 1000))
  {
    Serial.println(freq);
  }
}

void Task3()
{
  // Measure the HIGH period of the square wave
  halflambda = pulseIn(Signal_3_Input, HIGH);
  
  // Calculating frequency of the signal
  freq = 1 / (2*halflambda*0.000001);

  // Print frequency of signal within 500Hz and 1000Hz
  if ((freq >= 500) && (freq <= 1000))
  {
    Serial.println(freq);
  }
}