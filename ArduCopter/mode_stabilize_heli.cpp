#include "Copter.h"

#if FRAME_CONFIG == HELI_FRAME
/*
 * Init and run calls for stabilize flight mode for trad heli
 */

// stabilize_init - initialise stabilize controller
bool Copter::ModeStabilize_Heli::init(bool ignore_checks)
{
    // set target altitude to zero for reporting
    // To-Do: make pos controller aware when it's active/inactive so it can always report the altitude error?
    pos_control->set_alt_target(0);

    // set stab collective true to use stabilize scaled collective pitch range
    copter.input_manager.set_use_stab_col(true);

    return true;
}

// stabilize_run - runs the main stabilize controller
// should be called at 100hz or more
void Copter::ModeStabilize_Heli::run()
{
    float target_roll, target_pitch;
    float target_yaw_rate;
    float pilot_throttle_scaled;

    
    // apply SIMPLE mode transform to pilot inputs
    update_simple_mode();

    // convert pilot input to lean angles
    get_pilot_desired_lean_angles(target_roll, target_pitch, copter.aparm.angle_max, copter.aparm.angle_max);

    // get pilot's desired yaw rate
    target_yaw_rate = get_pilot_desired_yaw_rate(channel_yaw->get_control_in());

    // get pilot's desired throttle
    pilot_throttle_scaled = copter.input_manager.get_pilot_desired_collective(channel_throttle->get_control_in());

    // Tradheli should not reset roll, pitch, yaw targets when motors are not runup while flying, because
    // we may be in autorotation flight.  This is so that the servos move in a realistic fashion while disarmed
    // for operational checks. Also, unlike multicopters we do not set throttle (i.e. collective pitch) to zero
    // so the swash servos move.

    if (!motors->armed()) {  // Motors Stopped
        motors->set_desired_spool_state(AP_Motors::DESIRED_SHUT_DOWN);
        attitude_control->set_yaw_target_to_current_heading();
        attitude_control->reset_rate_controller_I_terms();
    } else if (!motors->get_interlock() && ap.land_complete) { // landed and motors interlock disabled
        motors->set_desired_spool_state(AP_Motors::DESIRED_GROUND_IDLE);
        attitude_control->set_yaw_target_to_current_heading();
        attitude_control->reset_rate_controller_I_terms();
    } else if (!motors->get_interlock()) { // motors interlock disabled inflight
        motors->set_desired_spool_state(AP_Motors::DESIRED_GROUND_IDLE);
    } else {
        motors->set_desired_spool_state(AP_Motors::DESIRED_THROTTLE_UNLIMITED);
    // clear landing flag above zero throttle
        if (motors->get_spool_mode() == AP_Motors::THROTTLE_UNLIMITED && !motors->limit.throttle_lower) {
            set_land_complete(false);
        }
    }

    // call attitude controller
    attitude_control->input_euler_angle_roll_pitch_euler_rate_yaw(target_roll, target_pitch, target_yaw_rate);

    // output pilot's throttle - note that TradHeli does not used angle-boost
    attitude_control->set_throttle_out(pilot_throttle_scaled, false, g.throttle_filt);
}

#endif  //HELI_FRAME
