/*
  SToRM32 mount backend class
 */
#pragma once

#include <AP_HAL/AP_HAL.h>
#include <AP_AHRS/AP_AHRS.h>

#include <AP_Math/AP_Math.h>
#include <AP_Common/AP_Common.h>
#include <RC_Channel/RC_Channel.h>
#include "AP_Mount_Backend.h"
#if HAL_MOUNT_ENABLED

#define AP_MOUNT_STORM32_RESEND_MS  1000    // resend angle targets to gimbal once per second
#define AP_MOUNT_STORM32_SEARCH_MS  600000   // search for gimbal for 1 minute after startup
#define PUS_STEP 0.5f
class AP_Mount_SToRM32 : public AP_Mount_Backend
{

public:
    // Constructor
    AP_Mount_SToRM32(AP_Mount &frontend, AP_Mount::mount_state &state, uint8_t instance);

    // init - performs any required initialisation for this instance
    void init() override {}

    // update mount position - should be called periodically
    void update() override;

    // has_pan_control - returns true if this mount can control it's pan (required for multicopters)
    bool has_pan_control() const override;

    // set_mode - sets mount's mode
    void set_mode(enum MAV_MOUNT_MODE mode) override;

    // send_mount_status - called to allow mounts to send their status to GCS using the MOUNT_STATUS message
    void send_mount_status(mavlink_channel_t chan) override;

private:

    // search for gimbal in GCS_MAVLink routing table
    void find_gimbal();

    // send_do_mount_control - send a COMMAND_LONG containing a do_mount_control message
    void send_do_mount_control(float pitch_deg, float roll_deg, float yaw_deg, enum MAV_MOUNT_MODE mount_mode);
    float caculate_pitch_angle(float pitch_deg);
    float caculate_yaw_angle(float yaw_deg);
    float last_pitch_channel = 0.0f;
    float last_yaw_channel = 0.0f;
    float pitch_dig = 0.0f;
    float yaw_dig = 0.0f;
    float pitch_pecent = 0.0f;
    float yaw_pecent = 0.0f;
    // internal variables
    bool _initialised;              // true once the driver has been initialised
    uint8_t _sysid;                 // sysid of gimbal
    uint8_t _compid;                // component id of gimbal
    mavlink_channel_t _chan;        // mavlink channel used to communicate with gimbal.  Currently hard-coded to Telem2
    uint32_t _last_send;            // system time of last do_mount_control sent to gimbal
};
#endif // HAL_MOUNT_ENABLED
