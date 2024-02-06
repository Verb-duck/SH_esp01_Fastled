#include <Arduino.h>
#include "button_remote_control.h"

// ***************************** НАСТРОЙКИ *****************************
 // ----- настройки параметров
  #define DEBUGING  1
  #define VERSION 1.00

 // ----- пины подключения
  #define LED_PIN 4              // пин DI светодиодной ленты
  #define IR_PIN 3                // пин ИК приёмника
 //-------переменные
  boolean an_action = true;
  boolean ir_flag;

 // ----- настройки ленты
  #include <FastLED.h>
  //-----leds_contur---
  #define LED_NUM 190        // количество светодиодов (данная версия поддерживает до 410 штук)
  CRGB leds[LED_NUM];
    //----для будильника------
  #define COOLING  70    //длина пламени  
  #define SPARKING 150   //искры
  bool gReverseDirection = false;
  #define FRAMES_PER_SECOND 60
  #define TIME_SUNRISE 40              //время включения рассвета до будильника
  const int time_sunrise = TIME_SUNRISE * 60000 / 256; // вычесление периода добавления яркости рассвета

  byte brightness = 10;      // яркость по умолчанию (0 - 255)
  uint8_t red_color_now = 30;
  uint8_t green_color_now = 180;
  uint8_t blue_color_now = 255;
  
  uint8_t gHue = 0; // rotating "base color" used by many of the patterns
 //--color temperature pattern
  #define TEMPERATURE_1 Tungsten100W
  #define TEMPERATURE_2 OvercastSky
  // How many seconds to show each temperature before switching
  #define DISPLAYTIME 20

  #include <IRremote.h>
  IRrecv IrRemote(IR_PIN);
  #include <EEPROM_memory.h>

 

 //-------макросы--------- 
  #define PERIOD(x) \
    static uint32_t tmr; \
    bool flag = millis() - tmr >= (x); \
    if (flag) tmr += (x); \
    if (flag)

  #if (DEBUGING == 1)
  #define PRINT(title, y) \
    Serial.print(title); \
    Serial.print(": "); \
    Serial.println(y);
  #else
  #define PRINT(titly, y)
  #endif
 //enum 
  enum MODE_LIGHT_BEDROM {      //нулевой не отправляет по сериал, NULL_LIGHT как заглушка
    NULL_LIGHT,
    OFF_LIGHT,            //выключен свет
    START_LIGHT,          //эффект при включении
    COLOR_LIGHT,         //просто цвет
    BLACKOUT_LIGHT,       //затемнение
    NIGHT_LIGHT,          //ночной режим
    SUNRISE_LIGHT,        //рассвет
    SUNSET_LIGHT,         //закат 
    MULTI_COLOR_LIGHT,          //эффекты
  };
  MODE_LIGHT_BEDROM mode_light_bedroom(OFF_LIGHT);
    void Cylon(); void rainbow(); void rainbow_With_Glitter();
    void confetti(); void animation();
  void (*multi_color_light)() = Cylon;
  
 // ----- настройки радуги 
  float RAINBOW_STEP = 5.00;         // шаг изменения цвета радуги

 // ----- отрисовка

  /*Цвета для HSV    
    HUE_RED
    HUE_ORANGE
    HUE_YELLOW
    HUE_GREEN
    HUE_AQUA
    HUE_BLUE
    HUE_PURPLE
    HUE_PINK
  */
  void full_paint (const int color , const int saturation, const int hue );
  void start_paint (const int color, const int saturation, const int hue, MODE_LIGHT_BEDROM next );
  void setBrightness(); 
  void remoteTick();

void setup() {
  Serial.begin(115200);
  Serial.print("Version: "); Serial.println(VERSION);
  
 //---------лента 2811------- 
  FastLED.addLeds<WS2811, LED_PIN, BRG>(leds, LED_NUM).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness);
  FastLED.clear();
  FastLED.show(); 

  IrRemote.enableIRIn();
}

void loop() {
  remoteTick();     // опрос ИК пульта
  animation();       // главный цикл обработки и отрисовки
}
 
void animation() {
  setBrightness();  
  static uint32_t timer_waiting;
  static bool flag_one_start = true; 
  switch (mode_light_bedroom) {         
  case (COLOR_LIGHT) :
    if(an_action) 
      full_paint (red_color_now, green_color_now, blue_color_now);
    break;
  case (START_LIGHT) :
    if (flag_one_start)  {          //задержка на включение света
      flag_one_start = false;
      timer_waiting = millis(); 
    }
    if (millis() - timer_waiting >= 1000) 
      start_paint (red_color_now, green_color_now, blue_color_now, COLOR_LIGHT);
    break;
  
  case (SUNRISE_LIGHT) :
    if (brightness < 255)         //плавный рассвет
      {PERIOD(time_sunrise) {      
        brightness++; 
        FastLED.setBrightness(brightness);
    }} 
    {PERIOD (20) {                //эффект искр
      static byte heat[LED_NUM];
    // Step 1.  Cool down every cell a little
      for( int i = 0; i < LED_NUM; i++) {
        heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / LED_NUM) + 2));
      }        
      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
      for( int k= LED_NUM - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
      }          
      // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
      if( random8() < SPARKING ) {
        int y = random8(7);
        heat[y] = qadd8( heat[y], random8(160,255) );
      }
      // Step 4.  Map from heat cells to LED colors
      for( int j = 0; j < LED_NUM; j++) {
        CRGB color = HeatColor( heat[j]);
        int pixelnumber;
        if( gReverseDirection ) {
          pixelnumber = (LED_NUM-1) - j;
        } else {
          pixelnumber = j;
        }
        leds[pixelnumber] = color;
      }
    }}
    break;
    
  case (SUNSET_LIGHT) :
    start_paint (10, 200, 200, BLACKOUT_LIGHT); 
    break;
    
  case (BLACKOUT_LIGHT) :   //плавное затемнение
    {PERIOD(300) {
      brightness--;
      FastLED.setBrightness(brightness);  
      if (brightness == 0)  
      mode_light_bedroom = OFF_LIGHT; 
    }}  
    break;      

  case (NIGHT_LIGHT) :      //ночная подсветка
    {PERIOD(200){ 
      if (brightness < 50) { 
        brightness++;
        for( int i = 0; i < LED_NUM; i++) 
        leds[i] = CHSV(0, 200, 255);
        FastLED.setBrightness(brightness);     
      }
    }}
    break;

  case (OFF_LIGHT) :        //полное выключение света
    if(an_action)
    {
      flag_one_start = true;
      FastLED.clear();    
    }
    break; 
  case (MULTI_COLOR_LIGHT) :
    multi_color_light();
  default :
    break;
  }
  an_action = false;
      
 }
  void setBrightness()
  {
    static byte last_brigthness;
    if(last_brigthness != brightness)
    {
      last_brigthness = brightness; 
      FastLED.setBrightness(brightness);
    }
  }
  //пришла команда на смену
  void AnAction (MODE_LIGHT_BEDROM set_this_mode )
  {
    an_action = true;
    mode_light_bedroom = set_this_mode;
  }
 //сохранение цвета для ленты
  void ColorLightingNow (byte set_red, byte set_green, byte set_blue)
   {
      red_color_now = set_red; 
      green_color_now = set_green;
      blue_color_now = set_blue;
   }
//заливка ленты цветом  
  void full_paint (const int color , const int saturation, const int hue ) {
   for (int i = 0; i < LED_NUM; i++)         leds[i].setRGB(color, saturation, hue);
 }
// 
 void start_paint (const int color, const int saturation, const int hue, MODE_LIGHT_BEDROM next ) {
   static uint8_t LED_LEFT =  LED_NUM / 2; 
   static uint8_t LED_RIGHT =  LED_NUM / 2;
   {PERIOD (300) { 
    if( LED_RIGHT < LED_NUM) {
      leds[LED_RIGHT++].setRGB( color, saturation ,hue); 
      leds[LED_LEFT--].setRGB( color, saturation ,hue); 
    }
   }}
   if (LED_RIGHT == LED_NUM)  {
    mode_light_bedroom = next;
    LED_LEFT =  LED_NUM / 2;
    LED_RIGHT =  LED_NUM / 2;
   }
} 

//Ir remote
 void remoteTick() {
  static decode_results result;
  if (IrRemote.decode_old(&result))  {
    ir_flag = true;
    IrRemote.resume();
  }
  if (ir_flag) { // если данные пришли
    switch (result.value) {
    //brightness
      PRINT("result.value ", result.value);
      case BUTT_HIGHT_UP : 
        if(brightness == 0) 
          brightness = 5;
        if(brightness <= 245)
          brightness +=10;
        break;
      case BUTT_HIGHT_DOWN : 
        if(brightness >= 10)
          brightness -=10;
          if(brightness == 5)
            brightness = 0;
        break;  
    //цвета
      case BUTT_RED_UP :
        red_color_now++;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
      case BUTT_RED_DOWN :
        red_color_now--;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
      case BUTT_GREEN_UP :
        green_color_now++;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
      case BUTT_GREEN_DOWN :
        green_color_now--;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
      case BUTT_BLUE_UP :
        blue_color_now++;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
      case BUTT_BLUE_DOWN :
        blue_color_now--;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
     //1 столбец
      case BUTT_RED : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(255,0,0);        
        break;  
      case BUTT_RED1 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(255, 10,0);
        break;  
      case BUTT_RED2 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(245,25,0);
        break;
      case BUTT_RED3 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(240,39,2);
        break;  
      case BUTT_RED4 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(255,255,0);
        break;  
     //2 столбец
      case BUTT_GREEN : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(0,100,0);
        break;
      case BUTT_GREEN1 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(24, 105 , 15);
        break;  
      case BUTT_GREEN2 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(255, 255 , 15);
        break;  
      case BUTT_GREEN3 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(34, 98 , 225);
        break;  
      case BUTT_GREEN4 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(40, 110 , 200);
        break;  
     //3 столбец      
      case BUTT_BLUE : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(0,0,128);
        break;    
      case BUTT_BLUE1 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(12,120,255);
        break; 
      case BUTT_BLUE2 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(255,0,10);
        break;    
      case BUTT_BLUE3 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(70,24,122);
        break; 
      case BUTT_BLUE4 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(255,100,255);
        break; 
     //4 столбец      
      case BUTT_WHITE : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(255,255,255);
        break; 
      case BUTT_WHITE1 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(216,30,24);
        break; 
      case BUTT_WHITE2 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(174,38,32);
        break; 
      case BUTT_WHITE3 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(40,103,39);
        break; 
      case BUTT_WHITE4 : 
        AnAction(COLOR_LIGHT);
        ColorLightingNow(45,110,89);
        break; 
    // режимы
      /*case BUTT_1: mode_light_bedroom = 0;
        break;
      case BUTT_2: mode_light_bedroom = 1;
        break;
      case BUTT_3: mode_light_bedroom = 2;
        break;
      case BUTT_4: mode_light_bedroom = 3;
        break;
      case BUTT_5: mode_light_bedroom = 4;
        break;
      case BUTT_6: mode_light_bedroom = 5;
        break;
      case BUTT_7: mode_light_bedroom = 6;
        break;
      case BUTT_8: mode_light_bedroom = 7;
        break;
      case BUTT_9: mode_light_bedroom = 8;
        break;
      case BUTT_0: 
        break; */
      case BUTT_STAR: 
        break;
      case BUTT_OK: 
        break;
      case BUTT_UP:
        break;
      case BUTT_DOWN:
        break;
      case BUTT_LEFT:
        break;
      case BUTT_RIGHT:
        break;
      default:    // если не распознали кнопку
        break;
    }
    ir_flag = false;
  }
 }


// --------разные режимы подсветки-------------
 void Color() {}

 void Cylon() {  
    fadeToBlackBy( leds, LED_NUM, 20);
    byte dothue = 0;
    for( int i = 0; i < 8; i++) {
      leds[beatsin16( i+7, 0, LED_NUM-1 )] |= CHSV(dothue, 200, 255);
      dothue += 32; }
 }
 void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, LED_NUM, gHue, 7);
 }
 void addGlitter( int chanceOfGlitter) 
  {
  if( random8() < chanceOfGlitter) {
    leds[ random16(LED_NUM) ] += CRGB::White;
  } 
 }
 void rainbow_With_Glitter() 
  {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
 }
 void confetti() 
  {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, LED_NUM, 10);
  int pos = random16(LED_NUM);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
 }

 void sinelon()
  {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, LED_NUM, 20);
  int pos = beatsin16( 13, 0, LED_NUM-1 );
  leds[pos] += CHSV( gHue, 255, 192);
 }

 void bpm()
  {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < LED_NUM; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
 }

 void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, LED_NUM, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, LED_NUM-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
 }
 void color_Temperature () {
  // draw a generic, no-name rainbow
  static uint8_t starthue = 0;
  fill_rainbow( leds + 5, LED_NUM - 5, --starthue, 20);

  // Choose which 'color temperature' profile to enable.
  uint8_t secs = (millis() / 1000) % (DISPLAYTIME * 2);
  if( secs < DISPLAYTIME) {
    FastLED.setTemperature( TEMPERATURE_1 ); // first temperature
    leds[0] = TEMPERATURE_1; // show indicator pixel
  } else {
    FastLED.setTemperature( TEMPERATURE_2 ); // second temperature
    leds[0] = TEMPERATURE_2; // show indicator pixel
  }

 
 }

