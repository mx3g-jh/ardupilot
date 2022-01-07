#include "Copter.h"

#ifdef USERHOOK_INIT
void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
}
#endif

#ifdef USERHOOK_FASTLOOP
void Copter::userhook_FastLoop()
{
    // put your 100Hz code here
}
#endif

#ifdef USERHOOK_50HZLOOP
void Copter::userhook_50Hz()
{
    // put your 50Hz code here
}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void Copter::userhook_MediumLoop()
{
    // put your 10Hz code here
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
    // put your 1Hz code here
}
#endif

#ifdef USERHOOK_AUXSWITCH
void Copter::userhook_auxSwitch1(uint8_t ch_flag)
{

    nowvideo_ms = AP_HAL::millis();
    // put your aux switch #1 handler here (CHx_OPT = 47)
if(ch_flag == 2 && ch_flag !=last_rc_value_video && last_rc_value_video!=7){

    mavlink_msg_command_long_send(MAVLINK_COMM_0,
                                1,
                                100,
                                MAV_CMD_SET_CAMERA_MODE,
                                0,        // confirmation of zero means this is the first time this message has been sent
                                0,
                                1,
                                0,
                                0, 0, 0,  // param4 ~ param6 unused
                                0);

                                mavlink_msg_command_long_send(MAVLINK_COMM_1,
                                1,
                                100,
                                MAV_CMD_SET_CAMERA_MODE,
                                0,        // confirmation of zero means this is the first time this message has been sent
                                0,
                                1,
                                0,
                                0, 0, 0,  // param4 ~ param6 unused
                                0);

            mavlink_msg_command_long_send(MAVLINK_COMM_2,
                                1,
                                100,
                                MAV_CMD_SET_CAMERA_MODE,
                                0,        // confirmation of zero means this is the first time this message has been sent
                                0,
                                1,
                                0,
                                0, 0, 0,  // param4 ~ param6 unused
                                0);
            // hal.scheduler->delay(2000);
if(last_rc_value_video != 3){
   
            last_ms =  nowvideo_ms;

            last_rc_value_video = ch_flag;
    

} else {
    last_rc_value_video = 7;
}
          
             gcs().send_text(MAV_SEVERITY_INFO, "set camera mode : video");

} else if (ch_flag ==2 && last_rc_value_video == ch_flag && (nowvideo_ms -last_ms >2000  && last_rc_value_video !=4)){

mavlink_msg_command_long_send(MAVLINK_COMM_0,
                                  1,
                                  100,
                                  MAV_CMD_VIDEO_START_CAPTURE,
                                  0,        // confirmation of zero means this is the first time this message has been sent
                                  0,
                                  0,
                                  0,
                                  0, 0, 0,  // param4 ~ param6 unused
                                  0);
                             

                                  mavlink_msg_command_long_send(MAVLINK_COMM_1,
                                  1,
                                  100,
                                  MAV_CMD_VIDEO_START_CAPTURE,
                                  0,        // confirmation of zero means this is the first time this message has been sent
                                  0,
                                  0,
                                  0,
                                  0, 0, 0,  // param4 ~ param6 unused
                                  0);
                               

             mavlink_msg_command_long_send(MAVLINK_COMM_2,
                                  1,
                                  100,
                                  MAV_CMD_VIDEO_START_CAPTURE,
                                  0,        // confirmation of zero means this is the first time this message has been sent
                                  0,
                                  0,
                                  0,
                                  0, 0, 0,  // param4 ~ param6 unused
                                  0);
                                  gcs().send_text(MAV_SEVERITY_INFO, "start camera video");

                                  last_rc_value_video = 4;
                                  
} 

if (ch_flag != 2 && (last_rc_value_video ==4 || last_rc_value_video ==7)) {

    mavlink_msg_command_long_send(MAVLINK_COMM_0,
                                  1,
                                  100,
                                  MAV_CMD_VIDEO_STOP_CAPTURE,
                                  0,        // confirmation of zero means this is the first time this message has been sent
                                  0,
                                  0,
                                  0,
                                  0, 0, 0,  // param4 ~ param6 unused
                                  0);

                                  mavlink_msg_command_long_send(MAVLINK_COMM_1,
                                  1,
                                  100,
                                  MAV_CMD_VIDEO_STOP_CAPTURE,
                                  0,        // confirmation of zero means this is the first time this message has been sent
                                  0,
                                  0,
                                  0,
                                  0, 0, 0,  // param4 ~ param6 unused
                                  0);


     mavlink_msg_command_long_send(MAVLINK_COMM_2,
                                  1,
                                  100,
                                  MAV_CMD_VIDEO_STOP_CAPTURE,
                                  0,        // confirmation of zero means this is the first time this message has been sent
                                  0,
                                  0,
                                  0,
                                  0, 0, 0,  // param4 ~ param6 unused
                                  0);
                    gcs().send_text(MAV_SEVERITY_INFO, "stop mode camera");

                    last_ms =  nowvideo_ms;

                    last_rc_value_video = ch_flag;
                    last_rc_value_video = 5;
} else if ((ch_flag != 2 && last_rc_value_video ==5 && (nowvideo_ms -last_ms) > 2000)  || (ch_flag != 2 && last_rc_value_video ==3)) {


mavlink_msg_command_long_send(MAVLINK_COMM_0,
                                1,
                                100,
                                MAV_CMD_SET_CAMERA_MODE,
                                0,        // confirmation of zero means this is the first time this message has been sent
                                0,
                                0,
                                0,
                                0, 0, 0,  // param4 ~ param6 unused
                                0);

                                mavlink_msg_command_long_send(MAVLINK_COMM_1,
                                1,
                                100,
                                MAV_CMD_SET_CAMERA_MODE,
                                0,        // confirmation of zero means this is the first time this message has been sent
                                0,
                                0,
                                0,
                                0, 0, 0,  // param4 ~ param6 unused
                                0);



                     mavlink_msg_command_long_send(MAVLINK_COMM_2,
                                1,
                                100,
                                MAV_CMD_SET_CAMERA_MODE,
                                0,        // confirmation of zero means this is the first time this message has been sent
                                0,
                                0,
                                0,
                                0, 0, 0,  // param4 ~ param6 unused
                                0);
                                gcs().send_text(MAV_SEVERITY_INFO, "set camera mode : take photo");
                        
        last_rc_value_video = 6;


}

}

void Copter::userhook_auxSwitch2(uint8_t ch_flag)
{
    // put your aux switch #2 handler here (CHx_OPT = 48)
}

void Copter::userhook_auxSwitch3(uint8_t ch_flag)
{
    // put your aux switch #3 handler here (CHx_OPT = 49)
}
#endif
