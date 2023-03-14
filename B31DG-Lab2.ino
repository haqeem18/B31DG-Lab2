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
int offset = 0;

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

/* Task timer */
int starttime;
int tasktime;

void setup() 
{
  // Cyclic Executive Monitor
  Monitor.startMonitoring();

  // Serial monitor
  Serial.begin(9600);

  // Setup GPIOs
  pinMode(LED_1_Output, OUTPUT);
  pinMode(Signal_2_Input, INPUT);
  pinMode(Signal_3_Input, INPUT);
  pinMode(Analog_4_Input, INPUT);
  pinMode(LED_4_Output, OUTPUT);

  // Set ticker interval 
  Cycle.attach_ms(1, CyclicExecutive);
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
  // int starttime = micros();

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
  // tasktime = micros() - starttime;
  // Serial.print("Task 1 duration is ");
  // Serial.println(tasktime);
}

void Task2()
{
  Monitor.jobStarted(2);

  // Measure Task start time
  // int starttime = micros();

  // Serial.println("Task 2");

  // Measure the HIGH period of the square wave
  halflambda = pulseIn(Signal_2_Input, HIGH, 1500);
  
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
  //Serial.println(time);

  Monitor.jobEnded(2);
}

void Task3()
{
  Monitor.jobStarted(3);

  // Measure Task start time
  // int starttime = micros();

  // Serial.println("Task 3");

  // Measure the HIGH period of the square wave
  halflambda = pulseIn(Signal_3_Input, HIGH, 1500);
  
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
  //Serial.println(time);
  
  Monitor.jobEnded(3);
}

void Task4()
{
  Monitor.jobStarted(4);

  // Measure Task start time
  // int starttime = micros();

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
  //Serial.println(time);
  
  Monitor.jobEnded(4);
}

void Task5()
{
  Monitor.jobStarted(5);

  // Measure Task start time
  // int starttime = micros();

  // Serial.println("Task 5");

  // Convert the frequnecy in percentage
  Frequency1 = 100 * (Frequency1 / (1000-333));
  Frequency2 = 100 * (Frequency2 / (1000-500));

  // Print Frequency1 & Frequency2
  // Serial.print("Frequency 1 is ");
  // Serial.println(Frequency1);
  // Serial.print("Frequency 2 is ");
  // Serial.println(Frequency2);

  // Measure Task end time
  // int endtime = micros();
  // int time = endtime - starttime;
  //Serial.print("Task duration is ");
  //Serial.println(time);

  
  Monitor.jobEnded(5);
}

void CyclicExecutive()
{
  // Performing Task 1
  if ((CE_Counter % 16) == 0 || (CE_Counter % 16) == 5 || (CE_Counter % 16) == 10 || (CE_Counter % 16) == 15)
  {
    Task1();
  }
  
  // Performing Task 2
  if ((CE_Counter % 80) == 6 || (CE_Counter % 80) == 22 || (CE_Counter % 80) == 54 || (CE_Counter % 80) == 70)
  {
    Task2();
  }
  
  // Performing Task 3
  if ((CE_Counter % 16) == 1 || (CE_Counter % 16) == 11)
  {
    Task3();
  }
  
  // Performing Task 4
  if ((CE_Counter % 21)== 0)
  {
    Task4();
  }
  
  // Performing Task 5
  switch (CE_Counter)
  {
    case 0:
      Task5();
      break;

    case 160:
      Task5();
      break;
  }

  // Reset cycle counter (hyperperiod)
  CE_Counter++;
  if (CE_Counter == 200)
  {
    CE_Counter = 0;
  } 
}