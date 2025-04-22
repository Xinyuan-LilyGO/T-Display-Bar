#include "bq27220.h"

BQ27220::BQ27220(uint8_t sda_pin, uint8_t scl_pin, uint32_t freq) 
    : _addr(BQ27220_ADDR), _sda(sda_pin), _scl(scl_pin), _freq(freq) {
    Wire.begin(_sda, _scl, _freq);
}

uint8_t BQ27220::readByte(uint8_t reg) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(_addr, 1U);
    return Wire.read();
}

uint16_t BQ27220::readWord(uint8_t reg) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(_addr, 2U);
    uint16_t val = Wire.read();
    val |= Wire.read() << 8;
    return val;
}

void BQ27220::writeByte(uint8_t reg, uint8_t data) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

void BQ27220::writeWord(uint8_t reg, uint16_t data) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.write(data & 0xFF); //low
    Wire.write(data >> 8);  //high
    Wire.endTransmission();
}

int BQ27220::read_registers(BQ27220_TypeDef& dataSTR) {
    // 实现关键寄存器读取（示例仅读取部分寄存器）
    dataSTR.cntlReg = readWord(BQ_CNTL);
    dataSTR.tempReg = readWord(BQ_TEMP);
    dataSTR.voltReg = readWord(BQ_VOLT);
    dataSTR.flagsReg = readWord(BQ_FLAGS);
    dataSTR.currentReg = readWord(BQ_CURRENT);
    dataSTR.rmReg = readWord(BQ_RM);
    dataSTR.fccReg = readWord(BQ_FCC);
    dataSTR.socReg = readWord(BQ_SOC);
    return 0;
}

uint32_t BQ27220::get_dev_id(BQ27220_TypeDef& dataSTR) {
    writeWord(BQ_CNTL, BQ_DEVICE_NUMBER);
    delay(5);
    return (uint32_t)readWord(BQ_SUB) << 16 | readWord(BQ_SUB+2);
}

void BQ27220::unseal() {
    writeWord(BQ_CNTL, 0x0414);
    delay(5);
    writeWord(BQ_CNTL, 0x3672);
    delay(5);
}

void BQ27220::seal() {
    writeWord(BQ_CNTL, 0x0030);
    delay(5);
}

void BQ27220::read_temp(BQ27220_TypeDef& dataSTR) {
    unseal();
    dataSTR.tempReg = readWord(BQ_TEMP);
    return;
}

void BQ27220::read_vlotage(BQ27220_TypeDef& dataSTR) {
    unseal();
    dataSTR.voltReg = readWord(BQ_VOLT);
    return;
}

void BQ27220::battery_status(BQ27220_TypeDef& dataSTR) {
    unseal();
    dataSTR.flagsReg = readWord(BQ_FLAGS);
    return;
}

void BQ27220::remaining_capacity(BQ27220_TypeDef& dataSTR) {
    unseal();
    dataSTR.rmReg = readWord(BQ_RM);
    return;
}

void BQ27220::state_of_charge(BQ27220_TypeDef& dataSTR) {
    unseal();
    dataSTR.socReg = readWord(BQ_SOC);
    return;
}

void BQ27220::current(BQ27220_TypeDef& dataSTR) {
    unseal();
    dataSTR.currentReg = readWord(BQ_SOC);
    return;
}

