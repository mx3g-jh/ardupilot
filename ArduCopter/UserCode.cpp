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
    uint64_t time_unix = 0;
    float posD;
    Vector3f vel;
    AP_AHRS &ahrs2 = AP::ahrs();
    switch (send_mavlink_camera_state)
    {
    case 0:

        // put your 100Hz code here
        AP::rtc().get_utc_usec(time_unix); // may fail, leaving time_unix at 0
        // add 8h in china
        time_unix = time_unix +28800000000; // +8 UTC
        if(time_unix > 1644480872000000){
        mavlink_msg_system_time_send(
            MAVLINK_COMM_2,
            time_unix,
            AP_HAL::millis());
        }
            // send_mavlink_camera_state++;

        break;
    case 1:

        UNUSED_RESULT(ahrs2.get_position(global_position_current_loc)); 

        if (!ahrs2.get_velocity_NED(vel)) {
            vel.zero();
        }

        AP::ahrs().get_relative_position_D_home(posD);
        mavlink_msg_global_position_int_send(
            MAVLINK_COMM_2,
            AP_HAL::millis(),
            global_position_current_loc.lat, // in 1E7 degrees
            global_position_current_loc.lng, // in 1E7 degrees
            global_position_current_loc.alt * 10UL,      // millimeters above ground/sea level
            posD * (-1000.0f),  // millimeters above home
            vel.x * 100,                     // X speed cm/s (+ve North)
            vel.y * 100,                     // Y speed cm/s (+ve East)
            vel.z * 100,                     // Z speed cm/s (+ve Down)
            ahrs2.yaw_sensor);                // compass heading in 1/100 degree
            // send_mavlink_camera_state++;
        break;
    default:
        break;
    }
send_mavlink_camera_state++;
if (send_mavlink_camera_state < 2) {
    return;
} else {
    send_mavlink_camera_state = 0;
}

if(first_detect == true || ap_mission->get_cmd_mode() == 1 || ap_mission->get_cmd_mode() == 2){
    if(mode_change_to_redio == true || ap_mission->get_cmd_mode() == 1){
        if(camera_status_flag == 0 ){
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
            camera_status_flag = 1;
            delay_start =  AP_HAL::millis();
        }

        if(camera_status_flag ==1){
            int32_t delay_loop = AP_HAL::millis();
            if(delay_loop - delay_start > 2000){
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
                camera_status_flag = 2;
            }
        }
    }
}

if(mode_change_to_redio == false || ap_mission->get_cmd_mode() == 2){
    if(camera_status_flag == 2){
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
        gcs().send_text(MAV_SEVERITY_INFO, "stop camera video");
        camera_status_flag = 3;
        delay_stop =  AP_HAL::millis();
    }

    if(camera_status_flag == 3){
    int32_t delay_loop_2 = AP_HAL::millis();
    if(delay_loop_2 - delay_stop > 2000){
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
        camera_status_flag = 0;
        ap_mission->set_cmd_mode(0);
    }
    }

}
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
    if(first_come_in == false){
        first_mode = ch_flag;
        first_come_in = true;
    }
    if(first_come_in == true && first_detect == false){
        if(first_mode != ch_flag)first_detect = true;
    }

    if(ch_flag == 2){
        mode_change_to_redio = true;
        gcs().send_text(MAV_SEVERITY_INFO, "mode_change_to_vedio = true");
    }
    if(ch_flag == 0){
        mode_change_to_redio = false;
        gcs().send_text(MAV_SEVERITY_INFO, "mode_change_to_vedio = false");
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
