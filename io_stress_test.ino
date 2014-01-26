/* io test
made by Spencer Neilan, 2014
spencer.neilan@gmail.com
For testing the IO on the sasquatch V2
Connections:


*/

#include <Servo.h> 

Servo servo[12];

void setup() 
{
  //attach servos to all PWM channels
  for(int i = 2; i<=13; i++)
  {
    servo[i-2].attach(i);
  } 
  
  analogWriteResolution(12);
  analogReadResolution(12);
  SerialUSB.begin(9600);
  
  //IO Load Test
  pinMode(23, OUTPUT);//connect through 220 ohm resistor and ampmeter. this is 15mA source pin
  digitalWrite(23, 1);
  
  pinMode(24, OUTPUT);//connect 3.3V through a 330 ohm resistor and ampmeter into pin 23.  this is 9mA  max sink pin
  digitalWrite(24, 0);
  
  pinMode(22, OUTPUT);//connect through 1k resistor and ampmeter. this is 3mA max source pin 
  digitalWrite(22, 1);
  
  pinMode(52, OUTPUT); //connect 3.3V going through a 680 ohm resistor and ampmeter. This is 6mA max sink pin. 
  digitalWrite(52, 0);
  
} 

void loop() 
{
  //PWM Test
  static int i=0;
  for( i = 0; i <= 180; i += random(0,1)) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    servo[random(0,11)].write(i);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for( i = 180; i>=0; i -=random(0,1))
  {     // goes from 180 degrees to 0 degrees 
    servo[random(0,11)].write(i);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  }
  
  
  // DAC Test
  //Split the DAC0 and DAC1 into the 3.3V output and 5V amplified output
  //Input a voltage with voltmeter into ADC7 and ADC8 to test.
  analogWrite(DAC0,analogRead(7)); //0-5V Measure-->[0-5V supply]-->[5 to 3.3V R divider]-->ADC-->DAC-->[3.3V to 5V Op Amp]-->0-5V Measure
  analogWrite(DAC1,analogRead(8)); //0-3.3V Measure-->[0-3.3V supply]-->ADC-->DAC-->0-3.3 Measure

  //ADC test
  SerialUSB.print("ADC values:  ");
  for(int adcpin = 1; adcpin<=11; adcpin++){
    int sensorValue = analogRead(adcpin);
      SerialUSB.print(adcpin);
      SerialUSB.print(": ");
      SerialUSB.print(sensorValue);
      SerialUSB.print(", ");
  }
  
  //IO Test
    //test bidirectional level shifter by plugging both sides of a button to pins 50 and 51 on the 5V side of a bidirectional level shifter
  SerialUSB.print("Button Fwd = ");
  SerialUSB.print(testBidirectionalButton(51,50));
  SerialUSB.print(". Button Rev = ");
  SerialUSB.print(testBidirectionalButton(50,51));
  SerialUSB.print(". ");


  SerialUSB.println();
  
} 

int testBidirectionalButton(int inputPin, int gndPin){
    pinMode(inputPin, INPUT_PULLUP);
    pinMode(gndPin, OUTPUT);
    digitalWrite(inputPin, 1);
    digitalWrite(gndPin,0);
    return(digitalRead(inputPin));
}


