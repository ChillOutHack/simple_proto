int peltier = 3; //The N-Channel MOSFET is on digital pin 3
int power = 0; //Power level fro 0 to 99%
int peltier_level = map(power, 0, 99, 0, 255); //This is a value from 0 to 255 that actually controls the MOSFET
int const pot_pin = A0; //The pot input is Analog 0
int pot_val; // This is the value of the pot, with a range of 0 - 1023

void setup(){
Serial.begin(9600);

//pinMode(peltier, OUTPUT);
}

void loop(){
char option;

pot_val = analogRead(pot_pin);
power = map(pot_val, 0, 1023, 0, 99);
peltier_level = map(power, 0, 99, 0, 255);

Serial.print("Power=");
Serial.print(power);
Serial.print(" PLevel=");
Serial.print(peltier_level);
Serial.print("pot_val=");
Serial.println(pot_val);

analogWrite(peltier, peltier_level); //Write this new value out to the port

}
