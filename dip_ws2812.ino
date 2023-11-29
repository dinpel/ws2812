 
#include "dip_ws2812.h"
WS2812 l12,l16; 

void setup(){
l16.begin(16,0,15);  
l12.begin(12,3,16);  
Serial.begin(115200);
}

void loop(){

l12.clear(0x330000); l16.clear(0xFF0000); delay(500); 
l12.clear(0x3300); l16.clear(0xFF00); delay(500); 
l12.clear(0x33); l16.clear(0xFF); delay(500); 

l12.clear(); for(int j=0;j<l12.n_leds;j+=3) l12.led(j,0x111100,true);
 
l16.led(0,0); 
l16.rgb(1,70,0,0); l16.rgb(2,70,0,0);  l16.rgb(3,70,0,0); //led position,red,green,blue 
l16.led(4,0); 
l16.rgb(5,0,50,0); l16.rgb(6,0,50,0);  l16.rgb(7,0,50,0); 
l16.led(8,0); 
l16.rgb(9,0,0,90); l16.rgb(10,0,0,90); l16.rgb(11,0,0,90); 
l16.led(12,0); 
l16.rgb(13,0x20,0x20,0); l16.rgb(14,0x20,0x20,0); l16.led(15,0x202000,true);  //last optional parameter showf = true for show
for(int j=0;j<2;j++){ 
  for(int i=0;i<50;i++){ delay(125); l16.rotate(1); l12.rotate(1); }
  for(int i=0;i<50;i++){ delay(40); l16.rotate(-1); l12.rotate(-1); }
}  
for(int i=0;i<2;i++){
  for(int j=0;j<l16.n_leds;j++){ l16.led(j,0xff0000,true); delay(100); l12.rotate(1); }  l16.clear(); delay(250);
  for(int j=l16.n_leds-1;j>=0;j--){ l16.led(j,0x0000ff,true); delay(100); l12.rotate(-1); } l16.clear(); delay(250);
}
for(int i=0;i<5;i++){
  l16.clear(0xFF0000); l12.clear(); delay(500); l16.clear(); l12.clear(0xFF0000); delay(500);
}

}
