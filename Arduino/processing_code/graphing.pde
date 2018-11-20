/*
<Reference>
1. Serial communication was inspired from
    https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all
2. Using arrays when receiving multiple information was inspired from
    https://www.dummies.com/computers/arduino/how-to-send-multiple-signals-from-the-arduino-to-processing/
*/

import processing.serial.*;
Serial myPort;
int[] serial_arr = new int[2];
int serial_cnt = 0;
int xPos=30;
//coordinates of previous dots
int lastxPos = 30;
int temp_lastheight = height/4;
int light_lastheight = 3*(height/4);

void setup()
{
  size(1000, 600);
  String arduino = Serial.list()[1];
  myPort = new Serial(this, arduino, 9600);
  background(100);
}

void draw()
{
  //layout of the graph
  stroke(0);
  strokeWeight(2);
  line(30, 30, 30, height/2 -15);
  line(30, height/2 - 15, width-30, height/2 - 15);
  line(30, height/2 + 15, 30, height-30);
  line(30, height-30, width-30, height-30);
  
  //  <Getting multiple information from Arduino at the same time using a list>
    String val_str = myPort.readStringUntil('\n');
    if(val_str != null){
      //getting a clean integer value
       val_str = trim(val_str);
       int val = int(val_str);
       
       serial_arr[serial_cnt] = val;
       serial_cnt++;
       //resetting serial count
       if(serial_cnt > 1){
         serial_cnt = 0;
       }
        
        int temperature_y = serial_arr[0];
        int light_y = serial_arr[1];
       
       //debugging
       println(temperature_y);
       println(light_y);
       
        //graph of temperture and light
        
        stroke(255,0,0);     //red line       
        line(lastxPos, temp_lastheight, xPos, height -15 - temperature_y); 
        lastxPos= xPos;
        temp_lastheight= int(height-temperature_y);
        
        stroke(0,255,0);     //green line        
        line(lastxPos, light_lastheight, xPos, height-15 - light_y); 
        lastxPos= xPos;
        light_lastheight= int(height-light_y);
        
        
        // clear the graph when xPos reaches the end
        if (xPos >= width-30) {
          //starting position
          xPos = 31;
          lastxPos= 30;
          //clearing
          background(100);  
          stroke(0);
          strokeWeight(2);
          line(30, 30, 30, height/2 -15);
          line(30, height/2 - 15, width-30, height/2 - 15);
          line(30, height/2 + 15, 30, height-30);
          line(30, height-30, width-30, height-30);
        } 
        else {
          xPos++;
        }
        
      //for debugging purpose
      //println("zero");
      //println(serial_arr[0]);
      //println("one");
      //println(serial_arr[1]);
    }
  
}
