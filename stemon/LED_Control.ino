

void ledPWM(void){

  if(m_color_dim==1){
    if(m_dim_led_counter<=255){

      if(m_color_code&RED_COLOR)
        ledcWrite(RED_LED,    (m_dim_led_counter*m_color_red_w/100));

      if(m_color_code&GREEN_COLOR)
        ledcWrite(GREEN_LED,  (m_dim_led_counter*m_color_green_w/100));
    
      if(m_color_code&BLUE_COLOR)
      ledcWrite(BLUE_LED,   (m_dim_led_counter*m_color_blue_w/100));

    }
    else{
      if(m_dim_led_counter<510){
        if(m_color_code&RED_COLOR)
          ledcWrite(RED_LED,    (255-m_dim_led_counter%255)*m_color_red_w/100);
        if(m_color_code&GREEN_COLOR)
          ledcWrite(GREEN_LED,  (255-m_dim_led_counter%255)*m_color_green_w/100);
        if(m_color_code&BLUE_COLOR)
          ledcWrite(BLUE_LED,   (255-m_dim_led_counter%255)*m_color_blue_w/100);
      }
      else{
          ledcWrite(RED_LED,    0);
          ledcWrite(GREEN_LED,  0);
          ledcWrite(BLUE_LED,  0);
      
      }
    }
  }
  else{
      if(m_color_code&RED_COLOR)
        ledcWrite(RED_LED,    (255*m_color_red_w/100));

      if(m_color_code&GREEN_COLOR)
        ledcWrite(GREEN_LED,  (255*m_color_green_w/100));
    
      if(m_color_code&BLUE_COLOR)
      ledcWrite(BLUE_LED,   (255*m_color_blue_w/100));
  }
  m_dim_led_counter++;
  m_dim_led_counter++;

  if(m_dim_led_counter==550)
    m_dim_led_counter=0;

}



void led_control(void){

  if(m_current_status.CURRENT_STATE==POWER_ON_PROCESSING){
    if(m_current_status.CURRENT_MODE==MAIN_CARGO_MODE){
      LED_YELLOW_START(1);
    }
    else if(m_current_status.CURRENT_MODE==MAIN_EXOSOME_MODE){
      LED_RED_START(1);
    }
    else if(m_current_status.CURRENT_MODE==MAIN_WASH_MODE){
      LED_BLUE_START(1);
    }
    else if(m_current_status.CURRENT_MODE==MAIN_END_MODE || m_current_status.CURRENT_MODE==MAIN_STOP_MODE){
      LED_JADE_START(1);
    }

    if(m_uvc_led==UVC_LED_ON)
      LED_VIOLET_START(1);
  
  }
  else if(m_current_status.CURRENT_STATE==POWER_ON){
    if(m_current_status.CURRENT_MODE==MAIN_CARGO_MODE){
      LED_YELLOW_START(0);
    }
    else if(m_current_status.CURRENT_MODE==MAIN_EXOSOME_MODE){
      LED_RED_START(0);
    }
    else if(m_current_status.CURRENT_MODE==MAIN_WASH_MODE){
      LED_BLUE_START(0);
    }
    else if(m_current_status.CURRENT_MODE==MAIN_STAND_BY_MODE){
      LED_GREEN_START(0);
    }
    else if(m_current_status.CURRENT_MODE==MAIN_BOOT_MODE){
      LED_GREEN_START(0);
    }
    else if(m_current_status.CURRENT_MODE==ADMIN_MODE){
      LED_ORANGE_START(0);
    }

  }
  else if(m_current_status.CURRENT_STATE==POWER_OFF){
    LED_WHITE_START(0);
  }
}



void LED_RED_START(uint8_t mode){


  ledcWrite(BLUE_LED,   0);
  ledcWrite(GREEN_LED,   0);

  m_color_red_w = 100;
  m_color_code=RED_COLOR;

    //dimming
    if(mode){
      m_color_dim=1;  
    }
    else{
      m_color_dim=0;  
    }
}

void LED_GREEN_START(uint8_t mode){


  ledcWrite(BLUE_LED,   0);
  ledcWrite(RED_LED,   0);

  m_color_green_w = 100;
  m_color_code=GREEN_COLOR;

    //dimming
    if(mode){
      m_color_dim=1;  
    }
    else{
      m_color_dim=0;  
    }
}

void LED_BLUE_START(uint8_t mode){

  ledcWrite(RED_LED,   0);
  ledcWrite(GREEN_LED,   0);

  m_color_blue_w = 100;
  m_color_code=BLUE_COLOR;

    //dimming
    if(mode){
      m_color_dim=1;  
    }
    else{
      m_color_dim=0;  
    }
}

void LED_YELLOW_START(uint8_t mode){

  ledcWrite(BLUE_LED,   0);

  m_color_red_w =   100;
  m_color_green_w = 60;

  m_color_code=YELLOW_COLOR;

    //dimming
    if(mode){
      m_color_dim=1;  
    }
    else{
      m_color_dim=0;  
    }
}

void LED_ORANGE_START(uint8_t mode){

  ledcWrite(BLUE_LED,   0);

  m_color_red_w =   100;
  m_color_green_w = 20;

  m_color_code=YELLOW_COLOR;

    //dimming
    if(mode){
      m_color_dim=1;  
    }
    else{
      m_color_dim=0;  
    }
}

void LED_VIOLET_START(uint8_t mode){

  ledcWrite(GREEN_LED,   0);

  m_color_red_w =   100;
  m_color_blue_w =  60;

  m_color_code=VIOLET_COLOR;

    //dimming
    if(mode){
      m_color_dim=1;  
    }
    else{
      m_color_dim=0;  
    }
}

void LED_JADE_START(uint8_t mode){

  ledcWrite(RED_LED,   0);

  m_color_blue_w =    50;
  m_color_green_w =   80;

  m_color_code=JADE_COLOR;

    //dimming
    if(mode){
      m_color_dim=1;  
    }
    else{
      m_color_dim=0;  
    }
}

void LED_WHITE_START(uint8_t mode){

 
  m_color_blue_w =    60;
  m_color_green_w =   60;
 m_color_red_w =    60;

  m_color_code=WHITE_COLOR;

    //dimming
    if(mode){
      m_color_dim=1;  
    }
    else{
      m_color_dim=0;  
    }
}