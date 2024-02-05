#ifndef BUTTON_REMOTE_CONTROL
#define BUTTON_REMOTE_CONTROL
#include <Arduino.h>
  #define __remote_control__ 1
  
  #if  (__remote_control__ == 0)   //малый пульт
    #define BUTT_UP     0xE207E1AD    //ch+
    #define BUTT_DOWN   0x4E5BA3AD    //ch-
    #define BUTT_LEFT   0x517068AD    //prew
    #define BUTT_RIGHT  0x1B92DDAD    //next
    #define BUTT_OK     0xE51CA6AD    //ch
    #define BUTT_PLUS   0xD22353AD
    #define BUTT_MINUS  0x5484B6AD
    #define BUTT_0      
    #define BUTT_100    0xF08A26AD
    #define BUTT_200    0x151CD6AD
    #define BUTT_1      0x18319BAD    
    #define BUTT_2      0xF39EEBAD
    #define BUTT_3      0x4AABDFAD
    #define BUTT_4      0xE25410AD
    #define BUTT_5      0x297C76AD
    #define BUTT_6      0x14CE54AD
    #define BUTT_7      0xAF3F1BAD
    #define BUTT_8      0xC089F6AD
    #define BUTT_9      0x38379AD
    #define BUTT_0      0x68E456AD
    #define BUTT_STAR   0xAC2A56AD  // play
    #define BUTT_HASH   0xDF3F4BAD   // eq
  #endif

  #if  (__remote_control__ == 1)  //
    #define BUTT_STAR   0x1B92DDAD   
    #define BUTT_HASH   0x187E18AD  
    #define BUTT_UP     0x65CF91AD    
    #define BUTT_DOWN   0x2FF206AD    
    #define BUTT_LEFT   0x72EB89AD      
    #define BUTT_RIGHT  0xA8C914AD     
    #define BUTT_OK     0x57997BAD    
    #define BUTT_0      0x5484B6AD   
    #define BUTT_1      0x18319BAD   
    #define BUTT_2      0x151CD6AD   
    #define BUTT_3      0xABDDD9AD   
    #define BUTT_4      0xE25410AD   
    #define BUTT_5      0xE207E1AD   
    #define BUTT_6      0x14CE54AD   
    #define BUTT_7      0xAF3F1BAD   
    #define BUTT_8      0xDF3F4BAD   
    #define BUTT_9      0x76004EAD   
    #define BUTT_JUMPUP 0xC3ED3DAD   
    #define BUTT_JUMPDW 0xC0D878AD   
    #define BUTT_HIGHT_UP   0xB6FFA1AD
    #define BUTT_HIGHT_DOWN 0xB3EADCAD
    #define BUTT_RED    0x812216AD
    #define BUTT_RED1   0x62BB43AD
    #define BUTT_RED2   0x2CDDB8AD
    #define BUTT_RED3   0x297C76AD
    #define BUTT_RED4   0xF39EEBAD
    #define BUTT_GREEN  0x7E0D51AD
    #define BUTT_GREEN1 0x5FA67EAD
    #define BUTT_GREEN2 0x29C8F3AD
    #define BUTT_GREEN3 0x2667B1AD
    #define BUTT_GREEN4 0xF08A26AD
    #define BUTT_BLUE   0x4E5BA3AD
    #define BUTT_BLUE1  0x6CC276AD
    #define BUTT_BLUE2  0xA2A001AD
    #define BUTT_BLUE3  0xBD28B4AD
    #define BUTT_BLUE4  0x874B29AD
    #define BUTT_WHITE  0x517068AD
    #define BUTT_WHITE1 0x6FD73BAD
    #define BUTT_WHITE2 0xA5B4C6AD
    #define BUTT_WHITE3 0xBA13EFAD
    #define BUTT_WHITE4 0x843664AD
    #define BUTT_RED_UP 0xD53818AD
    #define BUTT_RED_DOWN 0x9F5A8DAD
    #define BUTT_GREEN_UP 0xD22353AD
    #define BUTT_GREEN_DOWN 0x9C45C8AD
    #define BUTT_BLUE_UP 0x68E456AD
    #define BUTT_BLUE_DOWN 0x3306CBAD
   #endif

#endif