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
    // put your aux switch #1 handler here (CHx_OPT = 47)
if(ch_flag == 2){

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
             gcs().send_text(MAV_SEVERITY_INFO, "set camera mode : video");

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
                                  
} else {

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
