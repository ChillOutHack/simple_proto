 const int tmp_pin = A5; //Analog pin for the TMP36
 const int vibe_pin = 11; //Digital pin for motor
 const int num_readings = 10; //Number of readings to average for smoothing

int readings[num_readings]; //Define the array and initial values for all variables
int index = 0;
int total = 0;
int average = 0;

void setup(){
  Serial.begin(9600); //Start the serial connection in order to print outputs to the serial monitor
  pinMode(vibe_pin, OUTPUT); //Assign pin 11 as output
  // This initializes the array for temperature smoothing
  for (int this_reading = 0; this_reading < num_readings; this_reading++)
    readings[this_reading] = 0;
}

void loop(){

  int tmp_voltage = analogRead(tmp_pin); //Get the voltage reading from the TMP36
  float voltage = tmp_voltage * (3.3 / 1024); //Convert the 0-1023 ADC reading to 0-5V
  float tempC = (voltage - 0.33) * 100; //Convert from 10mV per degree with 500 mV offset
  float tempF = (tempC * (9/5)) + 32; //Now convert to 'F

  total = total - readings[index]; //Subtracts the current index value from the total
  readings[index] = tempF; //Assigns the current tempF reading to current index
  total = total + readings[index]; //Adds current tempF reading to total
  index = index + 1; //Increment the index by one

  if (index >= num_readings) //Resets index when it gets to highest value of the array
    index = 0;

  average = total / num_readings; //Divides total temp by number of readings

  //If loop to turn on motor above a certain temp
  if(average > 70.0){
    digitalWrite(vibe_pin, HIGH);
  }  
  else{
    digitalWrite(vibe_pin, LOW);
  }
  

  // Print voltage to serial
  //Serial.print(voltage); Serial.print(" volts, ");
  // Print tempC to serial 
  //Serial.print(tempC); Serial.print(" degrees C, ");
  // Print tempF to serial
  Serial.print(tempF); Serial.println(" degrees F");
  // Print average tempF
  Serial.print(average); Serial.println(" average");

  delay(1000);
}
