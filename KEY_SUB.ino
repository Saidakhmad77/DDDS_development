

#define 	ThresholdLongKey  40
#define 	ThresholdShortKey 2

#define 	OPEN_PRESSED      0x7E
#define 	CARGO_PRESSED     0x7D
#define 	WASH_PRESSED      0x7B
#define 	EXOME_PRESSED     0x77
#define 	START_PRESSED     0x6F
#define 	STOP_PRESSED      0x5F
#define 	POWER_PRESSED     0x3F

#define 	POWER_STOP_PRESSED      0x1F

uint16_t	CountPushDownButten = 0;
uint16_t CountInitCount = 0;
uint16_t	PushDownLongKeyFlag, PushDownShortKeyFlag;
uint16_t	OldStateKeyPad = 0; 

uint8_t StateKeyPad=0;
extern char  current_display_mode;  //0:speed 1: distance 
extern float m_distance_meter;


char debug_msg[64];
unsigned short m_dim_led_counter=0;


void ReadKeyPad(void)
{


  if(m_uvc_led==UVC_LED_ON)
    return;


		StateKeyPad = (digitalRead(KEY_7)<<6)+(digitalRead(KEY_6)<<5)+(digitalRead(KEY_5)<<4)+(digitalRead(KEY_4)<<3)+(digitalRead(KEY_3)<<2)+(digitalRead(KEY_2)<<1)+digitalRead(KEY_1);

//    sprintf(debug_msg,"key = %x %d",StateKeyPad,StateKeyPad);
///    Serial.println(debug_msg);

		if (StateKeyPad == OldStateKeyPad && StateKeyPad != 0x7F ){
		
			CountPushDownButten++;
		
			if (CountPushDownButten == ThresholdLongKey ){
				PushDownLongKeyFlag = 1;
			}
			else{
				PushDownLongKeyFlag = 0;
			}

			if (PushDownLongKeyFlag){
				//LONG KEY 
				switch(OldStateKeyPad){
          case POWER_STOP_PRESSED:

          Serial.println("POWER_STOP KEY IS PRESSED");


          if(m_current_status.CURRENT_MODE==ADMIN_MODE)
            GO_TO_STAND_BY_MODE(MAIN_STAND_BY_MODE);
          else{
           if(m_current_status.CURRENT_MODE==MAIN_STAND_BY_MODE)
              GO_TO_ADMIN_MODE();
           }

          break;


					case OPEN_PRESSED:		//POWER BUTTON
            if(m_current_status.CURRENT_MODE==ADMIN_MODE){

              if(m_current_status.horn_motor_position==HORN_POS_50_TOP){
                if(DOOR_STATUS==DOOR_CLOSE_STATUS){
                  DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR); 
                  m_door_motor_cmd = DOOR_OPEN_DIR;
                }
                else {
                  DOOR_MOTOR_CLOCK(DOOR_CLOSE_DIR);
                  m_door_motor_cmd = DOOR_CLOSE_DIR;
                } 
              }
            }
          break;

          case WASH_PRESSED:
          
            if(m_current_status.CURRENT_MODE==ADMIN_MODE){
               if(m_current_status.horn_motor_position==HORN_POS_50_TOP){
                  HORN_MOTOR_CLOCK(HORN_DOWN_DIR);
                  m_horn_motor_cmd=HORN_CMD_DIR_130;
               } 
               else if(m_current_status.horn_motor_position==HORN_POS_130_BOTTOM || m_horn_pos==3){
                  HORN_MOTOR_CLOCK(HORN_UP_DIR);
                  m_horn_motor_cmd=HORN_CMD_DIR_100;
               } 
               else if(m_current_status.horn_motor_position==HORN_POS_100_M){
                  HORN_MOTOR_CLOCK(HORN_UP_DIR);
                  m_horn_motor_cmd=HORN_CMD_DIR_50;
               } 

            }
            else if(m_current_status.CURRENT_MODE == MAIN_WASH_MODE){
              if(m_uvc_led==UVC_LED_ON){
                Serial.println("wash mode long key -- but UV 동작 중 ");
              }
              else{
                m_delay_time=0;
                ULTRA_STOP();
                m_current_status.CURRENT_MODE = MAIN_STOP_WASH_MODE;
                LCD_MAIN_MODE_IMAGE(MAIN_STOP_WASH_MODE);
                led_control();
                m_stop_wash_state.process_to_be =    STOP_WASH_HORN_TOP;          
                  
              }
            }

          break;

					case POWER_PRESSED:		//POWER BUTTON

            if(m_current_status.CURRENT_MODE==ADMIN_MODE){
                LCD_MAIN_MODE_IMAGE( m_current_status.CURRENT_MODE);
                return;
            }

            if(m_current_status.CURRENT_STATE==POWER_OFF){
              Serial.println("POWER LONG LIGHT ON");
              LCD_BACK_LIGHT_ON();
              LCD_LOGO_IMAGE();
   

              m_current_status.logo_image_count = 3;
              m_current_status.CURRENT_STATE=   POWER_ON;
              m_current_status.CURRENT_IMAGE=   IMAGE_LOGO_DISPLAY;

            }

            else if(m_current_status.CURRENT_STATE==POWER_ON){
              Serial.println("POWER LONG LIGHT OFF");
              m_current_status.CURRENT_STATE=POWER_ON_PROCESSING;
              m_current_status.CURRENT_MODE = MAIN_END_MODE;
              m_end_state.process_to_be = END_HORN_TOP;
              LCD_MAIN_MODE_IMAGE( m_current_status.CURRENT_MODE);
              led_control();

            }

          break;
				
				} //switch
			}//PushDownLongKeyFlag
		}//
		else{
				if (CountPushDownButten > ThresholdShortKey && CountPushDownButten < ThresholdLongKey)
				{
					PushDownShortKeyFlag = 1;
				}
				else
				{
    				PushDownShortKeyFlag = 0;
				}

			 	CountPushDownButten = 0;

				if (PushDownShortKeyFlag){
				
					//Short KEY 
					switch(OldStateKeyPad){
						case OPEN_PRESSED:		//POWER BUTTON
              Serial.println("OPEN_CLOSE button");


             if(m_current_status.CURRENT_MODE==ADMIN_MODE){
                LCD_ADMIN_KEY_IMAGE(OPEN_PRESSED); 
                return;
             }

             if(m_current_status.CURRENT_STATE==POWER_ON){
              if(DOOR_STATUS==DOOR_CLOSE_STATUS){
                DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR); 
                m_door_motor_cmd = DOOR_OPEN_DIR;
              }
              else {
                DOOR_MOTOR_CLOCK(DOOR_CLOSE_DIR);
                m_door_motor_cmd = DOOR_CLOSE_DIR;
              } 
             }            

             if(m_current_status.CURRENT_MODE== MAIN_WASH_MODE){

                if(m_wash_state.process_to_be == WASH_DOOR_OPEN_B){
                  DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR); 
                  m_door_motor_cmd = DOOR_OPEN_DIR;
                } 
             } 

             if(m_current_status.CURRENT_MODE== MAIN_EXOSOME_MODE){

                if(m_exosome_state.process_to_be == EXOSOME_DOOR_OPEN_2){
                  DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR); 
                  m_door_motor_cmd = DOOR_OPEN_DIR;
                } 
             } 

             if(m_current_status.CURRENT_MODE== MAIN_CARGO_MODE){

                if(m_cargo_state.process_to_be == CARGO_DOOR_OPEN_2){
                  DOOR_MOTOR_CLOCK(DOOR_OPEN_DIR); 
                  m_door_motor_cmd = DOOR_OPEN_DIR;
                } 
             } 

						break;		

 						case EXOME_PRESSED:		//POWER BUTTON
              Serial.println("EXOME button");

             if(m_current_status.CURRENT_MODE==ADMIN_MODE){
                 LCD_ADMIN_KEY_IMAGE(EXOME_PRESSED); 
                return;
             }


              //HORN_MOTOR_CLOCK(HORN_DOWN_DIR);
              //m_horn_motor_cmd=HORN_DOWN_DIR_100;

              if(m_current_status.CURRENT_STATE==POWER_ON_SETTING){
                
                if(m_current_status.CURRENT_IMAGE==IMAGE_S_CARGO_DISPLAY){
                  m_current_set.set_cargo--;
                  m_current_set.set_cargo = (m_current_set.set_cargo < SET_CARGO_LOW) ? SET_CARGO_LOW : m_current_set.set_cargo;
                  LCD_S_CARGO_SET_IMAGE(m_current_set.set_cargo);
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
                else{
                  m_current_set.set_exosome--;  
                  m_current_set.set_exosome = (m_current_set.set_exosome < SET_EXOSOME_SMALL) ? SET_EXOSOME_SMALL : m_current_set.set_exosome;
                  LCD_S_EXOSOME_SET_IMAGE(m_current_set.set_exosome);
                  if(m_current_set.set_exosome==SET_EXOSOME_LARGE){
                    m_current_set.exosome_time    = 20;
                    m_current_set.exosome_level   = 20;
                    Serial.println("      !!! TIME 20");
                  }
                  else {
                    m_current_set.exosome_time    = 10;
                    m_current_set.exosome_level   = 20;
                    Serial.println("      !!1 TIME 10");
                  }

                }
              
              }
              else if(m_current_status.CURRENT_STATE==POWER_ON){
                m_current_status.CURRENT_MODE = MAIN_EXOSOME_MODE;
                m_current_status.CURRENT_EXOSOME_MODE_COUNT=0;
                LCD_MAIN_MODE_IMAGE( m_current_status.CURRENT_MODE);
                led_control();
              }


            break;		

 						case WASH_PRESSED:		//POWER BUTTON
              Serial.println("WASH button");

             if(m_current_status.CURRENT_MODE==ADMIN_MODE){
                 LCD_ADMIN_KEY_IMAGE(WASH_PRESSED); 
                return;
             }


              if(m_current_status.CURRENT_STATE==POWER_ON){
                m_current_status.CURRENT_MODE = MAIN_WASH_MODE;
                m_current_status.CURRENT_WASH_MODE_COUNT=0;
                LCD_MAIN_MODE_IMAGE( m_current_status.CURRENT_MODE);
                led_control();
              }

              else if(m_current_status.CURRENT_STATE==POWER_ON_SETTING){
                m_current_status.CURRENT_STATE=POWER_ON;
                LCD_MAIN_MODE_IMAGE( m_current_status.CURRENT_MODE);
              }

						break;		

 						case CARGO_PRESSED:		//POWER BUTTON
              Serial.println("CARGO button");

             if(m_current_status.CURRENT_MODE==ADMIN_MODE){
                 LCD_ADMIN_KEY_IMAGE(CARGO_PRESSED); 
                return;
             }

              //HORN_MOTOR_CLOCK(HORN_DOWN_DIR);
              //m_horn_motor_cmd=HORN_DOWN_DIR_130;

              if(m_current_status.CURRENT_STATE==POWER_ON_SETTING){

                Serial.println("power on setting");

                if(m_current_status.CURRENT_IMAGE==IMAGE_S_CARGO_DISPLAY){
                  m_current_set.set_cargo++;
                  m_current_set.set_cargo = (m_current_set.set_cargo > SET_CARGO_HIGH) ? SET_CARGO_HIGH : m_current_set.set_cargo;
                  LCD_S_CARGO_SET_IMAGE(m_current_set.set_cargo);
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
                else{
                  m_current_set.set_exosome++;  
                  m_current_set.set_exosome = (m_current_set.set_exosome > SET_EXOSOME_LARGE) ? SET_EXOSOME_LARGE : m_current_set.set_exosome;
                  LCD_S_EXOSOME_SET_IMAGE(m_current_set.set_exosome);
                  if(m_current_set.set_exosome==SET_EXOSOME_LARGE){
                    m_current_set.exosome_time    = 20;
                    m_current_set.exosome_level   = 20;
                    Serial.println("      !!! TIME 20");
                  }
                  else {
                    m_current_set.exosome_time    = 10;
                    m_current_set.exosome_level   = 20;
                    Serial.println("      !!1 TIME 10");
 
                  }
                
                }
              
              }
              if(m_current_status.CURRENT_STATE==POWER_ON){
                 m_current_status.CURRENT_MODE = MAIN_CARGO_MODE;
                 m_current_status.CURRENT_CARGO_MODE_COUNT=0;
                 LCD_MAIN_MODE_IMAGE( m_current_status.CURRENT_MODE);
                 led_control();
              }

						break;		

 						case START_PRESSED:		//POWER BUTTON
              Serial.println("start button");

             if(m_current_status.CURRENT_MODE==ADMIN_MODE){
                 LCD_ADMIN_KEY_IMAGE(START_PRESSED); 
                return;
             }

              if(m_current_status.CURRENT_STATE==POWER_ON){
                if(m_current_status.CURRENT_MODE==MAIN_CARGO_MODE){ 
                  LCD_S_CARGO_IMAGE();
                  m_current_status.CURRENT_IMAGE=   IMAGE_S_CARGO_DISPLAY;
                  m_current_status.CURRENT_STATE=   POWER_ON_SETTING;
                }

                if(m_current_status.CURRENT_MODE==MAIN_EXOSOME_MODE){
                  LCD_S_EXOSOME_IMAGE();
                  m_current_status.CURRENT_IMAGE=   IMAGE_S_EXOSOME_DISPLAY;
                  m_current_status.CURRENT_STATE=   POWER_ON_SETTING;
                }

                if(m_current_status.CURRENT_MODE==MAIN_WASH_MODE){
                  m_current_status.CURRENT_STATE=   POWER_ON_PROCESSING;
                  led_control();
                  m_wash_state.process_to_be = WASH_DOOR_OPEN;

                }

              }

              else if(m_current_status.CURRENT_STATE==POWER_ON_SETTING){
                if(m_current_status.CURRENT_MODE==MAIN_CARGO_MODE){ 
                  m_current_status.CURRENT_STATE=   POWER_ON_PROCESSING;
                  m_current_status.CURRENT_IMAGE=   IMAGE_MAIN_DISPLAY;
                  m_current_status.CURRENT_MODE =   MAIN_CARGO_MODE;
                  m_cargo_state.process_to_be =     CARGO_DOOR_OPEN;
                  LCD_MAIN_MODE_IMAGE(MAIN_CARGO_MODE);
                  led_control();
                }

                if(m_current_status.CURRENT_MODE==MAIN_EXOSOME_MODE){
                  m_current_status.CURRENT_STATE=   POWER_ON_PROCESSING;
                  m_current_status.CURRENT_IMAGE=   IMAGE_MAIN_DISPLAY;
                  m_current_status.CURRENT_MODE =   MAIN_EXOSOME_MODE;
                  m_exosome_state.process_to_be =     EXOSOME_DOOR_OPEN;
                  LCD_MAIN_MODE_IMAGE(MAIN_EXOSOME_MODE);
                  led_control();
                }
              }





						break;		

 						case STOP_PRESSED:		//POWER BUTTON
              Serial.println("STOP button");

             if(m_current_status.CURRENT_MODE==ADMIN_MODE){
                 LCD_ADMIN_KEY_IMAGE(STOP_PRESSED); 
                return;
             }

              if(m_uvc_led==UVC_LED_ON){
                Serial.println("    !!! UVC 동작 중");
                return;
              }

              DOOR_MOTOR_STOP();
              HORN_MOTOR_STOP();
              ULTRA_STOP();
              digitalWrite(UV_CON_LED,LOW);             //UVC LED OFF
              m_delay_time=0;

              if(m_current_status.CURRENT_STATE != POWER_OFF){
                m_current_status.CURRENT_STATE= POWER_ON_PROCESSING;
                m_current_status.CURRENT_MODE = MAIN_STOP_MODE;
                LCD_MAIN_MODE_IMAGE(MAIN_STOP_MODE);
                led_control();
                m_stop_state.process_to_be =    STOP_HORN_TOP;
              
              }

                //WASH STOP
                m_wash_state.process_to_be  =   WASH_STAND_BY;
                digitalWrite(UV_CON_LED,LOW);             //LED OFF

                m_cargo_state.process_to_be   = CARGO_STAND_BY;
                m_exosome_state.process_to_be = EXOSOME_STAND_BY;

              //ULTRA_START();
              //m_set_DAC=70;

						break;		

 						case POWER_PRESSED:		//POWER BUTTON
              Serial.println("Power  button");

             if(m_current_status.CURRENT_MODE==ADMIN_MODE){
                Serial.println("Power  button");
                LCD_ADMIN_KEY_IMAGE(POWER_PRESSED); 
                return;
             }

              //ULTRA_START();
             // m_set_DAC=90;
						break;		

			 		}

			}
				
		} //else 

		OldStateKeyPad = StateKeyPad;	
}
