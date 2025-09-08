/*
 * SPDX-FileCopyrightText: 2025 GWENDESIGN
 *
 * SPDX-License-Identifier: MIT
 */
#include "M5Tab5.h"
#include <cstring>

using namespace m5;

M5_TAB5 M5Tab5;

static const char* TAG = "M5Tab5";

void M5_TAB5::begin(bool clearAndDisableRTCInterrupt)
{
//    M5.begin();
//    i2c_init();
    io_expander_a_init();
    rx8130_init(clearAndDisableRTCInterrupt);

    pinMode(M5TAB5_STAMP_RX_PIN, INPUT);
    pinMode(M5TAB5_STAMP_TX_PIN, INPUT);
}

void M5_TAB5::update()
{
    M5.update();
}

/* -------------------------------------------------------------------------- */
/*                                   RX8130                                   */
/* -------------------------------------------------------------------------- */
void M5_TAB5::rx8130_init(bool clearAndDisableRTCInterrupt)
{
    if (!RX8130.begin()) {
        ESP_LOGE(TAG, "rx8130 init failed!");
    } else {
        RX8130.initBat();
        if(clearAndDisableRTCInterrupt == true) {
            RX8130.disableIrq();
            RX8130.clearIrqFlags();
        }
    }
}

void M5_TAB5::setRtcTime(struct tm* time)
{
    RX8130.setTime(time);
}

void M5_TAB5::getRtcTime(struct tm* time)
{
    RX8130.getTime(time);
}

void M5_TAB5::setRtcAlarmTime(struct tm* time)
{
    RX8130.setAlarmTime(time);
}

void M5_TAB5::getRtcAlarmTime(struct tm* time)
{
    RX8130.getAlarmTime(time);
}

uint8_t M5_TAB5::readRtcIrqFlags()
{
    return RX8130.readIrqFlags();
}

void M5_TAB5::clearRtcIrqFlags()
{
    RX8130.clearIrqFlags();
}

void M5_TAB5::enableRtcIrq(uint8_t bitmap)
{
    RX8130.enableIrq(bitmap);
}

void M5_TAB5::disableRtcIrq()
{
    RX8130.disableIrq();
}

uint8_t M5_TAB5::readRtcReg(uint8_t reg)
{
    return RX8130.readReg(reg);
}

/* -------------------------------------------------------------------------- */
/*                                  IO EXT A (0)                              */
/* -------------------------------------------------------------------------- */
void M5_TAB5::setRFPath(bool l_int_h_ext)
{
    auto& ioe = M5.getIOExpander(0);
    ioe.digitalWrite(0, l_int_h_ext);
}

void M5_TAB5::setSpeakerEnable(bool enable)
{
    auto& ioe = M5.getIOExpander(0);
    ioe.digitalWrite(1, enable);
}

void M5_TAB5::setStamp5VEnable(bool enable)
{
    auto& ioe = M5.getIOExpander(0);
    ioe.digitalWrite(3, enable);
}

void M5_TAB5::io_expander_a_init()
{
    auto& ioe = M5.getIOExpander(0);
    // P3 : STAMP5V_EN : LOW -> disabled
    ioe.setDirection(3, true);
    ioe.setPullMode(3, false);
    ioe.setHighImpedance(3, false);
    ioe.digitalWrite(3, false);
}

/* -------------------------------------------------------------------------- */
/*                                  IO EXT B (1)                              */
/* -------------------------------------------------------------------------- */
void M5_TAB5::setWLANPowerEnable(bool enable)
{
    auto& ioe = M5.getIOExpander(1);
    ioe.digitalWrite(0, enable);
}

void M5_TAB5::setQuickChargeEnable(bool enable)
{
    // low: quick charge on
    // high: quick charge off
    auto& ioe = M5.getIOExpander(1);
    ioe.digitalWrite(5, !enable);
}

