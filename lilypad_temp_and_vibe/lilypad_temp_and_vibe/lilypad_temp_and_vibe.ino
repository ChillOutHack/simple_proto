 const int tmp_pin = A2; //Analog pin for the TMP36
 const int vibe_pin = 11; //Digital pin for motor

void setup(){
  Serial.begin(9600); //Start the serial connection in order to print outputs to the serial monitor
  pinMode(vibe_pin, OUTPUT); //Assign pin 11 as output
}

void loop(){

  int tmp_voltage = analogRead(tmp_pin); //Get the voltage reading from the TMP36
  float voltage = tmp_voltage * (3.3 / 1024); //Convert the 0-1023 ADC reading to 0-5V
  float tempC = (voltage - 0.33) * 100; //Convert from 10mV per degree with 500 mV offset
  float tempF = (tempC * (9/5)) + 32; //Now convert to 'F

  if(tempF > 70.0){
    digitalWrite(vibe_pin, HIGH);
  }  
  else{
    digitalWrite(vibe_pin, LOW);
  }
  

  // Print voltage to serial
  Serial.print(voltage); Serial.print(" volts, ");
  // Print tempC to serial 
  Serial.print(tempC); Serial.print(" degrees C, ");
  // Print tempF to serial
  Serial.print(tempF); Serial.println(" degrees F");

  delay(1000);
}
