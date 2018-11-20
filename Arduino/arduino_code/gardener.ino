/*
Serial communication was inspired from
https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all

*/


#include <math.h>
//temperature
// baseline of temperature in the garden in Celsius
const int tempPin = A0;
const float temp_base = 28.5;

//alarm
const int buzzerPin = 8;

//light
const int lightPin = A2;
const int light_redled = 6;

//LEDs reacts to temperature
const int greenled = 2;
const int yellowled = 3;
const int redled = 4;

char val; // data received from processing are stored in val

void setup(){
  Serial.begin(9600);

    pinMode(greenled,OUTPUT);
    digitalWrite(greenled, LOW);
    
    pinMode(yellowled,OUTPUT);
    digitalWrite(yellowled, LOW);
    
    pinMode(redled,OUTPUT);
    digitalWrite(redled, LOW);

    pinMode(light_redled,OUTPUT);
    digitalWrite(light_redled, LOW);
}

//Use the Steinhart-Hart equation to convert to degrees C
//given function from moodle
double getTemperature(int rawADC) {
  rawADC += 220; // Modify the input value to calibrate the temperature.
  double temp;
  temp = log(((10240000/rawADC) - 10000));
  temp = 1 / (0.001129148 +
   (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
  return temp - 273.15;   //convert kelvin to celsius
}

  

int checkTemp(){
  int check = analogRead(tempPin);
  double temperature = getTemperature(check);
  
  if(temp_base - 0.25 <= temperature && temperature < temp_base + 1.5){
    return 0;//safe
  }
  else if (temp_base + 1.5 <= temperature && temperature < temp_base + 3.6){
    return 1;//warm
  }
  else if(temperature >= temp_base + 3.6){
    return 2;//hot
  }
  else{
    return 3;//cold
  }
  
}


//function for letting processing knows if red LEDs are on or not
//processing will then turn the buzzer on because it is dangerous for plants
String checkRedLed(){
  int light = analogRead(lightPin);
  
  if(checkTemp()==2 || checkTemp()==3 || 950 <= light){
    tone(buzzerPin, 700, 100);
    delay(120);
  }
}


void loop(){
  int temp_input = analogRead(tempPin);
  double temperature = getTemperature(temp_input);
  double tempmap = map(temperature, 25, 32, 30, 275);//modify the range to the graph size
  
  int light_value = analogRead(lightPin);
  double lightmap = map(light_value, 100, 1100, 315, 570);//modify the range to the graph size
  checkRedLed();
  
  //temperature check
    
    switch(checkTemp()){
      case 0:   //safe
        digitalWrite(greenled, HIGH);
        digitalWrite(yellowled, LOW);
        digitalWrite(redled, LOW);
        Serial.println(tempmap);
        delay(20);
        break;
    
      case 1:   //warm
        digitalWrite(greenled, LOW);
        digitalWrite(yellowled, HIGH);
        digitalWrite(redled, LOW);
        Serial.println(tempmap);
        delay(20);
        break;
    
      case 2:   //hot
        digitalWrite(greenled, LOW);
        digitalWrite(yellowled, LOW);
        digitalWrite(redled, HIGH);
        Serial.println(tempmap);
        delay(20);
        break;
    
      case 3:   //cold
        digitalWrite(greenled, HIGH);
        digitalWrite(yellowled, HIGH);
        digitalWrite(redled, HIGH);
        Serial.println(tempmap);
        delay(20);
        break;
    }

  //light check
  
    if (1 <= light_value && light_value < 950){
      digitalWrite(light_redled, LOW);    //safe light intensity
      Serial.println(lightmap);
      delay(20);
    }
    else if (950 <= light_value){
      digitalWrite(light_redled, HIGH);   //dangerous light intensity
      Serial.println(lightmap);
      delay(20);
    }
  
}
