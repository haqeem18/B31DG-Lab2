#include <Ticker.h>

// LED pin for Task 1
#define LED_1_Output 4

Ticker Cycle;

void setup() 
{
  // Serial monitor
  Serial.begin(9600);

  // Set output pin
  pinMode(LED_1_Output, OUTPUT);

  // Set ticker interval 
  Cycle.attach_ms(1000, Task1); // Testing ticker to perform Task1 every 1s
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