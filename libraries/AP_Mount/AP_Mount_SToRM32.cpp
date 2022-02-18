#include "AP_Mount_SToRM32.h"
#if HAL_MOUNT_ENABLED
#include <AP_HAL/AP_HAL.h>
#include <GCS_MAVLink/GCS.h>

extern const AP_HAL::HAL& hal;

AP_Mount_SToRM32::AP_Mount_SToRM32(AP_Mount &frontend, AP_Mount::mount_state &state, uint8_t instance) :
    AP_Mount_Backend(frontend, state, instance),
    _chan(MAVLINK_COMM_0)
{}

// update mount position - should be called periodically
void AP_Mount_SToRM32::update()
{
    // exit immediately if not initialised
    if (!_initialised) {
        find_gimbal();
        return;
    }
    // flag to trigger sending target angles to gimbal
    bool resend_now = false;

    // update based on mount mode
    switch(get_mode()) {
        // move mount to a "retracted" position.  To-Do: remove support and replace with a relaxed mode?
        case MAV_MOUNT_MODE_RETRACT:
            {
            const Vector3f &target = _state._retract_angles.get();
            _angle_ef_target_rad.x = ToRad(target.x);
            _angle_ef_target_rad.y = ToRad(target.y);
            _angle_ef_target_rad.z = ToRad(target.z);
            }
            break;

        // move mount to a neutral position, typically pointing forward
        case MAV_MOUNT_MODE_NEUTRAL:
            {
            const Vector3f &target = _state._neutral_angles.get();
            _angle_ef_target_rad.x = ToRad(target.x);
            _angle_ef_target_rad.y = ToRad(target.y);
            _angle_ef_target_rad.z = ToRad(target.z);
            }
            break;

        // point to the angles given by a mavlink message
        case MAV_MOUNT_MODE_MAVLINK_TARGETING:
            // do nothing because earth-frame angle targets (i.e. _angle_ef_target_rad) should have already been set by a MOUNT_CONTROL message from GCS
            resend_now = true;
            // cmd_control = true;
            break;

        // RC radio manual angle control, but with stabilization from the AHRS
        case MAV_MOUNT_MODE_RC_TARGETING:
            // update targets using pilot's rc inputs
            update_targets_from_rc();
            resend_now = true;
            // cmd_control = true;
            break;

        // point mount to a GPS point given by the mission planner
        case MAV_MOUNT_MODE_GPS_POINT:
            if (calc_angle_to_roi_target(_angle_ef_target_rad, true, true)) {
                resend_now = true;
            }
            break;

        case MAV_MOUNT_MODE_HOME_LOCATION:
            // constantly update the home location:
            if (!AP::ahrs().home_is_set()) {
                break;
            }
            _state._roi_target = AP::ahrs().get_home();
            _state._roi_target_set = true;
            if (calc_angle_to_roi_target(_angle_ef_target_rad, true, true)) {
                resend_now = true;
            }
            break;

        case MAV_MOUNT_MODE_SYSID_TARGET:
            if (calc_angle_to_sysid_target(_angle_ef_target_rad, true, true)) {
                resend_now = true;
            }
            break;

        default:
            // we do not know this mode so do nothing
            break;
    }
    // if(cmd_control == true){
    //      if ( (AP_HAL::millis() - _last_send) > AP_MOUNT_STORM32_CMD_RESEND_MS) {
    //         if(once_get == false){
    //             gimbal_yaw = _angle_ef_target_rad.z;
    //             once_get = true;
    //         }
    //         send_gimbal_yaw = send_gimbal_yaw*0.9 + gimbal_yaw*0.1;
    //         if(abs(abs(send_gimbal_yaw) - abs(gimbal_yaw)) < 0.087f){
    //             once_get = false;
    //             cmd_control = false;
    //         }
    //         send_do_mount_control(ToDeg(_angle_ef_target_rad.y), ToDeg(_angle_ef_target_rad.x), ToDeg(send_gimbal_yaw), MAV_MOUNT_MODE_MAVLINK_TARGETING);
    //     }
    // }else{
        // resend target angles at least once per second
        if (resend_now || ((AP_HAL::millis() - _last_send) > AP_MOUNT_STORM32_RESEND_MS)) {
            send_do_mount_control(ToDeg(_angle_ef_target_rad.y), ToDeg(_angle_ef_target_rad.x), ToDeg(_angle_ef_target_rad.z), MAV_MOUNT_MODE_MAVLINK_TARGETING);
        }
    // }
}

// has_pan_control - returns true if this mount can control it's pan (required for multicopters)
bool AP_Mount_SToRM32::has_pan_control() const
{
    // we do not have yaw control
    return false;
}

// set_mode - sets mount's mode
void AP_Mount_SToRM32::set_mode(enum MAV_MOUNT_MODE mode)
{
    // exit immediately if not initialised
    if (!_initialised) {
        return;
    }

    // record the mode change
    _state._mode = mode;
}

// send_mount_status - called to allow mounts to send their status to GCS using the MOUNT_STATUS message
void AP_Mount_SToRM32::send_mount_status(mavlink_channel_t chan)
{
    // return target angles as gimbal's actual attitude.  To-Do: retrieve actual gimbal attitude and send these instead
    mavlink_msg_mount_status_send(chan, 0, 0, ToDeg(_angle_ef_target_rad.y)*100, ToDeg(_angle_ef_target_rad.x)*100, ToDeg(_angle_ef_target_rad.z)*100);
}

// search for gimbal in GCS_MAVLink routing table
void AP_Mount_SToRM32::find_gimbal()
{
    // return immediately if initialised
    if (_initialised) {
        return;
    }

    // return if search time has has passed
    if (AP_HAL::millis() > AP_MOUNT_STORM32_SEARCH_MS) {
        return;
    }

    if (GCS_MAVLINK::find_by_mavtype(MAV_TYPE_GENERIC, _sysid, _compid, _chan)) {
    gcs().send_text(MAV_SEVERITY_INFO, "sysid (%d) (%d) (%d)", _sysid,_compid,(int)_chan);
        _initialised = true;
    }
}

// send_do_mount_control - send a COMMAND_LONG containing a do_mount_control message
void AP_Mount_SToRM32::send_do_mount_control(float pitch_deg, float roll_deg, float yaw_deg, enum MAV_MOUNT_MODE mount_mode)
{
    // exit immediately if not initialised
    if (!_initialised) {
        return;
    }

    // check we have space for the message
    if (!HAVE_PAYLOAD_SPACE(_chan, COMMAND_LONG)) {
        return;
    }

    mavlink_msg_command_long_send(_chan,
                                  _sysid,
                                  _compid,
                                  MAV_CMD_DO_MOUNT_CONTROL,
                                  0,        // confirmation of zero means this is the first time this message has been sent
                                  pitch_deg,
                                  roll_deg,
                                  yaw_deg,
                                  0, 0, 0,  // param4 ~ param6 unused
                                  mount_mode);

        // gcs().send_text(MAV_SEVERITY_INFO, "titl gimbal %d | %d | %d | (%d) (%d) (%d)", _chan,_sysid,_compid,(int)pitch_deg,(int)roll_deg,(int)yaw_deg);
    //     uint32_t ttt = AP_HAL::millis();
    // if(ttt - _time_send > 1000){
    //     gcs().send_text(MAV_SEVERITY_INFO, "titl gimbal %d | %d | %d | (%d) (%d) (%d)", _chan,_sysid,_compid,(int)pitch_deg,(int)roll_deg,(int)yaw_deg);
    //     _time_send = ttt;
    // }
    // store time of send
    _last_send = AP_HAL::millis();

}
#endif // HAL_MOUNT_ENABLED
