#include <Ticker.h>

// Instantiate objects
Ticker Cycle;

// GPIOs for tasks
#define LED_1_Output 4
#define Signal_2_Input 1
#define Signal_3_Input 2
#define Analog_4_Input 3
#define LED_4_Output 5

// Instantiate variables
/* Task 2 & 3*/
unsigned long halflambda;
unsigned int freq;

/* Task 4*/
int ADC;
float volts [4];
int v = 0;
float avg;

/* Task 5*/
float Frequency1;
float Frequency2;

void setup() 
{
  // Serial monitor
  Serial.begin(9600);

  // Setup GPIOs
  pinMode(LED_1_Output, OUTPUT);
  pinMode(Signal_2_Input, INPUT);
  pinMode(Signal_3_Input, INPUT);
  pinMode(Analog_4_Input, INPUT);
  pinMode(LED_4_Output, OUTPUT);

  // Set ticker interval 
  // Cycle.attach_ms(2000, Task3);
  Cycle.attach_ms(2000, Task5);
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
  // Measure Task start time
  int starttime = micros();

  // Measure the HIGH period of the square wave
  halflambda = pulseIn(Signal_2_Input, HIGH);
  
  // Calculating frequency of the signal
  freq = 1 / (2*halflambda*0.000001);

  // Print frequency of signal within 333Hz and 1000Hz
  if ((freq >= 333) && (freq <= 1000))
  {
    Frequency2 = freq;
  }

  // Measure Task end time
  int endtime = micros();
  int time = endtime - starttime;
  Serial.print("Task duration is ");
  Serial.println(time);
}

void Task3()
{
  // Measure Task start time
  int starttime = micros();

  // Measure the HIGH period of the square wave
  halflambda = pulseIn(Signal_3_Input, HIGH);
  
  // Calculating frequency of the signal
  freq = 1 / (2*halflambda*0.000001);

  // Print frequency of signal within 500Hz and 1000Hz
  if ((freq >= 500) && (freq <= 1000))
  {
    Frequency2 = freq;
  }

  // Measure Task end time
  int endtime = micros();
  int time = endtime - starttime;
  Serial.print("Task duration is ");
  Serial.println(time);
}

void Task4()
{
  // Measure Task start time
  int starttime = micros();

  // Point array back to start
  if (v == 4)
  {
    v = 0;
  }

  // Take ADC value of analogue signal
  volts[v] = analogRead(Analog_4_Input) * 0.000805664;
  v++;

  // Sum the last 4 readings
  for (int i = 0; i < 4; i++)
  {
    avg += volts[i]; 
  }

  // Calculating the average
  avg = avg / 4;
  Serial.print("Average is ");
  Serial.println(avg);

  // LED error when average is greater than half of maximum range
  if (avg > 1.65)
  {
    digitalWrite(LED_4_Output, HIGH);
  }
  else
  {
    digitalWrite(LED_4_Output, LOW);
  }

  // Measure Task end time
  int endtime = micros();
  int time = endtime - starttime;
  Serial.print("Task duration is ");
  Serial.println(time);
}

void Task5()
{
  // Measure Task start time
  int starttime = micros();

  // Convert the frequnecy in percentage
  Frequency1 = 100 * (Frequency1 / (1000-333));
  Frequency2 = 100 * (Frequency2 / (1000-500));

  // Print Frequency1 & Frequency2
  Serial.print("Frequency 1 is ");
  Serial.println(Frequency1);
  Serial.print("Frequency 2 is ");
  Serial.println(Frequency2);

  // Measure Task end time
  int endtime = micros();
  int time = endtime - starttime;
  Serial.print("Task duration is ");
  Serial.println(time);
}