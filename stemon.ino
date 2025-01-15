#include <Arduino.h>
#include <Ticker.h>
#include <Wire.h>
#include <EEPROM.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_MCP4725.h>
#include "EspUsbHost.h"
#include "PIN_MAP.h"
#include "LCD_CMD_LIST.h"
#include "stem_define.h"

CURRENT_MODE  m_current_status;
SET_STATE     m_current_set;
MODE_STATE    m_wash_state;
MODE_STATE    m_cargo_state;
MODE_STATE    m_exosome_state;
MODE_STATE    m_end_state;            //power off 
MODE_STATE    m_boot_state;           //power on
MODE_STATE    m_stop_state;           //STOP button
MODE_STATE    m_stop_wash_state;      //STOP button

unsigned char m_motor_start_open_cmd=0;
unsigned char m_motor_start_close_cmd=0;
unsigned char m_door_motor_open_counter=0;
unsigned char m_door_motor_close_counter=0;

unsigned char m_over_motor=0;
unsigned char m_uvc_led=0;
unsigned char m_check_tube_flag=0;
unsigned int m_delay_time=0;
unsigned int m_horn_pos=0;


class MyEspUsbHost : public EspUsbHost {
  void onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier) {
    if (' ' <= ascii && ascii <= '~') {
      Serial.printf("USB Keyboard press : %c \r\n", ascii);
    } else if (ascii == '\r') {
      Serial.println();
    }
  };
};


Adafruit_MCP4725 dac;
#define DAC_RESOLUTION    (8)
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_60MS, TCS34725_GAIN_4X);
MyEspUsbHost usbHost;

unsigned char m_tube_color=0;
unsigned char EEPROM_FLAG=0; 
unsigned char DOOR_STATUS=0; 
unsigned char HORN_STATUS=0;
unsigned char m_door_motor_cmd=0;
unsigned char m_horn_motor_cmd=0;

Ticker ticker_1;
Ticker ticker_2;
Ticker ticker_3;


int uv_con_counter=0; 
int adcValue_horn_motor=0;
int adcValue_door_motor=0;
unsigned long repeatDebugMsg;

unsigned char m_set_DAC=0;
unsigned char m_color_code=0;
unsigned char m_color_dim=0;
unsigned char m_color_red_w=100;
unsigned char m_color_green_w=100;
unsigned char m_color_blue_w=100;

long main_counter=0;
unsigned char m_boot_wash=1;

char m_debug_msg[128];

//50HZ
void readKEY(void) {

  main_counter++;


  CHECK_DOOR_MOTOR_STATUS();
  CHECK_HORN_MOTOR_STATUS();

  //1HZ
  if(main_counter%5==0)
    main_control();

  //2HZ
  if(main_counter%10==0){
//    adcValue_door_motor  = adc1_get_raw(ADC1_CHANNEL_8);
//    Serial.printf("motor current =%d \r\n",adcValue_door_motor);
//    CHECK_MOTOR_STATUS();
  }

  ReadKeyPad();

  usbHost.task();


  if(m_set_DAC){
    dac.setVoltage( (4095*m_set_DAC/100),false);
    Serial.print(" DAC value =   ");
    Serial.println((4095*m_set_DAC/100));
    m_set_DAC=0;
  }



}


void setup() {

  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);

  Wire.begin(I2C_SDA,I2C_SCL);
  usbHost.begin();

  delay(100);
  
  //init
  m_current_status.CURRENT_STATE =    POWER_OFF;
  m_current_status.logo_image_count = 0;

  INIT_EEPROM();

  if (tcs.begin(0x29, &Wire)) {
    Serial.println("RGB Found sensor");
    m_current_status.rgb_sensor = SENSOR_YOU;

  } else {
    Serial.println("No TCS34725 found ... check your connections");
    m_current_status.rgb_sensor = SENSOR_MEIYOU;

  }

  if (dac.begin(0x62,&Wire)){
    Serial.println("DAC Found sensor");    
    m_current_status.dac_sensor = SENSOR_YOU;
  }
  else{
    Serial.println("No DAC sensor found ... check your connections");
    m_current_status.dac_sensor = SENSOR_MEIYOU;
  }

  pinMode(UV_CON_LED, OUTPUT);          // sets the digital pin as output
  pinMode(RED_LED, OUTPUT);             // sets the digital pin as output
  pinMode(GREEN_LED, OUTPUT);           // sets the digital pin as output
  pinMode(BLUE_LED, OUTPUT);            // sets the digital pin as output
  pinMode(BUZZER_1, OUTPUT);            // sets the digital pin as output

  pinMode(HORN_MOTOR_PWM, OUTPUT);        // sets the digital pin as output
  pinMode(HORN_MOTOR_DIR, OUTPUT);        // sets the digital pin as output
  pinMode(DOOR_MOTOR_PWM, OUTPUT);        // sets the digital pin as output
  pinMode(DOOR_MOTOR_DIR, OUTPUT);        // sets the digital pin as output

  //pinMode(HORN_MOTOR_ADC, INPUT);         // sets the digital pin as input

  pinMode(ULTRA_PWR_EN_1, OUTPUT);          // sets the digital pin as output
  pinMode(ULTRA_PWR_EN_2, OUTPUT);          // sets the digital pin as output

  //pinMode(ULTRA_PWR_CHK, INPUT);        // sets the digital pin as output

  pinMode(HORN_50,    INPUT);                // sets the digital pin as input
  pinMode(HORN_100,   INPUT);               // sets the digital pin as input
  pinMode(HORN_130,   INPUT);               // sets the digital pin as input
  pinMode(DOOR_OPEN,  INPUT);              // sets the digital pin as input
  pinMode(DOOR_CLOSE, INPUT);             // sets the digital pin as input

  pinMode(KEY_1, INPUT);              // sets the digital pin as input
  pinMode(KEY_2, INPUT);              // sets the digital pin as input
  pinMode(KEY_3, INPUT);              // sets the digital pin as input
  pinMode(KEY_4, INPUT);              // sets the digital pin as input
  pinMode(KEY_5, INPUT);              // sets the digital pin as input
  pinMode(KEY_6, INPUT);              // sets the digital pin as input
  pinMode(KEY_7, INPUT);              // sets the digital pin as input

  //ADC SET
  //analogReadResolution(12);
  pinMode(HORN_MOTOR_ADC, INPUT);              // sets the digital pin as input
  pinMode(DOOR_MOTOR_ADC, INPUT);              // sets the digital pin as input

  delay(100);


  // Ticker를 20ms마다 myFunction을 호출하도록 설정
  ticker_1.attach(0.02, readKEY); 
  ticker_2.attach(0.01, ledPWM);
//  ticker_3.attach(0.1,  main_control);
  

  ledcAttach(RED_LED,     5000,8); // 핀과 채널 연결
  ledcAttach(GREEN_LED,   5000,8); // 핀과 채널 연결
  ledcAttach(BLUE_LED,    5000,8); // 핀과 채널 연결
  ledcAttach(BUZZER_1,    2000,8); // 핀과 채널 연결

  ledcAttach(DOOR_MOTOR_PWM,    2000,8); // 핀과 채널 연결
  ledcAttach(DOOR_MOTOR_DIR,    2000,8); // 핀과 채널 연결

  ledcAttach(HORN_MOTOR_PWM,    2000,8); // 핀과 채널 연결
  ledcAttach(HORN_MOTOR_DIR,    2000,8); // 핀과 채널 연결



  //Horn Motor STOP
  digitalWrite(HORN_MOTOR_PWM,   LOW);
  digitalWrite(HORN_MOTOR_DIR,   LOW);
  
  //Door Motor STOP
  digitalWrite(DOOR_MOTOR_PWM,   LOW);
  digitalWrite(DOOR_MOTOR_DIR,   LOW);

  DOOR_STATUS=CHECK_DOOR_STATUS();
  m_current_status.door_motor_position = CHECK_DOOR_STATUS();
  
  HORN_STATUS=CHECK_HORN_STATUS();
  m_current_status.horn_motor_position = CHECK_HORN_STATUS();
  

  readFromFlash();

  led_control();

  m_set_DAC=0;

  repeatDebugMsg= millis();  //

}

unsigned char test_lcd_counter=0;
int   m_nStart_pc=0;
int   m_nEnd_pc=0;
char  m_PC_tempbuf[64];


void loop() {

  uint8_t temp=0;
  uint16_t r, g, b,c,ch;


  GetPC_Command();

  if(Serial.available()){
    m_nStart_pc%=64;
    ch =Serial.read();
    m_PC_tempbuf[m_nStart_pc++]=ch;  
  }

  if( (millis() -repeatDebugMsg) > 1000){


 
//    if(m_set_DAC==0 && m_check_tube_flag==1){
    if(m_set_DAC==0 ){

      temp=m_tube_color;
      tcs.getRawData(&r, &g, &b, &c);
      find_color(r,g,b);
      sprintf(m_debug_msg,"%s %d %d %d",  getStatusString_COLOR(m_tube_color),r,g,b);
      Serial.println(m_debug_msg);

//      if(temp!=m_tube_color)
        LCD_TUBE_IMAGE(m_tube_color);
    }

    m_current_status.door_motor_position = CHECK_DOOR_STATUS();
    m_current_status.horn_motor_position = CHECK_HORN_STATUS();


//    sprintf(m_debug_msg,"image=%d, Door= %s Horn= %s rgb=%d, dac=%d, eeprom=%d",  
//      m_current_status.CURRENT_IMAGE,  getStatusString_DOOR(m_current_status.door_motor_position), getStatusString_HORN(m_current_status.horn_motor_position),
//      m_current_status.rgb_sensor, m_current_status.dac_sensor,EEPROM_FLAG);
//    Serial.println(m_debug_msg);

/*
    adcValue_door_motor   = analogRead(DOOR_MOTOR_ADC);
    adcValue_horn_motor   = analogRead(HORN_MOTOR_ADC);

    Serial.printf("motor current =%d %d\r\n",adcValue_door_motor, adcValue_horn_motor);
*/
   // CHECK_MOTOR_STATUS();

    if(m_current_status.CURRENT_MODE==ADMIN_MODE){
      LCD_ADMIN_SENSOR_IMAGE();      
    }

    sprintf(m_debug_msg,"s_wash= %d, stop= %d boot= %d, end= %d, wash= %d, cargo= %d, exosome= %d, count=%d, mc_open=%d, mc_close=%d, uvled=%d ",  
      m_stop_wash_state.process_to_be, m_stop_state.process_to_be, m_boot_state.process_to_be, m_end_state.process_to_be,m_wash_state.process_to_be,m_cargo_state.process_to_be,m_exosome_state.process_to_be,
      m_delay_time,m_door_motor_open_counter,m_door_motor_close_counter,m_uvc_led);
    Serial.println(m_debug_msg);


    Serial.print("STATE = ");  
    Serial.print(getStatusString_STATE(m_current_status.CURRENT_STATE)); 
    Serial.print("  MODE = ");  
    Serial.print(getStatusString_MODE(m_current_status.CURRENT_MODE)); 
    Serial.print("  Door = ");  
    Serial.print(getStatusString_DOOR(m_current_status.door_motor_position)); 
    Serial.print("  HORN = ");  
    Serial.println(getStatusString_HORN(m_current_status.horn_motor_position)); 

  


 //   sprintf(m_debug_msg,"carge_set =%d, exosome_set = %d",  m_current_set.set_cargo,m_current_set.set_exosome);
 //   Serial.println(m_debug_msg);


    if(m_current_status.logo_image_count){
      m_current_status.logo_image_count--;
      if(m_current_status.logo_image_count==0){
        LCD_MAIN_IMAGE();    
        m_current_status.CURRENT_IMAGE=   IMAGE_MAIN_DISPLAY;
        m_current_status.CURRENT_STATE=   POWER_ON_PROCESSING;
        m_current_status.CURRENT_MODE =   MAIN_BOOT_MODE;
        m_boot_state.process_to_be =      BOOT_HORN_TOP;
        led_control();
      }
    }


    repeatDebugMsg=millis();

  }
/*
  adcValue_horn_motor = analogRead(HORN_MOTOR_ADC);   // GPIO 8번에서 아날로그 값 읽기

  Serial.print("horn current: ");
  Serial.print(adcValue_horn_motor);                // 읽은 값 출력
  Serial.print("    door current: ");
  Serial.println(adcValue_door_motor);                // 읽은 값 출력
*/  





}
