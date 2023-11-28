 
#include "dip_ws2812.h"

void setup(){
ws2812b.begin(16,0,4); //number leds,channel rmt, gpio
}
void loop()
{
ws2812b.led(0,0); 
ws2812b.rgb(1,70,0,0); ws2812b.rgb(2,70,0,0);  ws2812b.rgb(3,70,0,0); //led position,red,green,blue 
ws2812b.led(4,0); 
ws2812b.rgb(5,0,50,0); ws2812b.rgb(6,0,50,0);  ws2812b.rgb(7,0,50,0); 
ws2812b.led(8,0); 
ws2812b.rgb(9,0,0,90); ws2812b.rgb(10,0,0,90); ws2812b.rgb(11,0,0,90); 
ws2812b.led(12,0); 
ws2812b.rgb(13,0x20,0x20,0); ws2812b.rgb(14,0x20,0x20,0); ws2812b.led(15,0x202000,true);  //last optional parameter showf = true for show
for(int j=0;j<2;j++){ 
  for(int i=0;i<50;i++){ delay(125); ws2812b.rotate(1); }
  for(int i=0;i<50;i++){ delay(40); ws2812b.rotate(-1); }
}  
for(int i=0;i<2;i++){
  for(int j=0;j<ws2812b.n_leds;j++){ ws2812b.led(j,0xff0000,true); delay(100); }  ws2812b.clear(); delay(250);
  for(int j=ws2812b.n_leds-1;j>=0;j--){ ws2812b.led(j,0x0000ff,true); delay(100); } ws2812b.clear(); delay(250);
}
for(int i=0;i<20;i++){
  ws2812b.clear(0xff00); delay(50); ws2812b.clear(); delay(50);
}

}
