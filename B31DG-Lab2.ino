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
int CE_Counter = 0;         // Scheduling counter (every 4ms with ticker)

/* Task 2*/
bool state2;                // Check the state of signal at time of pulsein()
unsigned long halflambda2;  // Length of HIGH or LOW state (half of wave cycle)
unsigned int freq2;         // Frequency of wave

/* Task 3 */
bool state3;                // Check the state of signal at time of pulsein()
unsigned long halflambda3;  // Length of HIGH or LOW state (half of wave cycle)
unsigned int freq3;         // Frequency of wave

/* Task 4*/
int ADC;                    // Value from ADC
float volts [4];            // Array to store ADC values
int v = 0;                  // Pointer for the array  
float avg;                  // Average value of ADC

/* Task 5*/
int Frequency2;             // Frequency of Task 2 in percent
int Frequency3;             // Frequency of Task 3 in percent
char Task5_Output[5];       // String buffer to output %d,%d from Task 2 & Task 3

/* Task timer */
int starttime;              // Measuring start time of each tasks

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

  // Run Cyclic Executive schedule every 4ms
  CyclicExecutive();
  Cycle.attach_ms(4, CyclicExecutive);
}

void loop()
{
}

// Task 1 execution
// Period = 20ms, Duration = 300us
void Task1()
{
  // Measure Task start time (uncomment below to use)
  /* starttime = micros(); */

  // Cyclic Executive monitor start
  Monitor.jobStarted(1);

  // Generate signal
  digitalWrite(LED_1_Output, HIGH);
  delayMicroseconds(200);
  digitalWrite(LED_1_Output, LOW);
  delayMicroseconds(50);  
  digitalWrite(LED_1_Output, HIGH);
  delayMicroseconds(30);
  digitalWrite(LED_1_Output, LOW);

  // Cyclied Executive monitor end
  Monitor.jobEnded(1);

  // Measure Task duration (incomment below to use)
  /* Serial.print("Task 1 duration is ");
  Serial.println(micros() - starttime); */
}

void Task2()
{
  // Measure Task start time (uncomment below to use)
  /* starttime = micros(); */

  // Cyclic Executive monitor start
  Monitor.jobStarted(2);

  // Measure the HIGH period of the square wave
  if (digitalRead(Signal_2_Input) == HIGH)
  {
    state2 = LOW;   // Set pulseIn to measure signal at LOW when signal is currently HIGH
  }
  else
  {
    state2 = HIGH;  // Set pulseIn to measure signal at High when signal is currently LOW
  }
  halflambda2 = pulseIn(Signal_2_Input, state2, 3000);  // Measure period of HIGH or LOW
  
  // Calculating frequency of the signal
  freq2 = 1 / (2*halflambda2*0.000001);

  // Print frequency of signal within 333Hz and 1000Hz
  if ((freq2 >= 333) && (freq2 <= 1000))
  {
    Frequency2 = freq2;
  }
  else
  {
    Frequency2 = 333;
  }

  // Cyclied Executive monitor end
  Monitor.jobEnded(2);

  // Measure Task duration (uncomment below to use)
  /* Serial.print("Task 2 duration is ");
  Serial.println(micros() - starttime); */
}

void Task3()
{
  // Measure Task start time (uncomment below to use)
  /* starttime = micros(); */

  // Cyclic Executive monitor start
  Monitor.jobStarted(3);

  // Measure the HIGH period of the square wave
  if (digitalRead(Signal_3_Input) == HIGH)
  {
    state3 = LOW;   // Set pulseIn to measure signal at LOW when signal is currently HIGH
  }
  else
  {
    state3 = HIGH;  // Set pulseIn to measure signal at High when signal is currently LOW
  }
  halflambda3 = pulseIn(Signal_3_Input, state3, 2000);
  
  // Calculating frequency of the signal
  freq3 = 1 / (2*halflambda3*0.000001);

  // Print frequency of signal within 500Hz and 1000Hz
  if ((freq3 >= 500) && (freq3 <= 1000))
  {
    Frequency3 = freq3;
  }
  else
  {
    Frequency3 = 500;
  }

  // Cyclied Executive monitor end
  Monitor.jobEnded(3);

  // Measure Task duration (uncomment below to use)
  /* Serial.print("Task 3 duration is ");
  Serial.println(micros() - starttime); */
}

void Task4()
{
  // Measure Task start time (uncomment below to use)
  /* starttime = micros(); */

  // Cyclic Executive monitor start
  Monitor.jobStarted(4);

  // Point array back to start (0)
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

  // LED error when average is greater than half of maximum range
  if (avg > 1.65)
  {
    digitalWrite(LED_4_Output, HIGH);
  }
  else
  {
    digitalWrite(LED_4_Output, LOW);
  }

  // Cyclied Executive monitor end
  Monitor.jobEnded(4);

  // Measure Task duration (uncomment below to use)
  /* Serial.print("Task 4 duration is ");
  Serial.println(micros() - starttime); */
}

void Task5()
{
  // Measure Task start time (uncomment below to use)
  /* starttime = micros(); */

  // Cyclic Executive monitor start
  Monitor.jobStarted(5);

  // Convert the frequnecy in percentage
  Frequency2 =  (100 *(Frequency2 - 333) / (1000-333));
  Frequency3 =  (100 *(Frequency3 - 500) / (1000-500));

  // Print Frequency1 & Frequency2
  sprintf(Task5_Output, "%d,%d", Frequency2, Frequency3);
  Serial.println(Task5_Output);

  // Cyclied Executive monitor end
  Monitor.jobEnded(5);

  // Measure Task duration (uncomment below to use)
  /* Serial.print("Task 5 duration is ");
  Serial.println(micros() - starttime); */
}

void CyclicExecutive()
{
  // Task 1 schedule
  Task1();

  // Task 2 schedule
  if((CE_Counter % 10) == 1 || (CE_Counter % 10) == 5)
  {
    Task2();
  }

  // Task 3 schedule
  if(CE_Counter % 2 == 0)
  {
    Task3();
  }

  // Task 4 schedule
  if(CE_Counter % 5 == 0)
  {
    Task4();
  }
  
  // Task 5 schedule
  if(CE_Counter % 25 == 0)
  {
    Task5();
  }
  CE_Counter++;
}