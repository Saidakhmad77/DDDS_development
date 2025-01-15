#include <Arduino.h>

//#define A_TYPE
//#define B_TYPE

uint8_t temp_8=0,temp_8_b;

void LCD_ADMIN_SENSOR_IMAGE(void){

   if(m_current_status.door_motor_position==DOOR_OPEN_STATUS) {
    Serial1.write(Admin_Green_Door_Open_cmd,8);
    Serial1.write(Admin_Red_Door_Close_cmd,8);

   }
   else if(m_current_status.door_motor_position==DOOR_CLOSE_STATUS) {
    Serial1.write(Admin_Green_Door_Close_cmd,8);
    Serial1.write(Admin_Red_Door_Open_cmd,8);

   }

  if(m_current_status.horn_motor_position==HORN_POS_50_TOP) {
    Serial1.write(Admin_Red_Horn_50_cmd,8);
    Serial1.write(Admin_Red_Horn_100_cmd,8);
    Serial1.write(Admin_Green_Horn_130_cmd,8);
  }
  else if(m_current_status.horn_motor_position==HORN_POS_100_M) {
    Serial1.write(Admin_Red_Horn_50_cmd,8);
    Serial1.write(Admin_Green_Horn_100_cmd,8);
    Serial1.write(Admin_Red_Horn_130_cmd,8);
  }
  else if(m_current_status.horn_motor_position==HORN_POS_130_BOTTOM || m_horn_pos==3) {
    Serial1.write(Admin_Green_Horn_50_cmd,8);
    Serial1.write(Admin_Red_Horn_100_cmd,8);
    Serial1.write(Admin_Red_Horn_130_cmd,8);
  }
    

}

void LCD_ADMIN_KEY_IMAGE(char mode){

  if(mode == OPEN_PRESSED)
    Serial1.write(Admin_Green_Open_cmd,8);
  else if(mode == EXOME_PRESSED)
    Serial1.write(Admin_Green_Exosome_cmd,8);
  else if(mode == WASH_PRESSED)
    Serial1.write(Admin_Green_Wash_cmd,8);
  else if(mode == START_PRESSED)
    Serial1.write(Admin_Green_Start_cmd,8);
  else if(mode == CARGO_PRESSED)
    Serial1.write(Admin_Green_Cargo_cmd,8);
  else if(mode == STOP_PRESSED)
    Serial1.write(Admin_Green_Stop_cmd,8);
  else if(mode == POWER_PRESSED){
    Serial.println("      !!!! admin power key");
    Serial1.write(Admin_Green_Power_cmd,8);
  }

} 
void LCD_BACK_LIGHT_ON(void){   Serial1.write(back_light_on_cmd,7);   }

void LCD_BACK_LIGHT_OFF(void){  
  Serial1.write(P2_exosome_off_cmd,8);
  Serial1.write(P2_cargo_off_cmd,8);
  Serial1.write(P2_wash_off_cmd,8);
  Serial1.write(P2_starilization_off_cmd,8);
  Serial1.write(P2_standby_off_cmd,8);  
  
  Serial1.write(back_light_off_cmd,7);

}

void LCD_END_MODE_IMAGE(char counter){

  Serial1.write(P2_ending_1_cmd,7);

  temp_8_b=0x13+counter%4;
  Serial1.write(temp_8_b);

}

void LCD_TUBE_IMAGE(char mode){

  if( m_current_status.CURRENT_IMAGE == IMAGE_MAIN_DISPLAY){
    if( m_current_status.CURRENT_MODE == MAIN_CARGO_MODE){
      if(mode==YELLOW_COLOR || mode==GREEN_COLOR )
        Serial1.write(P2_tube_on_cmd,8);
      else
        Serial1.write(P2_tube_off_cmd,8);
    }
    else if( m_current_status.CURRENT_MODE == MAIN_EXOSOME_MODE){
      if(mode==RED_COLOR || mode==GREEN_COLOR )
        Serial1.write(P2_tube_on_cmd,8);
      else
        Serial1.write(P2_tube_off_cmd,8);
    }
    else if( m_current_status.CURRENT_MODE == MAIN_WASH_MODE){
      if(mode==BLUE_COLOR || mode==GREEN_COLOR )
        Serial1.write(P2_tube_on_cmd,8);
      else
        Serial1.write(P2_tube_off_cmd,8);
    }
    else{
      if(mode)
        Serial1.write(P2_tube_on_cmd,8);
      else
        Serial1.write(P2_tube_off_cmd,8);

    }
  
  }
}

void LCD_LOGO_IMAGE(void){      Serial1.write(logo_cmd,10);            }

void LCD_WARNING_1_IMAGE(void){      Serial1.write(warning_cmd_1,10);            }
void LCD_WARNING_2_IMAGE(void){      Serial1.write(warning_cmd_2,10);            }


void LCD_STER_ICON_ON(void){
    Serial1.write(P2_starilization_on_cmd,8);
    Serial1.write(P2_center_ddds_cmd,8);
}

void LCD_STER_ICON_OFF(void){
    Serial1.write(P2_starilization_off_cmd,8);
    Serial1.write(P2_center_ddds_cmd,8);
}


void LCD_S_CARGO_IMAGE(void){ 
  Serial1.write(P3_cargo_back_cmd,10);
  LCD_S_CARGO_SET_IMAGE(m_current_set.set_cargo);
}

void LCD_S_EXOSOME_IMAGE(void){
  Serial1.write(P3_exosome_back_cmd,10);
  LCD_S_EXOSOME_SET_IMAGE(m_current_set.set_exosome);
}

void LCD_S_EXOSOME_SET_IMAGE(unsigned char mode){

  if(mode==SET_EXOSOME_LARGE){
    Serial1.write(P3_exosome_large_on_cmd,8);
  }
  else if(mode==SET_EXOSOME_SMALL){
    Serial1.write(P3_exosome_small_on_cmd,8);
  }
}

void LCD_S_CARGO_SET_IMAGE(unsigned char mode){

  if(mode==SET_CARGO_HIGH){
    Serial1.write(P3_cargo_high_on_cmd,8);
  }
  else if(mode==SET_CARGO_MEDIUM){
    Serial1.write(P3_cargo_medium_on_cmd,8);
  }
  else if(mode==SET_CARGO_LOW){
    Serial1.write(P3_cargo_low_on_cmd,8);
  }
}

void LCD_MAIN_IMAGE(void){
  Serial1.write(P2_back_cmd,10);
  Serial1.write(P2_center_ddds_cmd,8);  
  Serial1.write(P2_loading_empty_cmd,8);
}

void LCD_MAIN_MODE_IMAGE(unsigned char mode){

  m_current_status.CURRENT_IMAGE=IMAGE_MAIN_DISPLAY;

  if(mode==ADMIN_MODE){
    Serial1.write(Admin_back_cmd,10); 
    Serial1.write(Admin_Red_Start_cmd,8); 
    Serial1.write(Admin_Red_Cargo_cmd,8); 
    Serial1.write(Admin_Red_Exosome_cmd,8); 
    Serial1.write(Admin_Red_Stop_cmd,8); 
    Serial1.write(Admin_Red_Wash_cmd,8); 
    Serial1.write(Admin_Red_Power_cmd,8); 
    Serial1.write(Admin_Red_Open_cmd,8); 
 
    Serial1.write(Admin_Red_Horn_50_cmd,8); 
    Serial1.write(Admin_Red_Horn_100_cmd,8); 
    Serial1.write(Admin_Red_Horn_130_cmd,8); 
    Serial1.write(Admin_Red_Door_Open_cmd,8); 
    Serial1.write(Admin_Red_Door_Close_cmd,8); 
    
  }
  else{

    Serial1.write(P2_back_cmd,10);
    Serial1.write(P2_exosome_off_cmd,8);
    Serial1.write(P2_cargo_off_cmd,8);
    Serial1.write(P2_wash_off_cmd,8);
    Serial1.write(P2_starilization_off_cmd,8);
    Serial1.write(P2_standby_off_cmd,8);
    Serial1.write(P2_loading_empty_cmd,8);

    if(mode==MAIN_EXOSOME_MODE){
      Serial1.write(P2_exosome_on_cmd,8);
      #ifdef TYPE_A
        Serial1.write(P2_center_exosome_cmd_A,8);
      #else
        Serial1.write(P2_center_exosome_cmd_B,8);
      #endif
      Serial.println("EXOSOME display image 시작");

    }
    else if(mode==MAIN_CARGO_MODE){
      Serial1.write(P2_cargo_on_cmd,8);
      #ifdef TYPE_A
        Serial1.write(P2_center_cargo_cmd_A,8);
      #else
        Serial1.write(P2_center_cargo_cmd_B,8);
      #endif

      Serial.println("CARGO display image 시작");

    }
    else if(mode==MAIN_WASH_MODE){
      Serial1.write(P2_wash_on_cmd,8);
      Serial1.write(P2_center_ddds_cmd,8);
      Serial.println("WASH display image 시작");

    }
    else if(mode==MAIN_STOP_MODE){
      Serial1.write(P2_wash_off_cmd,8);
      Serial1.write(P2_center_ddds_cmd,8);
    }
    else if(mode==MAIN_STOP_WASH_MODE){
      Serial1.write(P2_wash_off_cmd,8);
      Serial1.write(P2_center_ddds_cmd,8);
    }

    else if(mode==MAIN_END_MODE){
      Serial1.write(P2_wash_off_cmd,8);
      Serial1.write(P2_standby_off_cmd,8);
      Serial1.write(P2_center_ddds_cmd,8);
    }
    else if(mode==MAIN_STAND_BY_MODE){
      Serial1.write(P2_center_ddds_cmd,8);
      Serial1.write(P2_standby_on_cmd,8);
    }
  }
}

void LCD_STOP_IMAGE(void){

  Serial1.write(P2_exosome_off_cmd,8);
  Serial1.write(P2_cargo_off_cmd,8);
  Serial1.write(P2_wash_off_cmd,8);
  Serial1.write(P2_starilization_off_cmd,8);
  Serial1.write(P2_standby_on_cmd,8);


}


void LCD_WASH_START(void){

  Serial1.write(P2_loading_10_cmd,7);

  temp_8_b =   m_current_status.CURRENT_WASH_MODE_COUNT * 90 / 100 ;
  
  if(temp_8_b > 0x13) 
    temp_8_b=0x13;

  Serial1.write(temp_8_b);

  m_current_status.CURRENT_WASH_MODE_COUNT++;

}

void LCD_END_UV_START(void){

  Serial1.write(P2_loading_10_cmd,7);

//  temp_8_b =   m_current_status.CURRENT_END_UV_MODE_COUNT / 6;
  

  temp_8_b =   19 -  round(((m_delay_time) / 63));

  if(temp_8_b < 0) 
    temp_8_b=0;

  if(temp_8_b > 0x13) 
    temp_8_b=0x13;

  Serial1.write(temp_8_b);

  m_current_status.CURRENT_END_UV_MODE_COUNT++;


}

void LCD_WASH_UV_START(void){

  Serial1.write(P2_loading_10_cmd,7);

  if(m_current_status.WASH_UV_TIME==120){
    temp_8_b =   19 -  round(((m_delay_time) / 63));
  }
  else{
    temp_8_b =   m_current_status.CURRENT_WASH_UV_MODE_COUNT / (m_current_status.WASH_UV_TIME/20);
  }

  Serial.printf("   !!! UVC START %d %d \r\n",m_delay_time, temp_8_b);

  if(temp_8_b < 0) 
    temp_8_b=0;

  if(temp_8_b > 0x13) 
    temp_8_b=0x13;

  Serial1.write(temp_8_b);

  m_current_status.CURRENT_WASH_UV_MODE_COUNT++;

}

void LCD_CARGO_START(void){


  temp_8=m_current_status.CURRENT_CARGO_MODE_COUNT%2;

      if(temp_8==0){
        #ifdef A_TYPE
          Serial1.write(P2_center_cargo_s2_cmd_A,8);
        #else  
          Serial1.write(P2_center_cargo_s2_cmd_B,8);
        #endif

      }
      else if(temp_8==1){
        #ifdef A_TYPE
          Serial1.write(P2_center_cargo_s1_cmd_A,8);
        #else  
          Serial1.write(P2_center_cargo_s1_cmd_B,8);
        #endif

      }

      Serial1.write(P2_loading_10_cmd,7);

      temp_8_b= 0x00+m_current_status.CURRENT_CARGO_MODE_COUNT*1.1;
      if(temp_8_b > 0x13) 
        temp_8_b=0x13;
      Serial1.write(temp_8_b);

  m_current_status.CURRENT_CARGO_MODE_COUNT++;
 
}

void LCD_CARGO_ICON_START(void){

  temp_8=m_current_status.CURRENT_CARGO_MODE_COUNT%2;

      if(temp_8==0){
        Serial1.write(P2_cargo_off_cmd,8);
      }
      else if(temp_8==1){
        Serial1.write(P2_cargo_on_cmd,8);
      }


  m_current_status.CURRENT_CARGO_MODE_COUNT++;

 }

void LCD_EXOSOME_ICON_START(void){

  temp_8=m_current_status.CURRENT_EXOSOME_MODE_COUNT%2;

      if(temp_8==0){
        Serial1.write(P2_exosome_off_cmd,8);
      }
      else if(temp_8==1){
        Serial1.write(P2_exosome_on_cmd,8);
 
      }


  m_current_status.CURRENT_EXOSOME_MODE_COUNT++;

 }
void LCD_EXOSOME_START(void){


  temp_8=m_current_status.CURRENT_EXOSOME_MODE_COUNT%2;

      if(temp_8==0){
        #ifdef TYPE_A
          Serial1.write(P2_center_exosome_s1_cmd_A,8);
        #else
          Serial1.write(P2_center_exosome_s1_cmd_B,8);
        #endif

      }
      else if(temp_8==1){
        #ifdef TYPE_A
          Serial1.write(P2_center_exosome_s2_cmd_A,8);
        #else 
          Serial1.write(P2_center_exosome_s2_cmd_B,8);
        #endif

      }

      Serial1.write(P2_loading_10_cmd,7);

      temp_8_b= 0x00+ (m_current_status.CURRENT_EXOSOME_MODE_COUNT*1.1);
      if(temp_8_b > 0x13) 
        temp_8_b=0x13;
      Serial1.write(temp_8_b);

  m_current_status.CURRENT_EXOSOME_MODE_COUNT++;
 
}