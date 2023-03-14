#include <Ticker.h>
#include <B31DGMonitor.h>

// Instantiate objects
Ticker Cycle;
B31DGCyclicExecutiveMonitor Monitor;

// GPIOs for tasks
#define LED_1_Output 4
#define Signal_2_Input 1
#define Signal_3_Input 2
#define Analog_4_Input 3
#define LED_4_Output 5

// Instantiate variables
/* CE */
int CE_Counter = 0;

/* Task 2 & 3*/
unsigned long halflambda;
unsigned int freq;

/* Task 4*/
int ADC;
float volts [4];
int v = 0;
float avg;

/* Task 5*/
int Frequency1;
int Frequency2;
char Task5_Output[5];

/* Task timer */
int starttime;

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

  // Cyclic Executive Monitor
  Monitor.startMonitoring();
  // Set ticker interval 
  CyclicExecutive();
  Cycle.attach_ms(4, CyclicExecutive);
  // Cycle.attach_ms(1, Task1);
}

void loop()
{
}

// Task 1 execution
// Period = 20ms, Duration = 280us
void Task1()
{
  // Measure Task start time
  // starttime = micros();

  Monitor.jobStarted(1);

  // Serial.println("Task 1");

  digitalWrite(LED_1_Output, HIGH);
  delayMicroseconds(200);
  digitalWrite(LED_1_Output, LOW);
  delayMicroseconds(50);  
  digitalWrite(LED_1_Output, HIGH);
  delayMicroseconds(30);
  digitalWrite(LED_1_Output, LOW);

  Monitor.jobEnded(1);

  // Measure Task end time
  // Serial.print("Task 1 duration is ");
  // Serial.println(micros() - starttime);
}

void Task2()
{
  Monitor.jobStarted(2);

  // Measure Task start time
  // starttime = micros();

  // Serial.println("Task 2");

  // Measure the HIGH period of the square wave
  halflambda = pulseIn(Signal_2_Input, HIGH, 1600);
  
  // Calculating frequency of the signal
  freq = 1 / (2*halflambda*0.000001);

  // Print frequency of signal within 333Hz and 1000Hz
  if ((freq >= 333) && (freq <= 1000))
  {
    Frequency2 = freq;
  }

  // Measure Task end time
  // int endtime = micros();
  // int time = endtime - starttime;
  //Serial.print("Task duration is ");
  // Serial.println(micros() - starttime);

  Monitor.jobEnded(2);
}

void Task3()
{
  Monitor.jobStarted(3);

  // Measure Task start time
  // starttime = micros();

  // Serial.println("Task 3");

  // Measure the HIGH period of the square wave
  halflambda = pulseIn(Signal_3_Input, HIGH, 1100);
  
  // Calculating frequency of the signal
  freq = 1 / (2*halflambda*0.000001);

  // Print frequency of signal within 500Hz and 1000Hz
  if ((freq >= 500) && (freq <= 1000))
  {
    Frequency2 = freq;
  }

  // Measure Task end time
  // int endtime = micros();
  // int time = endtime - starttime;
  //Serial.print("Task duration is ");
  // Serial.println(micros() - starttime);
  
  Monitor.jobEnded(3);
}

void Task4()
{
  Monitor.jobStarted(4);

  // Measure Task start time
  // starttime = micros();

  // Serial.println("Task 4");

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
  //Serial.print("Average is ");
  //Serial.println(avg);

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
  // int endtime = micros();
  // int time = endtime - starttime;
  //Serial.print("Task duration is ");
  // Serial.println(micros() - starttime);
  
  Monitor.jobEnded(4);
}

void Task5()
{
  Monitor.jobStarted(5);

  // Measure Task start time
  // starttime = micros();

  // Serial.println("Task 5");

  // Convert the frequnecy in percentage
  Frequency1 =  (100 *(Frequency1 - 333) / (1000-333));
  Frequency2 =  (100 *(Frequency2 - 500) / (1000-500));

  // Print Frequency1 & Frequency2
  sprintf(Task5_Output, "%d,%d", Frequency1, Frequency2);
  Serial.println(Task5_Output);

  // Measure Task end time
  // Serial.println(micros() - starttime);

  
  Monitor.jobEnded(5);
}

void CyclicExecutive()
{
  Task1();
  if(CE_Counter % 5 == 0)
  {
    Task2();
    Task4();
  }
  if(CE_Counter % 2 == 0)
  {
    Task3();
  }
  if(CE_Counter % 25 == 0)
  {
    Task5();
  }
  CE_Counter++;
}