/*
 * SPDX-FileCopyrightText: 2025 GWENDESIGN
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "utils/rx8130/rx8130.h"
#include <M5GFX.h>
#include <M5Unified.hpp>

#define M5TAB5_STAMP_RX_PIN 46
#define M5TAB5_STAMP_TX_PIN 6

namespace m5 {

class M5_TAB5 {
public:
    void begin(bool clearAndDisableRTCInterrupt = true);
    void update();

    // Deprecated: float getBatteryVoltage();
    // Use: int32_t M5.Power.getBatteryLevel();
    // Deprecated: float getBatteryCurrent();
    // Use: int32_t M5.Power.getBatteryCurrent();

    // === RX8130 ===
    RX8130_Class RX8130;

    void setRtcTime(struct tm* time);
    void getRtcTime(struct tm* time);
    void setRtcAlarmTime(struct tm* time);
    void getRtcAlarmTime(struct tm* time);
    uint8_t readRtcIrqFlags();
    void clearRtcIrqFlags();
    void enableRtcIrq(uint8_t bitmap);
    void disableRtcIrq();
    uint8_t readRtcReg(uint8_t reg);

    // === IO EXT A (0) ===
    void setRFPath(bool l_int_h_ext);
    void setSpeakerEnable(bool enable);
    // Deprecated: void setExt5VEnable(bool enable);
    // Use: void M5.Power.setExtOutput(bool enable, m5::ext_port_mask_t::ext_PA);
    void setStamp5VEnable(bool enable); // unused on non modified M5Tab5
    // resetLCD
    // resetTouch
    // resetCamera
    // headphoneDetect

    // === IO EXT B (1) ===
    void setWLANPowerEnable(bool enable);
    // unused
    // unused
    // Deprecated: void setUSB5VEnable(bool enable);
    // Use: void M5.Power.setExtOutput(bool enable, m5::ext_port_mask_t::ext_USB);
    // Deprecated: void powerOff();
    // Use: void M5.Power.powerOff();
    void setQuickChargeEnable(bool enable);
    // Deprecated: bool getChargeState();
    // Use: Power_Class::is_charging_t M5.Power.isCharging();
    // Deprecated: void setChargerEnable(bool enable);
    // Use: void M5.Power.setBatteryCharge(bool enable);

private:
    void io_expander_a_init();
    void rx8130_init(bool clearAndDisableRTCInterrupt = true);
};

}  // namespace m5

extern m5::M5_TAB5 M5Tab5;

