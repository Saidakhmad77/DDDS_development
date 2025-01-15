#include <Arduino.h>
#include <EEPROM.h>

const char* getStatusString_MODE(int status) {
    switch (status) {
        case MAIN_CARGO_MODE:
            return "MAIN_CARGO_MODE";
        case MAIN_EXOSOME_MODE:
            return "MAIN_EXOSOME_MODE";
        case MAIN_STAND_BY_MODE:
            return "MAIN_STAND_BY_MODE";
        case MAIN_WASH_MODE:
            return "MAIN_WASH_MODE";
        case MAIN_STOP_MODE:
            return "MAIN_STOP_MODE";
        case MAIN_END_MODE:
            return "MAIN_END_MODE";
        case ADMIN_MODE:
            return "ADMIN_MODE";

        default:
            return "UNKNOWN MODE";
    }
}

const char* getStatusString_STATE(int status) {
    switch (status) {
        case POWER_ON:
            return "POWER_ON";
        case POWER_ON_SETTING:
            return "POWER_ON_SETTING";
        case POWER_OFF:
            return "POWER_OFF";
        case POWER_ON_PROCESSING:
            return "POWER_ON_PROCESSING";

        default:
            return "UNKNOWN STATE";
    }
}

const char* getStatusString_DOOR(int status) {
    switch (status) {
        case DOOR_NO_STATUS:
            return "Door between";
        case DOOR_OPEN_STATUS:
            return "OPEN";
        case DOOR_CLOSE_STATUS:
            return "CLOSE";

        default:
            return "UNKNOWN STATE";
    }
}

const char* getStatusString_HORN(int status) {
    switch (status) {
        case HORN_NO_STATUS:
            return "horn between";
        case HORN_POS_50_TOP:
            return "TOP";
        case HORN_POS_100_M:
            return "MIDDLE";
        case HORN_POS_130_BOTTOM:
            return "BOTTOM";

        default:
            return "UNKNOWN STATE";
    }
}

const char* getStatusString_COLOR(int status) {
    switch (status) {
        case RED_COLOR:
            return "RED_TUBE";
        case GREEN_COLOR:
            return "GREEN_TUBE";
        case BLUE_COLOR:
            return "BLUE_TUBE";
        case YELLOW_COLOR:
            return "YELLOW TUBE";

        default:
            return "NO TUBE";
    }
}

void INIT_EEPROM(void){

   if (!EEPROM.begin(EEPROM_SIZE)) {

    Serial.println("EEPROM initialization failed. Retrying...");
    EEPROM.end(); // 리소스 해제
    delay(100); 
    if (!EEPROM.begin(EEPROM_SIZE)) {
      Serial.println("EEPROM initialization failed again.");
      EEPROM_FLAG=0;
      
    } else {
      EEPROM_FLAG=1;
      Serial.println("EEPROM initialized successfully.");
    }
  } else {
    EEPROM_FLAG=1;
    Serial.println("EEPROM initialized successfully.");
  }

}


void find_color(uint16_t r, uint16_t g, uint16_t b){

  if( r>1350 && g>900)
      m_tube_color = YELLOW_COLOR; 
  else if(b > 370 && g < 520)
      m_tube_color = BLUE_COLOR; 
  else if( r<650 && g>500 && b<400)
      m_tube_color = GREEN_COLOR; 
  else if( r>1000 && g<550)
      m_tube_color = RED_COLOR; 
  else 
      m_tube_color = NO_TUBE;
}


// 플래시에 구조체 저장
void saveToFlash(void) {
  if(EEPROM_FLAG==1){
    EEPROM.put(0, m_current_set);  // EEPROM의 시작 주소에 구조체 저장
    Serial.println("저장 완료");
    EEPROM.commit(); 
  }
}

// 플래시에서 구조체 읽기
void readFromFlash() {

  if(EEPROM_FLAG==1){
    EEPROM.get(0, m_current_set);  // EEPROM의 시작 주소에서 구조체 읽기
  }

    if(m_current_set.set_cargo > SET_CARGO_HIGH ){
      m_current_set.set_cargo     = SET_CARGO_LOW;
      m_current_set.cargo_time    = 10;
      m_current_set.cargo_level   = 20;
    }
    else{
      if(m_current_set.set_cargo==SET_CARGO_LOW){
        m_current_set.cargo_time    = 10;
        m_current_set.cargo_level   = 20;
      }
      else if(m_current_set.set_cargo==SET_CARGO_MEDIUM){
        m_current_set.cargo_time    = 10;
        m_current_set.cargo_level   = 25;
      }
      else {
        m_current_set.cargo_time    = 10;
        m_current_set.cargo_level   = 30;
      }

    }

    if(m_current_set.set_exosome > SET_EXOSOME_SMALL){
      m_current_set.set_exosome     = SET_EXOSOME_LARGE;
      m_current_set.exosome_time    = 20;
      m_current_set.exosome_level   = 20;
    }
    else{
      if(m_current_set.set_cargo==SET_EXOSOME_LARGE){
        m_current_set.exosome_time    = 20;
        m_current_set.exosome_level   = 20;
      }
      else {
        m_current_set.exosome_time    = 10;
        m_current_set.exosome_level   = 20;
      }
    }
}
