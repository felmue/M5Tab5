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
    io_expander_b_init();
    ina226_init();
    rx8130_init(clearAndDisableRTCInterrupt);

    pinMode(M5TAB5_STAMP_RX_PIN, INPUT);
    pinMode(M5TAB5_STAMP_TX_PIN, INPUT);
}

void M5_TAB5::update()
{
    M5.update();
}

/* -------------------------------------------------------------------------- */
/*                                   INA226                                   */
/* -------------------------------------------------------------------------- */
void M5_TAB5::ina226_init()
{
    if (!INA226.begin()) {
        ESP_LOGE(TAG, "ina226 init failed");
    } else {
        // 28.4 Hz
        INA226.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US,
                         INA226_MODE_SHUNT_BUS_CONT);
        INA226.calibrate(0.005, 8.192);
    }
}

float M5_TAB5::getBatteryVoltage()
{
    return INA226.readBusVoltage();
}

float M5_TAB5::getBatteryCurrent()
{
    return INA226.readShuntCurrent();
}

/* -------------------------------------------------------------------------- */
/*                                   RX8130                                   */
/* -------------------------------------------------------------------------- */
void M5_TAB5::rx8130_init(bool clearAndDisableRTCInterrupt)
{
    if (!RX8130.begin()) {
        ESP_LOGE(TAG, "rx8130 init failed!");
    } else {
        ESP_LOGE(TAG, "rx8130 init OK!");
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

/* -------------------------------------------------------------------------- */
/*                                  IO EXT A                                  */
/* -------------------------------------------------------------------------- */
void M5_TAB5::setExt5VEnable(bool enable)
{
    _io_expander_a->digitalWrite(2, enable);
}

void M5_TAB5::setStamp5VEnable(bool enable)
{
    _io_expander_a->digitalWrite(3, enable);
}

void M5_TAB5::io_expander_a_init()
{
    _io_expander_a = new PI4IOE5V6408_Class;
    if (!_io_expander_a->begin()) {
        ESP_LOGE(TAG, "io expander a init failed");
    } else {
        _io_expander_a->resetIrq();
        // P0 : RF_PTH_L_INT_H_EXT : LOW -> internal antenna
        _io_expander_a->setDirection(0, true);
        _io_expander_a->setPullMode(0, false);
        _io_expander_a->setHighImpedance(0, false);
        _io_expander_a->digitalWrite(0, false);
        // P1 : SPK_EN : HIGH -> enable
        _io_expander_a->setDirection(1, true);
        _io_expander_a->setPullMode(1, false);
        _io_expander_a->setHighImpedance(1, false);
        _io_expander_a->digitalWrite(1, true);
        // P2 : EXT5V_EN : HIGH -> enable
        _io_expander_a->setDirection(2, true);
        _io_expander_a->setPullMode(2, false);
        _io_expander_a->setHighImpedance(2, false);
        _io_expander_a->digitalWrite(2, true);
        // P3 : STAMP5V_EN : LOW -> disabled
        _io_expander_a->setDirection(3, true);
        _io_expander_a->setPullMode(3, false);
        _io_expander_a->setHighImpedance(3, false);
        _io_expander_a->digitalWrite(3, false);
        // P4 : LCD_RST : HIGH
        _io_expander_a->setDirection(4, true);
        _io_expander_a->setPullMode(4, false);
        _io_expander_a->setHighImpedance(4, false);
        _io_expander_a->digitalWrite(4, true);
        // P5 : TP_RST : HIGH
        _io_expander_a->setDirection(5, true);
        _io_expander_a->setPullMode(5, false);
        _io_expander_a->setHighImpedance(5, false);
        _io_expander_a->digitalWrite(5, true);
        // P6 : CAM_RST : HIGH
        _io_expander_a->setDirection(6, true);
        _io_expander_a->setPullMode(6, false);
        _io_expander_a->setHighImpedance(6, false);
        _io_expander_a->digitalWrite(6, true);
        // P7 : HP_DET
        _io_expander_a->setDirection(7, false);
        _io_expander_a->setPullMode(7, true);
        _io_expander_a->setHighImpedance(7, false);
        delay(100);
    }
}

/* -------------------------------------------------------------------------- */
/*                                  IO EXT B                                  */
/* -------------------------------------------------------------------------- */
void M5_TAB5::setWLANPowerEnable(bool enable)
{
    _io_expander_b->digitalWrite(0, enable);
}

void M5_TAB5::setUSB5VEnable(bool enable)
{
    _io_expander_b->digitalWrite(3, enable);
}

void M5_TAB5::powerOff()
{
    M5.Display.sleep();
    M5.Display.waitDisplay();
    for(int i = 0; i < 3; i++)
    {
        _io_expander_b->digitalWrite(4, true);
        delay(100);
        _io_expander_b->digitalWrite(4, false);
        delay(100);
    }
}

void M5_TAB5::setQuickChargeEnable(bool enable)
{
    // low: quick charge on
    // high: quick charge off
    _io_expander_b->digitalWrite(5, !enable);
}

bool M5_TAB5::getChargeState()
{
    if(_io_expander_b->digitalRead(6) == 1) return true;
    return false;
}

void M5_TAB5::setChargerEnable(bool enable)
{
    _io_expander_b->digitalWrite(7, enable);
}

void M5_TAB5::io_expander_b_init()
{
    _io_expander_b = new PI4IOE5V6408_Class(0x44);
    if (!_io_expander_b->begin()) {
        ESP_LOGE(TAG, "io expander b init failed");
    } else {
        _io_expander_b->resetIrq();
        // P0 : WLAN_PWR_EN : HIGH -> enable
        _io_expander_b->setDirection(0, true);
        _io_expander_b->setPullMode(0, false);
        _io_expander_b->setHighImpedance(0, false);
        _io_expander_b->digitalWrite(0, true);
        // P1 : unused
        _io_expander_b->setDirection(1, false);
        _io_expander_b->setPullMode(1, true);
        _io_expander_b->setHighImpedance(1, true);
        // P2 : unused
        _io_expander_b->setDirection(2, false);
        _io_expander_b->setPullMode(2, true);
        _io_expander_b->setHighImpedance(2, true);
        // P3 : USB5V_EN : HIGH -> enable
        _io_expander_b->setDirection(3, true);
        _io_expander_b->setPullMode(3, false);
        _io_expander_b->setHighImpedance(3, false);
        _io_expander_b->digitalWrite(3, true);
        // P4 : PWR_OFF_PULSE : LOW
        _io_expander_b->setDirection(4, true);
        _io_expander_b->setPullMode(4, false);
        _io_expander_b->setHighImpedance(4, false);
        _io_expander_b->digitalWrite(4, false);
        // P5 : nCHG_QC_EN : LOW -> quick charge
        _io_expander_b->setDirection(5, true);
        _io_expander_b->setPullMode(5, false);
        _io_expander_b->setHighImpedance(5, false);
        _io_expander_b->digitalWrite(5, false);
        // P6 : CHG_STAT
        _io_expander_b->setDirection(6, false);
        _io_expander_b->setPullMode(6, true);
        _io_expander_b->setHighImpedance(6, false);
        // P7 : CHG_EN : LOW -> disable
        _io_expander_b->setDirection(7, true);
        _io_expander_b->setPullMode(7, false);
        _io_expander_b->setHighImpedance(7, false);
        _io_expander_b->digitalWrite(7, false);
        delay(100);
    }
}

