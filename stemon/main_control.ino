
long lcd_main_counter=0;
unsigned char  lcd_end_mode_counter=0;


void GO_TO_ADMIN_MODE(void){
  m_current_status.CURRENT_STATE=POWER_ON;
  m_current_status.CURRENT_MODE=ADMIN_MODE;
  LCD_MAIN_MODE_IMAGE( m_current_status.CURRENT_MODE);
  led_control();
}

void GO_TO_WASH_MODE(void){

  m_current_status.CURRENT_MODE=    MAIN_WASH_MODE;
  m_current_status.CURRENT_WASH_MODE_COUNT=0;
  m_current_status.CURRENT_STATE=   POWER_ON_PROCESSING;
  LCD_MAIN_MODE_IMAGE( m_current_status.CURRENT_MODE);
  led_control();
  m_wash_state.process_to_be =      WASH_DOOR_OPEN;

}


void GO_TO_STAND_BY_MODE(unsigned char mode){

  m_current_status.CURRENT_STATE=POWER_ON;
  m_current_status.CURRENT_MODE=mode;
  LCD_MAIN_MODE_IMAGE( m_current_status.CURRENT_MODE);
  led_control();
  Serial.print("go to stand by mode"); 

}


void STOP_MODE_PROCESS(void){

 if(m_stop_state.process_to_be == STOP_HORN_TOP){
    HORN_MOTOR_CLOCK(HORN_UP_DIR);
    m_horn_motor_cmd=HORN_CMD_DIR_50;
    m_delay_time = 10;
 }
 else if(m_stop_state.process_to_be == STOP_DOOR_OPEN){
  DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR); 
  m_door_motor_cmd = DOOR_OPEN_DIR;
  m_delay_time = 10;

 }
 else if(m_stop_state.process_to_be == STOP_FINISH){
  m_stop_state.process_to_be = STOP_STAND_BY;
  GO_TO_STAND_BY_MODE(MAIN_STAND_BY_MODE);
 }
}

void STOP_WASH_MODE_PROCESS(void){


 if(m_stop_wash_state.process_to_be == STOP_WASH_HORN_TOP){
    ULTRA_STOP();
    HORN_MOTOR_CLOCK(HORN_UP_DIR);
    m_horn_motor_cmd=HORN_CMD_DIR_50;
    m_delay_time = 10;
 }
 else if(m_stop_wash_state.process_to_be == STOP_WASH_DOOR_CLOSE){
  DOOR_MOTOR_CLOCK(DOOR_CLOSE_DIR); 
  m_door_motor_cmd = DOOR_CLOSE_DIR;
  m_delay_time = 10;

 }
 else if(m_stop_wash_state.process_to_be == STOP_WASH_FINISH){
  m_stop_wash_state.process_to_be = STOP_WASH_STAND_BY;
  GO_TO_STAND_BY_MODE(MAIN_STAND_BY_MODE);
 }
}


void BOOT_MODE_PROCESS(void){

 if(m_boot_state.process_to_be == BOOT_HORN_TOP){
    HORN_MOTOR_CLOCK(HORN_UP_DIR);
    m_horn_motor_cmd=HORN_CMD_DIR_50;
    m_delay_time = 10;
 }
 else if(m_boot_state.process_to_be == BOOT_DOOR_CLOSE){
  DOOR_MOTOR_CLOCK(DOOR_CLOSE_DIR); 
  m_door_motor_cmd = DOOR_CLOSE_DIR;
  m_delay_time = 10;

 }
 else if(m_boot_state.process_to_be == BOOT_FINISH){
  m_boot_state.process_to_be = BOOT_STAND_BY;
  //GO_TO_STAND_BY_MODE(MAIN_STAND_BY_MODE);

  m_boot_wash=1;
  m_current_status.CURRENT_MODE=    MAIN_WASH_MODE; 
  m_current_status.CURRENT_STATE=   POWER_ON_PROCESSING;
  led_control();
  LCD_MAIN_MODE_IMAGE(m_current_status.CURRENT_MODE);
  m_wash_state.process_to_be =      WASH_DOOR_OPEN;
 
 }

}


void END_MODE_PROCESS(void){

 if(m_end_state.process_to_be == END_HORN_TOP){
  HORN_MOTOR_CLOCK(HORN_UP_DIR);
  m_horn_motor_cmd=HORN_CMD_DIR_50;
  m_delay_time = 20;
 } 
 else if(m_end_state.process_to_be == END_CHECK_TUBE_1){
  m_check_tube_flag=1;
  if(m_tube_color==NO_TUBE){
    m_end_state.process_to_be = END_DOOR_CLOSE;
  }
  else{
    m_end_state.process_to_be = END_DOOR_OPEN;
    m_delay_time = 10;
  }
 }
 else if(m_end_state.process_to_be == END_DOOR_OPEN){
  DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR); 
  m_door_motor_cmd = DOOR_OPEN_DIR;
 }
 else if(m_end_state.process_to_be == END_CHECK_TUBE_2){
  m_check_tube_flag=1;
  if(m_tube_color==NO_TUBE){
    m_end_state.process_to_be = END_DOOR_CLOSE;
    m_delay_time = 50;     
  }
 }
 else if(m_end_state.process_to_be == END_DOOR_CLOSE){
  m_check_tube_flag=0;
  DOOR_MOTOR_CLOCK(DOOR_CLOSE_DIR); 
  m_door_motor_cmd = DOOR_CLOSE_DIR;
 }
 else if(m_end_state.process_to_be == END_UV_CON){
  digitalWrite(UV_CON_LED,HIGH);             //LED ON
  m_uvc_led=UVC_LED_ON;
  led_control();
  LCD_STER_ICON_ON();
  m_end_state.process_to_be = END_FINISH;
  m_current_status.CURRENT_END_UV_MODE_COUNT=0;
  m_delay_time = UVC_WORKING_TIME;     
 }
 else if(m_end_state.process_to_be == END_FINISH){

  digitalWrite(UV_CON_LED,LOW);             //LED OFF
  m_uvc_led=UVC_LED_OFF;

  m_end_state.process_to_be = END_STAND_BY;
  LCD_BACK_LIGHT_OFF();
  m_current_status.CURRENT_STATE=POWER_OFF; 
  led_control();
 }

}


void EXOSOME_MODE_PROCESS(void){

 if(m_exosome_state.process_to_be == EXOSOME_DOOR_OPEN){
  DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR); 
  m_door_motor_cmd = DOOR_OPEN_DIR;
 }
 else if(m_exosome_state.process_to_be == EXOSOME_CHECK_TUBE){
  m_check_tube_flag=1;
  if(m_tube_color==RED_COLOR || m_tube_color==GREEN_COLOR){
    LCD_LOGO_IMAGE();
    LCD_MAIN_MODE_IMAGE(m_current_status.CURRENT_MODE);
    m_exosome_state.process_to_be = EXOSOME_DOOR_CLOSE;
    m_delay_time = 20;     
  }
  else{
    if(m_tube_color != NO_TUBE)
      LCD_WARNING_1_IMAGE();
  }
 }
 else if(m_exosome_state.process_to_be == EXOSOME_DOOR_CLOSE){
  m_check_tube_flag=0;
  DOOR_MOTOR_CLOCK(DOOR_CLOSE_DIR); 
  m_door_motor_cmd = DOOR_CLOSE_DIR;
  m_delay_time = 40;
 }
 else if(m_exosome_state.process_to_be == EXOSOME_HORN_BOTTOM){
  HORN_MOTOR_CLOCK(HORN_DOWN_DIR);
  m_horn_motor_cmd=HORN_CMD_DIR_130;
  m_delay_time = 60;
 }
 else if(m_exosome_state.process_to_be == EXOSOME_ULTRA){
  m_delay_time = m_current_set.exosome_time;
  //START ULTRA
  EXOSOME_ULTRA_START();
  Serial.printf("exosome ultra start %d %d %d \r\n",m_set_DAC, m_delay_time, m_current_set.set_exosome);
  m_exosome_state.process_to_be = EXOSOME_HOLD;
 }
 else if(m_exosome_state.process_to_be == EXOSOME_HOLD){
  //STOP ULTRA
  m_set_DAC=30;  
  ULTRA_STOP();
  Serial.printf("exosome ultra stop %d \r\n",m_set_DAC);
 
  m_delay_time = 60;
  m_exosome_state.process_to_be = EXOSOME_HORN_TOP;

 }
 else if(m_exosome_state.process_to_be == EXOSOME_HORN_TOP){
  HORN_MOTOR_CLOCK(HORN_UP_DIR);
  m_horn_motor_cmd=HORN_CMD_DIR_50;
  m_delay_time = 70-m_current_set.exosome_time;
 }
 else if(m_exosome_state.process_to_be == EXOSOME_FINISH_HOLD){
  m_exosome_state.process_to_be = EXOSOME_FINISH;
  m_delay_time = 10;
 }
 else if(m_exosome_state.process_to_be == EXOSOME_FINISH){
  m_exosome_state.process_to_be = EXOSOME_DOOR_OPEN_2;
 }

 else if(m_exosome_state.process_to_be == EXOSOME_CHECK_TUBE_2){
  m_check_tube_flag=1;
  if(m_tube_color==NO_TUBE){
    m_exosome_state.process_to_be = EXOSOME_STAND_BY;
    m_delay_time = 20;     
  }
 }


 else if(m_exosome_state.process_to_be == EXOSOME_STAND_BY){
  m_check_tube_flag=0;
  m_exosome_state.process_to_be = EXOSOME_STAND_BY;
  //GO_TO_STAND_BY_MODE(MAIN_EXOSOME_MODE);
  GO_TO_WASH_MODE();
 }

}

void CARGO_MODE_PROCESS(void){

 if(m_cargo_state.process_to_be == CARGO_DOOR_OPEN){
  DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR); 
  m_door_motor_cmd = DOOR_OPEN_DIR;
 }
 else if(m_cargo_state.process_to_be == CARGO_CHECK_TUBE){
  m_check_tube_flag=1;
  if(m_tube_color==YELLOW_COLOR || m_tube_color==GREEN_COLOR){
    LCD_LOGO_IMAGE();
    LCD_MAIN_MODE_IMAGE(m_current_status.CURRENT_MODE);
    m_cargo_state.process_to_be = CARGO_DOOR_CLOSE;
    m_delay_time = 30;     
  }
  else{
    if(m_tube_color != NO_TUBE)
      LCD_WARNING_1_IMAGE();
  }
 }
 else if(m_cargo_state.process_to_be == CARGO_DOOR_CLOSE){
  m_check_tube_flag=0;
  DOOR_MOTOR_CLOCK(DOOR_CLOSE_DIR); 
  m_door_motor_cmd = DOOR_CLOSE_DIR;
  m_delay_time = 50;
 }
 else if(m_cargo_state.process_to_be == CARGO_HORN_BOTTOM){
  HORN_MOTOR_CLOCK(HORN_DOWN_DIR);
  m_horn_motor_cmd=HORN_CMD_DIR_130;
  m_delay_time = 60;
 }
 else if(m_cargo_state.process_to_be == CARGO_ULTRA){     //5
  m_delay_time = m_current_set.cargo_time;
  //START ULTRA
  CARGO_ULTRA_START();
  Serial.printf("cargo ultra start %d \r\n",m_set_DAC);

  m_cargo_state.process_to_be = CARGO_HOLD;               //6
 }
 else if(m_cargo_state.process_to_be == CARGO_HOLD){
  //STOP ULTRA
  m_set_DAC=40;  
  ULTRA_STOP();
  Serial.printf("cargo ultra stop %d \r\n",m_set_DAC);
 
  m_delay_time = 60;
  m_cargo_state.process_to_be = CARGO_HORN_TOP;

 }
 else if(m_cargo_state.process_to_be == CARGO_HORN_TOP){      //7
  Serial.println("cargo horn-up command");
  HORN_MOTOR_CLOCK(HORN_UP_DIR);
  m_horn_motor_cmd=HORN_CMD_DIR_50;
  m_delay_time = 60;
 }
 else if(m_cargo_state.process_to_be == CARGO_FINISH_HOLD){
  m_cargo_state.process_to_be = CARGO_FINISH;
  m_delay_time = 10;
 }

 else if(m_cargo_state.process_to_be == CARGO_FINISH){
  m_cargo_state.process_to_be = CARGO_DOOR_OPEN_2;
 }

 else if(m_cargo_state.process_to_be == CARGO_CHECK_TUBE_2){
  m_check_tube_flag=1;
  if(m_tube_color==NO_TUBE){
    m_cargo_state.process_to_be = CARGO_STAND_BY;
    m_delay_time = 20;     
  }
 }

 else if(m_cargo_state.process_to_be == CARGO_STAND_BY){
  m_check_tube_flag=0;
  m_exosome_state.process_to_be = CARGO_STAND_BY;
  //GO_TO_STAND_BY_MODE(MAIN_EXOSOME_MODE);
  GO_TO_WASH_MODE();
 
 }

}


void CHECK_MOTOR_STATUS(void){

 

  if(adcValue_door_motor>OVER_MOTOR_CURRENT){
    m_over_motor++;
    Serial.printf("motor current =%d \r\n",adcValue_door_motor);

    if(m_over_motor>4)
      Serial.println("        !!!! MOTOR 이상");
  }
  else
    m_over_motor=0;


}


void main_control(void){

  lcd_main_counter++;

  if(m_motor_start_open_cmd){
    m_door_motor_open_counter++;
    if(m_door_motor_open_counter>45){
      GO_TO_STAND_BY_MODE(m_current_status.CURRENT_MODE);
      DOOR_MOTOR_CLOCK(DOOR_CLOSE_DIR);
      m_door_motor_cmd = DOOR_CLOSE_DIR;

      m_door_motor_open_counter=0;
      m_motor_start_open_cmd=0;
    }
  }

  if(m_motor_start_close_cmd){
    m_door_motor_close_counter++;
    if(m_door_motor_close_counter>45){
      GO_TO_STAND_BY_MODE(m_current_status.CURRENT_MODE);
      DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR);
      m_door_motor_cmd = DOOR_OPEN_DIR;

      m_motor_start_close_cmd=0;
      m_door_motor_close_counter=0;
    }
  }



  //2HZ
  if(lcd_main_counter%5==0){
  
    if(m_current_status.CURRENT_MODE == MAIN_END_MODE){
      if(m_end_state.process_to_be >= END_UV_CON)
        LCD_END_MODE_IMAGE(lcd_end_mode_counter);

      lcd_end_mode_counter++;
    }
  
  }
 
  //1HZ
  if(lcd_main_counter%10==0){
    if(m_current_status.CURRENT_STATE == POWER_ON_PROCESSING){
 
      if(m_current_status.CURRENT_MODE == MAIN_CARGO_MODE){

        if(m_cargo_state.process_to_be>=CARGO_HORN_BOTTOM && m_cargo_state.process_to_be<CARGO_FINISH) 
          LCD_CARGO_START();
        if(m_cargo_state.process_to_be>=CARGO_FINISH){

          LCD_CARGO_ICON_START();
        }

      }

      if(m_current_status.CURRENT_MODE == MAIN_EXOSOME_MODE){
        
        if(m_exosome_state.process_to_be>=EXOSOME_HORN_BOTTOM && m_exosome_state.process_to_be<EXOSOME_FINISH)  
          LCD_EXOSOME_START();

        if(m_exosome_state.process_to_be>=EXOSOME_FINISH){
          LCD_EXOSOME_ICON_START();
        }

      }

      if(m_current_status.CURRENT_MODE == MAIN_END_MODE){
        if(m_end_state.process_to_be == END_FINISH)
          LCD_END_UV_START();
      
      }

      if(m_current_status.CURRENT_MODE == MAIN_WASH_MODE){

        if(m_wash_state.process_to_be < WASH_FINISH && m_wash_state.process_to_be >= WASH_HORN_BOTTOM_MOVE)
           LCD_WASH_START(); 
        else if(m_wash_state.process_to_be == WASH_FINISH)
          LCD_WASH_UV_START();
      }

    }

  }

  if(m_delay_time){
    m_delay_time--;
    return;
  }

  if(m_current_status.CURRENT_STATE == POWER_ON_PROCESSING){
 
    if(m_current_status.CURRENT_MODE == MAIN_END_MODE){
      END_MODE_PROCESS();
    }
    else if(m_current_status.CURRENT_MODE == MAIN_CARGO_MODE){
      CARGO_MODE_PROCESS();
    }
    else if(m_current_status.CURRENT_MODE == MAIN_EXOSOME_MODE){
      EXOSOME_MODE_PROCESS();
    }
    else if(m_current_status.CURRENT_MODE == MAIN_BOOT_MODE){
      BOOT_MODE_PROCESS();
    }
    else if(m_current_status.CURRENT_MODE == MAIN_STOP_MODE){
      STOP_MODE_PROCESS();
    }
    else if(m_current_status.CURRENT_MODE == MAIN_STOP_WASH_MODE){
      STOP_WASH_MODE_PROCESS();
    }
 
    else if(m_current_status.CURRENT_MODE == MAIN_WASH_MODE){
 
      if(m_wash_state.process_to_be == WASH_DOOR_OPEN){
          DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR); 
          m_door_motor_cmd = DOOR_OPEN_DIR;
      } 
      else if(m_wash_state.process_to_be == WASH_CHECK_TUBE){
        m_check_tube_flag=1;
        if(m_tube_color==GREEN_COLOR || m_tube_color==BLUE_COLOR){
          LCD_LOGO_IMAGE();
          LCD_MAIN_MODE_IMAGE(m_current_status.CURRENT_MODE);
          m_wash_state.process_to_be = WASH_DOOR_CLOSE;
          m_delay_time = 30;
        }
        else{
          if(m_tube_color!=NO_TUBE)
            LCD_WARNING_1_IMAGE();
        }
      }
      else if(m_wash_state.process_to_be == WASH_DOOR_CLOSE){
        m_check_tube_flag=0;
        DOOR_MOTOR_CLOCK(DOOR_CLOSE_DIR); 
        m_door_motor_cmd = DOOR_CLOSE_DIR;
        m_delay_time = 30;
      }
      else if(m_wash_state.process_to_be == WASH_HORN_BOTTOM_MOVE){
        HORN_MOTOR_CLOCK(HORN_DOWN_DIR);
        m_horn_motor_cmd=HORN_CMD_DIR_130;
        m_delay_time = 20;

      }
      else if(m_wash_state.process_to_be == WASH_ULTRA_ONE){
        m_delay_time = 90;
        //START ULTRA
        WASH_ULTRA_START(1);
       
        m_wash_state.process_to_be = WASH_HORN_MID_MOVE;
      }
      else if(m_wash_state.process_to_be == WASH_HORN_MID_MOVE){
        ULTRA_STOP();

        HORN_MOTOR_CLOCK(HORN_UP_DIR);
        m_horn_motor_cmd=HORN_CMD_DIR_100;
        m_delay_time = 20;

      }
      else if(m_wash_state.process_to_be == WASH_ULTRA_TWO){
        m_delay_time = 10;
        //START ULTRA
        WASH_ULTRA_START(2);

        m_wash_state.process_to_be = WASH_HOLD;
      }
     else if(m_wash_state.process_to_be == WASH_HOLD){
        m_delay_time = 20;
        m_wash_state.process_to_be = WASH_HORN_TOP_MOVE;
     }

      else if(m_wash_state.process_to_be == WASH_HORN_TOP_MOVE){
        ULTRA_STOP();
        HORN_MOTOR_CLOCK(HORN_UP_DIR);
        m_horn_motor_cmd=HORN_CMD_DIR_50;
        m_delay_time = 50;
      }
  //    else if(m_wash_state.process_to_be == WASH_DOOR_OPEN_B){
  //       DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR); 
  //       m_door_motor_cmd = DOOR_OPEN_DIR;

  //    } 
      else if(m_wash_state.process_to_be == WASH_CHECK_TUBE_B){
        m_check_tube_flag=1;
        if(m_tube_color==0){
          m_wash_state.process_to_be = WASH_DOOR_CLOSE_B;
          m_delay_time =30;
        }
      }
      else if(m_wash_state.process_to_be == WASH_DOOR_CLOSE_B){
        m_check_tube_flag=0;
        DOOR_MOTOR_CLOCK(DOOR_CLOSE_DIR); 
        m_door_motor_cmd = DOOR_CLOSE_DIR;
        m_delay_time = 20;
      }
      else if(m_wash_state.process_to_be == WASH_UV_CON){
        digitalWrite(UV_CON_LED,HIGH);             //LED ON
        m_uvc_led=UVC_LED_ON;
        led_control();
        LCD_STER_ICON_ON();
        m_wash_state.process_to_be = WASH_FINISH;
        m_current_status.CURRENT_WASH_UV_MODE_COUNT=0;
        if(m_boot_wash){
          m_delay_time = UVC_WORKING_BOOT_TIME;   //1min
          m_boot_wash=0;
          m_current_status.WASH_UV_TIME = 60;

        }
        else{
         m_current_status.WASH_UV_TIME = 60;   //120 seconds
         m_delay_time = UVC_WORKING_BOOT_TIME;       //2 min
        }

      } 
      else if(m_wash_state.process_to_be == WASH_FINISH){
        digitalWrite(UV_CON_LED,LOW);             //LED OFF
        m_uvc_led=UVC_LED_OFF;
        LCD_STER_ICON_OFF();
        m_wash_state.process_to_be = WASH_STAND_BY;
        GO_TO_STAND_BY_MODE(MAIN_STAND_BY_MODE);
      } 

    }
  }

}