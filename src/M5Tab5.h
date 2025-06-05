/*
 * SPDX-FileCopyrightText: 2025 GWENDESIGN
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "utils/pi4ioe5v6408/pi4ioe5v6408.h"
#include "utils/ina226/ina226.h"
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

    INA226_Class INA226 = INA226_Class(0x41);
    RX8130_Class RX8130;

    float getBatteryVoltage();
    float getBatteryCurrent();

    void setRtcTime(struct tm* time);
    void getRtcTime(struct tm* time);
    void setRtcAlarmTime(struct tm* time);
    void getRtcAlarmTime(struct tm* time);
    uint8_t readRtcIrqFlags();
    void clearRtcIrqFlags();
    void enableRtcIrq(uint8_t bitmap);
    void disableRtcIrq();

    // setRFPath
    // setSpeakerEnable
    void setExt5VEnable(bool enable);
    void setStamp5VEnable(bool enable);
    // resetLCD
    // resetTouch
    // resetCamera
    // headphoneDetect

    void setWLANPowerEnable(bool enable);
    void setUSB5VEnable(bool enable);
    void powerOff();
    void setQuickChargeEnable(bool enable);
    bool getChargeState();
    void setChargerEnable(bool enable);

private:
    PI4IOE5V6408_Class* _io_expander_a = nullptr;
    PI4IOE5V6408_Class* _io_expander_b = nullptr;

    void io_expander_a_init();
    void io_expander_b_init();

    void ina226_init();
    void rx8130_init(bool clearAndDisableRTCInterrupt = true);
};

}  // namespace m5

extern m5::M5_TAB5 M5Tab5;

