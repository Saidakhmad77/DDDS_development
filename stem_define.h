

#define EEPROM_SIZE       32
#define eeprom_maxLength  0x20

#define OVER_MOTOR_CURRENT          40
#define UVC_LED_ON                  1
#define UVC_LED_OFF                 2
#define UVC_WORKING_TIME            1200
#define UVC_WORKING_BOOT_TIME       600


#define MAIN_CARGO_MODE             1
#define MAIN_EXOSOME_MODE           2
#define MAIN_WASH_MODE              3
#define MAIN_END_MODE               4
#define MAIN_STAND_BY_MODE          5
#define MAIN_BOOT_MODE              6
#define MAIN_STOP_MODE              7
#define MAIN_STOP_WASH_MODE         8
#define ADMIN_MODE                  9


#define STOP_HORN_TOP                    1
#define STOP_DOOR_OPEN                   2
#define STOP_FINISH                      3
#define STOP_STAND_BY                    0

#define STOP_WASH_HORN_TOP               1
#define STOP_WASH_DOOR_CLOSE             2
#define STOP_WASH_FINISH                 3
#define STOP_WASH_STAND_BY               0


#define WASH_DOOR_OPEN                 1
#define WASH_CHECK_TUBE                2
#define WASH_DOOR_CLOSE                3
#define WASH_HORN_BOTTOM_MOVE          4
#define WASH_ULTRA_ONE                 5
#define WASH_HORN_MID_MOVE             6
#define WASH_ULTRA_TWO                 7
#define WASH_HOLD                      8
#define WASH_HORN_TOP_MOVE             9
#define WASH_DOOR_OPEN_B               10
#define WASH_CHECK_TUBE_B              11
#define WASH_DOOR_CLOSE_B              12
#define WASH_UV_CON                    13
#define WASH_FINISH                    14
#define WASH_STAND_BY                  0

#define CARGO_DOOR_OPEN                1
#define CARGO_CHECK_TUBE               2
#define CARGO_DOOR_CLOSE               3
#define CARGO_HORN_BOTTOM              4
#define CARGO_ULTRA                    5
#define CARGO_HOLD                     6
#define CARGO_HORN_TOP                 7
#define CARGO_FINISH_HOLD              8
#define CARGO_FINISH                   9
#define CARGO_DOOR_OPEN_2              10
#define CARGO_CHECK_TUBE_2             11
#define CARGO_STAND_BY                 0

#define EXOSOME_DOOR_OPEN                1
#define EXOSOME_CHECK_TUBE               2
#define EXOSOME_DOOR_CLOSE               3
#define EXOSOME_HORN_BOTTOM              4
#define EXOSOME_ULTRA                    5
#define EXOSOME_HOLD                     6
#define EXOSOME_HORN_TOP                 7
#define EXOSOME_FINISH_HOLD              8
#define EXOSOME_FINISH                   9
#define EXOSOME_DOOR_OPEN_2              10
#define EXOSOME_CHECK_TUBE_2             11
#define EXOSOME_STAND_BY                 0

#define END_HORN_TOP                     1
#define END_CHECK_TUBE_1                 2
#define END_DOOR_OPEN                    3
#define END_CHECK_TUBE_2                 4
#define END_DOOR_CLOSE                   5
#define END_UV_CON                       6
#define END_FINISH                       7
#define END_STAND_BY                     0

#define BOOT_HORN_TOP                    1
#define BOOT_DOOR_CLOSE                  2
#define BOOT_FINISH                      3
#define BOOT_STAND_BY                    0


#define POWER_ON                  1
#define POWER_OFF                 99
#define POWER_ON_SETTING          10
#define POWER_ON_PROCESSING       20


#define SENSOR_YOU                1
#define SENSOR_MEIYOU             99

#define DOOR_NO_STATUS            0
#define DOOR_OPEN_STATUS          1
#define DOOR_CLOSE_STATUS         99

#define HORN_NO_STATUS            0
#define HORN_POS_50_TOP           50
#define HORN_POS_100_M            100
#define HORN_POS_130_BOTTOM       130


#define DOOR_CMD_FINISHED         0            //
#define DOOR_OPEN_DIR             1            //DOOR OPEN
#define DOOR_CLOSE_DIR            2            //DOOR CLOSE 

#define HORN_CMD_FINISHED         0            //
#define HORN_DOWN_DIR             1            //DOOR OPEN
#define HORN_UP_DIR               2            //DOOR CLOSE 

#define HORN_CMD_DIR_50          3            //DOOR OPEN
#define HORN_CMD_DIR_100         4            //DOOR OPEN
#define HORN_CMD_DIR_130         5            //DOOR OPEN



#define RED_COLOR             0x01
#define GREEN_COLOR           0x02
#define BLUE_COLOR            0x04
#define WHITE_COLOR           0x07
#define JADE_COLOR            0x06
#define YELLOW_COLOR          0x03
#define VIOLET_COLOR          0x05

#define NONE_COLOR            0x00
#define NO_TUBE               0x00


#define IMAGE_LOGO_DISPLAY        1
#define IMAGE_MAIN_DISPLAY        2
#define IMAGE_S_CARGO_DISPLAY     3
#define IMAGE_S_EXOSOME_DISPLAY   4
#define IMAGE_WARNING_DISPLAY     5

#define SET_CARGO_LOW             1
#define SET_CARGO_MEDIUM          2
#define SET_CARGO_HIGH            3

#define SET_EXOSOME_LARGE         2
#define SET_EXOSOME_SMALL         1


typedef struct{
  unsigned char door_status; 
  unsigned char tube_status; 
  unsigned char process_status; 
  unsigned char process_to_be; 
} MODE_STATE; 

typedef struct{

  unsigned char set_cargo;
  unsigned char set_exosome;
  unsigned char set_wash;
  unsigned char cargo_time;
  unsigned char cargo_level;
  unsigned char exosome_time;
  unsigned char exosome_level;
  unsigned char wash_A_time;
  unsigned char wash_A_level;
  unsigned char wash_B_time;
  unsigned char wash_B_level;

} SET_STATE;


typedef struct{

  unsigned char CURRENT_STATE;
  unsigned char CURRENT_IMAGE;
  unsigned char CURRENT_MODE;
  unsigned char WASH_UV_TIME;

  unsigned char   CURRENT_EXOSOME_MODE_COUNT;
  unsigned char   CURRENT_CARGO_MODE_COUNT;
  unsigned char   CURRENT_WASH_MODE_COUNT;
  unsigned char   CURRENT_WASH_UV_MODE_COUNT;
  unsigned char   CURRENT_END_UV_MODE_COUNT;
  

  unsigned char door_motor_position;
  unsigned char horn_motor_position;

  unsigned char rgb_sensor;
  unsigned char dac_sensor;


  unsigned char logo_image_count;

} CURRENT_MODE;
