#include <Arduino.h>

void WASH_ULTRA_START(char mode){
  if(mode==1)
    m_set_DAC=  76;     //60%
  else
    m_set_DAC=  52;     //20%
  ULTRA_START();    

}
void CARGO_ULTRA_START(void){
  m_set_DAC=40+0.6*m_current_set.cargo_level;
  ULTRA_START();
}

void EXOSOME_ULTRA_START(void){
  m_set_DAC=40+0.6*m_current_set.exosome_level;
  ULTRA_START();
}

void ULTRA_START(void){
  digitalWrite(ULTRA_PWR_EN_1,   HIGH);
  digitalWrite(ULTRA_PWR_EN_2,   HIGH);

}

void ULTRA_STOP(void){
  digitalWrite(ULTRA_PWR_EN_1,   LOW);
  digitalWrite(ULTRA_PWR_EN_2,   LOW);

}

void HORN_MOTOR_CLOCK(char mode){

  Serial.println("      !!! start horn motor operation ");
  if(mode==HORN_DOWN_DIR){
    ledcWrite(HORN_MOTOR_PWM,   0);
    ledcWrite(HORN_MOTOR_DIR,   180);     //180
  }
  else if(mode==HORN_UP_DIR){
    ledcWrite(HORN_MOTOR_PWM,    180);    //180
    ledcWrite(HORN_MOTOR_DIR,    0);
  }

}

void DOOR_MOTOR_CLOCK(char mode){


  if(mode==DOOR_OPEN_DIR){
    m_motor_start_open_cmd=1;

    ledcWrite(DOOR_MOTOR_PWM,    0);
    ledcWrite(DOOR_MOTOR_DIR,    80);

  }
  else if(mode==DOOR_CLOSE_DIR){
    m_motor_start_close_cmd=1;

    ledcWrite(DOOR_MOTOR_PWM,    80);
    ledcWrite(DOOR_MOTOR_DIR,    0);
  }
}

void HORN_MOTOR_STOP(void){

  ledcWrite(HORN_MOTOR_PWM,    255);
  ledcWrite(HORN_MOTOR_DIR,    255);
  ledcWrite(HORN_MOTOR_PWM,    0);
  ledcWrite(HORN_MOTOR_DIR,    0);

}

void DOOR_MOTOR_STOP(void){

  if(m_door_motor_cmd==DOOR_OPEN_DIR){
    Serial.printf("         !!! motor open count = %d \r\n", m_door_motor_open_counter);
    m_door_motor_open_counter=0;
    m_motor_start_open_cmd=0;
  }
  if(m_door_motor_cmd==DOOR_CLOSE_DIR){
    Serial.printf("         !!! motor close count = %d \r\n", m_door_motor_close_counter);
    m_door_motor_close_counter=0;
    m_motor_start_close_cmd=0;
  }
 
  ledcWrite(DOOR_MOTOR_PWM,    255);
  ledcWrite(DOOR_MOTOR_DIR,    255);
  ledcWrite(DOOR_MOTOR_PWM,    0);
  ledcWrite(DOOR_MOTOR_DIR,    0);

}

unsigned char CHECK_HORN_STATUS(void){

  HORN_STATUS=HORN_NO_STATUS;

  if(digitalRead(HORN_50))
   HORN_STATUS=HORN_POS_50_TOP;

  if(digitalRead(HORN_100))
   HORN_STATUS=HORN_POS_100_M;

  if(digitalRead(HORN_130)) 
   HORN_STATUS=HORN_POS_130_BOTTOM;
 
  return HORN_STATUS;

}

unsigned char CHECK_DOOR_STATUS(void){
  
  DOOR_STATUS= DOOR_NO_STATUS;

  if(digitalRead(DOOR_OPEN))
    DOOR_STATUS = DOOR_OPEN_STATUS; 
  
  if(digitalRead(DOOR_CLOSE))
    DOOR_STATUS = DOOR_CLOSE_STATUS; 

  return DOOR_STATUS;

}


void CHECK_DOOR_MOTOR_STATUS(void){

  if(m_door_motor_cmd==DOOR_OPEN_DIR){
    if(digitalRead(DOOR_OPEN)){
      DOOR_MOTOR_STOP();
      m_door_motor_cmd=DOOR_CMD_FINISHED;
      Serial.println("문 열기 완료");
      DOOR_STATUS=CHECK_DOOR_STATUS();
 
      if(m_current_status.CURRENT_MODE == MAIN_WASH_MODE){
        if(m_wash_state.process_to_be == WASH_DOOR_OPEN){
           m_wash_state.process_to_be = WASH_CHECK_TUBE; 
        }
        else if(m_wash_state.process_to_be == WASH_DOOR_OPEN_B){
           m_wash_state.process_to_be = WASH_CHECK_TUBE_B; 
        }

      }
 
      else if(m_current_status.CURRENT_MODE == MAIN_END_MODE){
         if(m_end_state.process_to_be == END_DOOR_OPEN){ 
          m_end_state.process_to_be = END_CHECK_TUBE_2;
         }
      } 
      else if(m_current_status.CURRENT_MODE == MAIN_STOP_MODE){
         if(m_stop_state.process_to_be == STOP_DOOR_OPEN){ 
          m_stop_state.process_to_be = STOP_FINISH;
         }
      } 
      else if(m_current_status.CURRENT_MODE == MAIN_CARGO_MODE){
       if(m_cargo_state.process_to_be == CARGO_DOOR_OPEN){ 
        m_cargo_state.process_to_be = CARGO_CHECK_TUBE;
       }
       else if(m_cargo_state.process_to_be == CARGO_DOOR_OPEN_2){ 
        m_cargo_state.process_to_be = CARGO_CHECK_TUBE_2;
       }
       
      } 
      else if(m_current_status.CURRENT_MODE == MAIN_EXOSOME_MODE){
       if(m_exosome_state.process_to_be == EXOSOME_DOOR_OPEN){ 
        m_exosome_state.process_to_be = EXOSOME_CHECK_TUBE;
       }
       else if(m_exosome_state.process_to_be == EXOSOME_DOOR_OPEN_2){ 
        m_exosome_state.process_to_be = EXOSOME_CHECK_TUBE_2;
       }

      } 

    
    }
  }

  if(m_door_motor_cmd==DOOR_CLOSE_DIR){
    if(digitalRead(DOOR_CLOSE)){
      DOOR_MOTOR_STOP();
      m_door_motor_cmd=DOOR_CMD_FINISHED;
      Serial.println("문 닫기 완료");
      DOOR_STATUS=CHECK_DOOR_STATUS();

     if(m_current_status.CURRENT_MODE == MAIN_WASH_MODE){
        if(m_wash_state.process_to_be == WASH_DOOR_CLOSE){
           m_wash_state.process_to_be = WASH_HORN_BOTTOM_MOVE; 
        }
        else if(m_wash_state.process_to_be == WASH_DOOR_CLOSE_B){
           m_wash_state.process_to_be = WASH_UV_CON; 
        }
 
      }
      else if(m_current_status.CURRENT_MODE == MAIN_END_MODE){
         if(m_end_state.process_to_be == END_DOOR_CLOSE){ 
          m_end_state.process_to_be = END_UV_CON;
         }
      } 
      else if(m_current_status.CURRENT_MODE == MAIN_BOOT_MODE){
         if(m_boot_state.process_to_be == BOOT_DOOR_CLOSE){ 
          m_boot_state.process_to_be = BOOT_FINISH;
         }
      } 
      else if(m_current_status.CURRENT_MODE == MAIN_STOP_WASH_MODE){
         if(m_stop_wash_state.process_to_be == STOP_WASH_DOOR_CLOSE){ 
          m_stop_wash_state.process_to_be = STOP_WASH_FINISH;
         }
      } 

      else if(m_current_status.CURRENT_MODE == MAIN_CARGO_MODE){
         if(m_cargo_state.process_to_be == CARGO_DOOR_CLOSE){ 
          m_cargo_state.process_to_be = CARGO_HORN_BOTTOM;
         }
      } 
      else if(m_current_status.CURRENT_MODE == MAIN_EXOSOME_MODE){
         if(m_exosome_state.process_to_be == EXOSOME_DOOR_CLOSE){ 
          m_exosome_state.process_to_be = EXOSOME_HORN_BOTTOM;
         }
      } 
 
    }
  }

}

void CHECK_HORN_MOTOR_STATUS(void){


  if(m_horn_motor_cmd==HORN_CMD_DIR_100){
    if(digitalRead(HORN_100)){
      HORN_MOTOR_STOP();
      m_horn_motor_cmd=HORN_CMD_FINISHED;
      Serial.println("혼 100_Middie 위치 완료");
      HORN_STATUS=CHECK_HORN_STATUS();
      m_horn_pos=2;

     if(m_current_status.CURRENT_MODE == MAIN_WASH_MODE){
        if(m_wash_state.process_to_be == WASH_HORN_MID_MOVE){
           m_wash_state.process_to_be = WASH_ULTRA_TWO; 
        }

      }


    }
  }

  if(m_horn_motor_cmd==HORN_CMD_DIR_130){
    if(digitalRead(HORN_130)){
      HORN_MOTOR_STOP();
      m_horn_motor_cmd=HORN_CMD_FINISHED;
      Serial.println("혼 130_BOTTOM 위치 완료");
      m_horn_pos=3;
      HORN_STATUS=CHECK_HORN_STATUS();
      if(digitalRead(HORN_130)){
        Serial.printf("status %d 혼 130_BOTTOM 위치 완료 AGAIN \r\n",HORN_STATUS);
      }

     if(m_current_status.CURRENT_MODE == MAIN_WASH_MODE){
        if(m_wash_state.process_to_be == WASH_HORN_BOTTOM_MOVE){
           m_wash_state.process_to_be = WASH_ULTRA_ONE; 
        }
     }
     else if(m_current_status.CURRENT_MODE == MAIN_CARGO_MODE){
        if(m_cargo_state.process_to_be == CARGO_HORN_BOTTOM){
           m_cargo_state.process_to_be = CARGO_ULTRA; 
        }
     }
     else if(m_current_status.CURRENT_MODE == MAIN_EXOSOME_MODE){
        if(m_exosome_state.process_to_be == EXOSOME_HORN_BOTTOM){
           m_exosome_state.process_to_be = EXOSOME_ULTRA; 
        }
     }

    }


  }

  if(m_horn_motor_cmd==HORN_CMD_DIR_50){
    if(digitalRead(HORN_50)){
      HORN_MOTOR_STOP();
      m_horn_motor_cmd=HORN_CMD_FINISHED;
      Serial.println("혼 50_TOP 위치 완료");
      m_horn_pos=1;
      HORN_STATUS=CHECK_HORN_STATUS();

     if(m_current_status.CURRENT_MODE == MAIN_WASH_MODE){
        if(m_wash_state.process_to_be == WASH_HORN_TOP_MOVE){
           m_wash_state.process_to_be = WASH_DOOR_OPEN_B; 
        }
      }

     else if(m_current_status.CURRENT_MODE == MAIN_END_MODE){
        if(m_end_state.process_to_be == END_HORN_TOP){
           m_end_state.process_to_be = END_CHECK_TUBE_1; 
        }
      }
     else if(m_current_status.CURRENT_MODE == MAIN_BOOT_MODE){
        if(m_boot_state.process_to_be == BOOT_HORN_TOP){
           m_boot_state.process_to_be = BOOT_DOOR_CLOSE; 
        }
      }
     else if(m_current_status.CURRENT_MODE == MAIN_STOP_MODE){
        if(m_stop_state.process_to_be == STOP_HORN_TOP){
           m_stop_state.process_to_be = STOP_DOOR_OPEN; 
        }
      }
     else if(m_current_status.CURRENT_MODE == MAIN_STOP_WASH_MODE){
        if(m_stop_wash_state.process_to_be == STOP_WASH_HORN_TOP){
           m_stop_wash_state.process_to_be = STOP_WASH_DOOR_CLOSE; 
        }
      }
     else if(m_current_status.CURRENT_MODE == MAIN_CARGO_MODE){
        if(m_cargo_state.process_to_be == CARGO_HORN_TOP){
           m_cargo_state.process_to_be = CARGO_FINISH_HOLD; 
        }
      }
     else if(m_current_status.CURRENT_MODE == MAIN_EXOSOME_MODE){
        if(m_exosome_state.process_to_be == EXOSOME_HORN_TOP){
           m_exosome_state.process_to_be = EXOSOME_FINISH_HOLD; 
        }
      }

    }
  }

}
