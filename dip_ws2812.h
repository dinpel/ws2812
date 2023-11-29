 /*
  driver to controller strip of leds type ws2812 (Rev.2 28/11/2023)
  create by dinpel@hotmail.com
  when include dip_ws2812.h, create object named ws2812b
  
  in setup() you have initcialize with 
  ws2812b.begin(number_leds,channel_rmt,gpio)
  
  in loop() 
  ws2812b.rgb(led_position,red,green,blue,showf)  or
  ws2812b.led(led_position,0xRRGGBB,showf)
  showf= false or missing for no send , true for iluminate 

  can rotate all ws2812b.rotate(steps)  or
  rotate reverse ws2812b.rotate(-steps)
  clear off all leds 
 */
#ifndef dip_ws2812_h
#define dip_ws2812_h

#include "driver/rmt.h"
 
class WS2812 {
private:
  const uint32_t *ramled,*ramtable;
  uint32_t RESET,ZERO,ONE;
  uint8_t CH_RMT_TX,GPIO; //static_cast<rmt_channel_t>(1), static_cast<gpio_num_t>(4)
void show(){
  rmt_wait_tx_done(static_cast<rmt_channel_t>(CH_RMT_TX), portMAX_DELAY);
  uint32_t *table=(uint32_t *)ramtable,*dest=(uint32_t *)ramled; 
  *dest++=RESET; for (uint32_t be,m,i=0;i<n_leds;i++) {  //reset 50 microseg.
    be=*table++; m=0x800000;  while (m) { *dest++=(be&m)?ONE:ZERO; m>>=1;} 
  }
  rmt_write_items(static_cast<rmt_channel_t>(CH_RMT_TX),(const rmt_item32_t*)ramled,((n_leds*24)+1), false);
}
public:
  WS2812(){}
  ~WS2812(){ if (ramled) free((void *)ramled); if (ramtable) free((void *)ramtable); }
  uint8_t n_leds; 
void rotate(int8_t pasos=1){
  if (pasos<0) pasos=n_leds+pasos; if (pasos<0 || pasos>=n_leds) pasos=1;
  for(int j=0;j<pasos;j++) { uint32_t *m=(uint32_t *)ramtable,pe=*m;for(int i=0;i<n_leds-1;i++){ *(m+i)=*(m+i+1); } *(m+n_leds-1)=pe;}
  show();
}
void clear(uint32_t vl=0){ for(int i=0;i<n_leds;i++) rgb(i,(vl&0xff0000)>>16,(vl&0xff00)>>8,vl); show(); } // off all leds
void times(uint32_t t0h,uint32_t t0l,uint32_t t1h,uint32_t t1l){ 
  RESET=(rmt_item32_t){1,1,2000, 0}.val; 
  ZERO=(rmt_item32_t){t0h/25,1,t0l/25,0}.val; ONE=(rmt_item32_t){t1h/25,1,t1l/25,0}.val; 
  }
void rgb(uint8_t nl,uint8_t r,uint8_t g,uint8_t b,bool showf=false){ if (nl<n_leds) { *((uint32_t *)ramtable+nl)=g<<16|r<<8|b; if (showf) show(); } }
void led(uint8_t nl,uint32_t vl,bool showf=false){  if (nl<n_leds) rgb(nl,vl>>16,vl>>8,vl,showf); }
void begin(uint8_t leds,uint8_t can,uint8_t port){
  n_leds=leds; ramled=(uint32_t*)malloc((leds*24*4)+4); ramtable=(uint32_t*)malloc(leds*4);
  times(400,850,800,450); CH_RMT_TX=can; GPIO=port;
  rmt_config_t config = {.rmt_mode = RMT_MODE_TX,.channel = static_cast<rmt_channel_t>(CH_RMT_TX),.gpio_num = static_cast<gpio_num_t>(GPIO),.clk_div=2,.mem_block_num=3 }; // clk_div 80Mhz/2,step=25nseg  
  rmt_config(&config); rmt_driver_install(config.channel, 0, 0); clear(); 
}
};
 
#endif