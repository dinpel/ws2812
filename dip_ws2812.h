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
  uint8_t T0H,T1H,T0L,T1L,CH_RMT_TX,GPIO; //static_cast<rmt_channel_t>(1), static_cast<gpio_num_t>(4)
void show(){
  uint32_t *table=(uint32_t *)ramtable,*dest=(uint32_t *)ramled; 
  rmt_item32_t rmt_tmp;
  *dest++=(((1<<1)|1)<<16)|(2000<<1); //reset 50 microseg.
  for (uint32_t led = 0;led<n_leds;led++) {  uint32_t be=*(table+led),m=0x800000; 
    while (m) { rmt_tmp = (be&m) ? (rmt_item32_t){{{T1H, 1, T1L, 0}}} : (rmt_item32_t){{{T0H, 1, T0L, 0}}}; *dest++=rmt_tmp.val; m>>=1;} }
  rmt_wait_tx_done(static_cast<rmt_channel_t>(CH_RMT_TX), portMAX_DELAY);
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
void clear(){ for(int i=0;i<n_leds;i++) led(i,0); show(); } // off all leds
void times(uint32_t nt0h,uint32_t nt0l,uint32_t nt1h,uint32_t nt1l){ T0H=nt0h/25; T0L=nt0l/25; T1H=nt1h/25;  T1L=nt1l/25; }
void rgb(uint8_t nl,uint8_t r,uint8_t g,uint8_t b,bool showf=false){ if (nl<n_leds) { *((uint32_t *)ramtable+nl)=g<<16|r<<8|b; if (showf) show(); } }
void led(uint8_t nl,uint32_t vl,bool showf=false){  if (nl<n_leds) rgb(nl,vl>>16,vl>>8,vl,showf); }
void begin(uint8_t leds,uint8_t can,uint8_t port)
{
  n_leds=leds;
  ramled=(uint32_t*)malloc((leds*24*4)+4); ramtable=(uint32_t*)malloc(leds*4);
  T0H=(400/25); T0L=(850/25); T1H=(800/25); T1L=(450/25);
  CH_RMT_TX=can;
  GPIO=port;
  rmt_config_t config = {
    .rmt_mode = RMT_MODE_TX,
    .channel = static_cast<rmt_channel_t>(CH_RMT_TX),
    .gpio_num = static_cast<gpio_num_t>(GPIO), 
    .mem_block_num = 3
  }; 
  config.clk_div = 2; // 80000000/2, 1 step=25nseg
  rmt_config(&config); rmt_driver_install(config.channel, 0, 0);
  clear(); show(); 
}
} ws2812b;
 
#endif