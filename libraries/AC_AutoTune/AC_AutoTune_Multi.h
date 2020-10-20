/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
  support for autotune of multirotors. Based on original autotune code from ArduCopter, written by Leonard Hall
  Converted to a library by Andrew Tridgell
 */

#pragma once

#include "AC_AutoTune.h"

class AC_AutoTune_Multi : public AC_AutoTune
{
public:
    // constructor
    AC_AutoTune_Multi()
    {
        tune_seq[0] = RD_UP;
        tune_seq[1] = RD_DOWN;
        tune_seq[2] = RP_UP;
        tune_seq[3] = SP_DOWN;
        tune_seq[4] = SP_UP;
        tune_seq[5] = TUNE_COMPLETE;
    };
    // save gained, called on disarm
    void save_tuning_gains() override;

protected:
    void test_init() override;
    void test_run(AxisType test_axis, const float dir_sign) override;
    void do_gcs_announcements() override;
    void load_test_gains() override;
    // generic method used to update gains for the rate p up tune type
    void updating_rate_p_up_all(AxisType test_axis) override;
    // generic method used to update gains for the rate p down tune type
    void updating_rate_p_down_all(AxisType test_axis) override {};
    // generic method used to update gains for the rate d up tune type
    void updating_rate_d_up_all(AxisType test_axis) override;
    // generic method used to update gains for the rate d down tune type
    void updating_rate_d_down_all(AxisType test_axis) override;
    // generic method used to update gains for the angle p up tune type
    void updating_angle_p_up_all(AxisType test_axis) override;
    // generic method used to update gains for the angle p down tune type
    void updating_angle_p_down_all(AxisType test_axis) override;

    void Log_AutoTune() override;
    void Log_AutoTuneDetails() override;
    void Log_Write_AutoTune(uint8_t axis, uint8_t tune_step, float meas_target, float meas_min, float meas_max, float new_gain_rp, float new_gain_rd, float new_gain_sp, float new_ddt);
    void Log_Write_AutoTuneDetails(float angle_cd, float rate_cds);
    bool allow_zero_rate_p() override {return false;}
    float get_intra_test_ri(AxisType test_axis) override;
    float get_load_tuned_ri(AxisType test_axis) override;
    float get_load_tuned_yaw_rd() override {return 0.0f;}
    float get_rp_min() const override;
    float get_sp_min() const override;
    float get_rlpf_min() const override;

private:
    // updating_rate_d_up - increase D and adjust P to optimize the D term for a little bounce back
    // optimize D term while keeping the maximum just below the target by adjusting P
    void updating_rate_d_up(float &tune_d, float tune_d_min, float tune_d_max, float tune_d_step_ratio, float &tune_p, float tune_p_min, float tune_p_max, float tune_p_step_ratio, float rate_target, float meas_rate_min, float meas_rate_max);
    // updating_rate_d_down - decrease D and adjust P to optimize the D term for no bounce back
    // optimize D term while keeping the maximum just below the target by adjusting P
    void updating_rate_d_down(float &tune_d, float tune_d_min, float tune_d_step_ratio, float &tune_p, float tune_p_min, float tune_p_max, float tune_p_step_ratio, float rate_target, float meas_rate_min, float meas_rate_max);
    // updating_rate_p_up_d_down - increase P to ensure the target is reached while checking bounce back isn't increasing
    // P is increased until we achieve our target within a reasonable time while reducing D if bounce back increases above the threshold
    void updating_rate_p_up_d_down(float &tune_d, float tune_d_min, float tune_d_step_ratio, float &tune_p, float tune_p_min, float tune_p_max, float tune_p_step_ratio, float rate_target, float meas_rate_min, float meas_rate_max);
    // updating_angle_p_down - decrease P until we don't reach the target before time out
    // P is decreased to ensure we are not overshooting the target
    void updating_angle_p_down(float &tune_p, float tune_p_min, float tune_p_step_ratio, float angle_target, float meas_angle_max, float meas_rate_min, float meas_rate_max);
    // updating_angle_p_up - increase P to ensure the target is reached
    // P is increased until we achieve our target within a reasonable time
    void updating_angle_p_up(float &tune_p, float tune_p_max, float tune_p_step_ratio, float angle_target, float meas_angle_max, float meas_rate_min, float meas_rate_max);

};
