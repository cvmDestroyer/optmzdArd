// clang-format off
/*
'    ___                                   ___                   ___             
'  _|   |_   _______   _______   _______  |   |      _______   _|   |_   _______   _______ 
' |_     _| |       | |       | |       | |   | __  |____   | |_     _| |       | |    ___|
'   |   |   |   ' __| |  |  | | |   '   | |   v   | |   .   |   |   |   |   ' __| |___    |
'   |___|   |_______| |__|__|_| |    ___| \______7  |_____|_|   |___|   |_______| |_______|
'                               |___|    
'                                                    ___
'  _______   _______   ________  _______            |   |___
' |       | |       | |   __  | |____   |           |       |
' |  |  | | |   ' __| |     __| |   .   |    ___    |   |   |
' |__|__|_| |_______| ┌' __'──┐ |_____|_|   |___|   |___|___|
'                     |_______|
  megas template libary for optmzdArd
*/
#include <Arduino.h>
#include <stdint.h>
#include "util.h"

#pragma once
#ifndef templates_mega_h
#define templates_mega_h

// this just for the template part cool ik :)
template<uint8_t PIN> struct hardwearLvl;
#define DEFINE_PIN(pin, port_letter, port_bit, adc_ch, is_pwm, pwm_reg, tccra_reg, tccrb_reg, com_bit, tccra_bits, tccrb_bits) \
template<> struct hardwearLvl<pin> { \
    static constexpr uintptr_t PORT       = (uintptr_t)&PORT##port_letter; \
    static constexpr uintptr_t PIN_REG    = (uintptr_t)&PIN##port_letter; \
    static constexpr uintptr_t DDR        = (uintptr_t)&DDR##port_letter; \
    static constexpr uint8_t   BIT        = port_bit; \
    static constexpr uint8_t   ADC_CH     = adc_ch; \
    static constexpr bool      HAS_PWM    = is_pwm; \
    static constexpr uintptr_t PWM_REG    = (uintptr_t)(pwm_reg); \
    static constexpr uintptr_t TCCRA_REG  = (uintptr_t)(tccra_reg); \
    static constexpr uintptr_t TCCRB_REG  = (uintptr_t)(tccrb_reg); \
    static constexpr uint8_t   COM_BIT    = com_bit; \
    static constexpr uint8_t   TCCRA_BITS = tccra_bits; \
    static constexpr uint8_t   TCCRB_BITS = tccrb_bits; \
};

// pin  : just the arduino pin yk? 
// reg  : this is what of the 3 register the pin is placed (B, C, D)
// bit  : where the bit is placed in the byte
// adc  : basicly the bit thingy just for Analog Digital Converter (255 = no adc/analogPin) 
// pwm  : does the pin support pwm? (true/false)
// OCR  : just saves the pwm val 0-255 (Output Compare Register)
// TCCRA: control-register a of timers (Timer/Counter Control Register) take look in timerUno
// TCCRB: control-register b of timers (Timer/Counter Control Register) take look in timerUno
// COM  : it is the bitmask to wire the pin to the hardware timer (Compare Output Mode)
// tccra bits: here are wgm set to fast pwm 8bit for analogWrite
// tccrb bits: prescaler bits set tp 64 + wgm2 bit set to fast pwm 8 bit (tiemer 2)
// ---------------------------------------------------------------------------------------
//         pin  reg  bit  adc   pwm                 OCR               TCCRA     TCCRB        COM     tccra bits  tccrb bits
DEFINE_PIN( 0,   E,   0,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 1,   E,   1,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 2,   E,   4,  255,  true,  (volatile uint16_t*)&OCR3B,  &TCCR3A,  &TCCR3B,  (1 << COM3B1),  0x01,       0x0B   )
DEFINE_PIN( 3,   E,   5,  255,  true,  (volatile uint16_t*)&OCR3C,  &TCCR3A,  &TCCR3B,  (1 << COM3C1),  0x01,       0x0B   )
DEFINE_PIN( 4,   G,   5,  255,  true,  (volatile uint16_t*)&OCR0B,  &TCCR0A,  &TCCR0B,  (1 << COM0B1),  0x03,       0x03   )
DEFINE_PIN( 5,   E,   3,  255,  true,  (volatile uint16_t*)&OCR3A,  &TCCR3A,  &TCCR3B,  (1 << COM3A1),  0x01,       0x0B   )
DEFINE_PIN( 6,   H,   3,  255,  true,  (volatile uint16_t*)&OCR4A,  &TCCR4A,  &TCCR4B,  (1 << COM4A1),  0x01,       0x0B   )
DEFINE_PIN( 7,   H,   4,  255,  true,  (volatile uint16_t*)&OCR4B,  &TCCR4A,  &TCCR4B,  (1 << COM4B1),  0x01,       0x0B   )
DEFINE_PIN( 8,   H,   5,  255,  true,  (volatile uint16_t*)&OCR4C,  &TCCR4A,  &TCCR4B,  (1 << COM4C1),  0x01,       0x0B   )
DEFINE_PIN( 9,   H,   6,  255,  true,  (volatile uint16_t*)&OCR2B,  &TCCR2A,  &TCCR2B,  (1 << COM2B1),  0x03,       0x04   )
DEFINE_PIN( 10,  B,   4,  255,  true,  (volatile uint16_t*)&OCR2A,  &TCCR2A,  &TCCR2B,  (1 << COM2A1),  0x03,       0x04   )
DEFINE_PIN( 11,  B,   5,  255,  true,  (volatile uint16_t*)&OCR1A,  &TCCR1A,  &TCCR1B,  (1 << COM1A1),  0x01,       0x0B   )
DEFINE_PIN( 12,  B,   6,  255,  true,  (volatile uint16_t*)&OCR1B,  &TCCR1A,  &TCCR1B,  (1 << COM1B1),  0x01,       0x0B   )
DEFINE_PIN( 13,  B,   7,  255,  true,  (volatile uint16_t*)&OCR0A,  &TCCR0A,  &TCCR0B,  (1 << COM0A1),  0x03,       0x03   )

DEFINE_PIN( 14,  J,   1,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 15,  J,   0,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 16,  H,   1,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 17,  H,   0,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 18,  D,   3,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 19,  D,   2,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 20,  D,   1,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 21,  D,   0,  255, false,                0,                 0,        0,          0,         0,          0     )

DEFINE_PIN( 22,  A,   0,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 23,  A,   1,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 24,  A,   2,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 25,  A,   3,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 26,  A,   4,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 27,  A,   5,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 28,  A,   6,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 29,  A,   7,  255, false,                0,                 0,        0,          0,         0,          0     )

DEFINE_PIN( 30,  C,   7,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 31,  C,   6,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 32,  C,   5,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 33,  C,   4,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 34,  C,   3,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 35,  C,   2,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 36,  C,   1,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 37,  C,   0,  255, false,                0,                 0,        0,          0,         0,          0     )

DEFINE_PIN( 38,  D,   7,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 39,  G,   2,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 40,  G,   1,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 41,  G,   0,  255, false,                0,                 0,        0,          0,         0,          0     )

DEFINE_PIN( 42,  L,   7,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 43,  L,   6,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 44,  L,   5,  255,  true,  (volatile uint16_t*)&OCR5C,  &TCCR5A,  &TCCR5B,  (1 << COM5C1),  0x01,       0x0B   )
DEFINE_PIN( 45,  L,   4,  255,  true,  (volatile uint16_t*)&OCR5B,  &TCCR5A,  &TCCR5B,  (1 << COM5B1),  0x01,       0x0B   )
DEFINE_PIN( 46,  L,   3,  255,  true,  (volatile uint16_t*)&OCR5A,  &TCCR5A,  &TCCR5B,  (1 << COM5A1),  0x01,       0x0B   )
DEFINE_PIN( 47,  L,   2,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 48,  L,   1,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 49,  L,   0,  255, false,                0,                 0,        0,          0,         0,          0     )

DEFINE_PIN( 50,  B,   3,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 51,  B,   2,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 52,  B,   1,  255, false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 53,  B,   0,  255, false,                0,                 0,        0,          0,         0,          0     )

DEFINE_PIN( 54,  F,   0,   0,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 55,  F,   1,   1,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 56,  F,   2,   2,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 57,  F,   3,   3,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 58,  F,   4,   4,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 59,  F,   5,   5,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 60,  F,   6,   6,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 61,  F,   7,   7,  false,                0,                 0,        0,          0,         0,          0     )

DEFINE_PIN( 62,  K,   0,   0,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 63,  K,   1,   1,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 64,  K,   2,   2,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 65,  K,   3,   3,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 66,  K,   4,   4,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 67,  K,   5,   5,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 68,  K,   6,   6,  false,                0,                 0,        0,          0,         0,          0     )
DEFINE_PIN( 69,  K,   7,   7,  false,                0,                 0,        0,          0,         0,          0     )

template<uint8_t PIN> void digitalWrite(bool val) 
{
    uint8_t oldSREG{SREG};
    cli();

    util_mega::template_util<PIN>::turnOffPWM();
    
    volatile uint8_t* reg{reinterpret_cast<volatile uint8_t*>(hardwearLvl<PIN>::PORT)};
    constexpr uint8_t mask{(1 << hardwearLvl<PIN>::BIT)};

    if (val) {
        *reg |= mask;
    } else {
        *reg &= ~mask;
    }  

    SREG = oldSREG;
}

template<uint8_t PIN, bool HAS_PWM = hardwearLvl<PIN>::HAS_PWM> // these are for anlogRead cuz
struct analogWriteHelper {                                      // if constexpr only works cince C++ 17 and 
    static void apply(uint8_t val) {                            // avr-compiler are stuck on C++ 11
        digitalWrite<PIN>(val >= 128);
    }
};

template<uint8_t PIN>
struct analogWriteHelper<PIN, true> {
    // i feel like this doesnt need explaination its in normal analogWrite if you want it
    static void apply(uint8_t val) {
        if (val == 0) {
            *reinterpret_cast<volatile uint8_t*>(hardwearLvl<PIN>::TCCRA_REG) &= ~hardwearLvl<PIN>::COM_BIT;
            digitalWrite<PIN>(LOW);
        }
        else if (val == 255) {
            *reinterpret_cast<volatile uint8_t*>(hardwearLvl<PIN>::TCCRA_REG) &= ~hardwearLvl<PIN>::COM_BIT;
            digitalWrite<PIN>(HIGH);
        }
        else {
            *reinterpret_cast<volatile uint8_t*>(hardwearLvl<PIN>::TCCRA_REG) |= hardwearLvl<PIN>::COM_BIT | hardwearLvl<PIN>::TCCRA_BITS;
            *reinterpret_cast<volatile uint8_t*>(hardwearLvl<PIN>::TCCRB_REG) = hardwearLvl<PIN>::TCCRB_BITS;
            *reinterpret_cast<volatile uint16_t*>(hardwearLvl<PIN>::PWM_REG) = val;
        }
    }
};

// templates
template<uint8_t PIN> void pinMode(uint8_t func) 
{
    uint8_t oldSREG{SREG};
    cli();

    volatile uint8_t* outputReg{reinterpret_cast<volatile uint8_t*>(hardwearLvl<PIN>::DDR)};
    volatile uint8_t* pullupReg{reinterpret_cast<volatile uint8_t*>(hardwearLvl<PIN>::PORT)};
    constexpr uint8_t mask{(1 << hardwearLvl<PIN>::BIT)};

    if (func == _OUTPUT) {
        *outputReg |= mask;
        *pullupReg &= ~mask;
    } else if (func == _INPUT_PULLUP) {
        *outputReg &= ~mask;
        *pullupReg |= mask;
    } else if (func == _INPUT) {
        *outputReg &= ~mask;
        *pullupReg &= ~mask;
    }

    SREG = oldSREG;
}
template<uint8_t PIN> bool digitalRead(void) 
{
    volatile uint8_t* reg{reinterpret_cast<volatile uint8_t*>(hardwearLvl<PIN>::PIN_REG)};
    constexpr uint8_t mask{(1 << hardwearLvl<PIN>::BIT)};

    return (*reg & mask);
}
template<uint8_t PIN> uint16_t analogRead(void) 
{
    static_assert(hardwearLvl<PIN>::ADC_CH != 255, "ERROR: analogRead can only read anlog pins PWM DOES NOT COUNT(~pin) onyl A0 - A15");
    uint8_t oldSREG{SREG};
    cli();
    constexpr uint8_t channel{hardwearLvl<PIN>::ADC_CH};
    constexpr uint8_t bit{hardwearLvl<PIN>::BIT};
    
    ADMUX = 0;
    if (PIN >= A8) {
        ADCSRB |= (1 << MUX5);
    } else {
        ADCSRB &= ~(1 << MUX5);
    }
    ADMUX |= mega::privat::analog_reference | channel;
    if (PIN >= A8) {
        DIDR2 |= (1 << channel);
    } else {
        DIDR0 |= (1 << channel);
    }
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    ADCSRA |= (1 << ADSC);
    SREG = oldSREG;
    while (ADCSRA & (1 << ADSC))
        ; // null statement

    if (PIN >= A8) {
        DIDR2 &= ~(1 << channel);
    } else {
        DIDR0 &= ~(1 << channel);
    }
    
    return ADC;
}
template<uint8_t PIN> void analogWrite(uint8_t val) {
    analogWriteHelper<PIN>::apply(val);
}
template<uint8_t PIN> uint32_t pulseIn(bool state, uint32_t timeout) 
{  
    uint8_t oldSREG{SREG};
    cli();
    volatile uint8_t* reg{reinterpret_cast<volatile uint8_t*>(hardwearLvl<PIN>::PIN_REG)};
    constexpr uint8_t mask{(1 << hardwearLvl<PIN>::BIT)};

    uint32_t cycles{};
        
    uint32_t maxCycles{timeout * (F_CPU / 1000000L) / 16};

    while ((*reg & mask) == (state ? mask : 0)) {
        if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;}
    } 
    while ((*reg & mask) != (state ? mask : 0)) {
        if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;}
    }

    uint32_t pulseCycles{};
    while ((*reg & mask) == (state ? mask : 0)) {
        if (pulseCycles++ >= maxCycles) {SREG = oldSREG; return 0;}
    }

    SREG = oldSREG;
    return (pulseCycles * 16) / (F_CPU / 1000000L);
}
template<uint8_t PIN> uint32_t pulseInLong(bool state, uint32_t timeout) 
{
    uint8_t oldSREG{SREG};
    cli();
    volatile uint8_t* reg{reinterpret_cast<volatile uint8_t*>(hardwearLvl<PIN>::PIN_REG)};
    constexpr uint8_t mask{(1 << hardwearLvl<PIN>::BIT)};
    
    const uint32_t startMicros{mega::micros()};

    while ((*reg & mask) == (state ? mask : 0)) {
        if (mega::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
    }
    while ((*reg & mask) != (state ? mask : 0)) {
        if (mega::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
    }

    const uint32_t pulseStart{mega::micros()};

    while ((*reg & mask) == (state ? mask : 0)) {
        if (mega::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
    }

    SREG = oldSREG;
    return mega::micros() - pulseStart;
}
template<uint8_t PIN> uint32_t pulseIn(bool state) {
    return mega::pulseIn<PIN>(state, 1000000L);
}
template<uint8_t PIN> uint32_t pulseInLong(bool state) {
    return mega::pulseInLong<PIN>(state, 1000000UL);
}
template<uint8_t PIN> void tone(uint16_t frequency) 
{
    if (frequency < 31) { TIMSK2 &= ~(1 << OCIE2A); digitalWrite<PIN>(_LOW); return; }
    
    mega::privat::current_pin = PIN;
    mega::pinMode<PIN>(_OUTPUT);

    uint32_t prescaler  {
                            (frequency < 244) ? 1024 : (frequency < 488) ? 256 :
                            (frequency < 976) ? 128  : (frequency < 1953) ? 64 :
                            (frequency < 7812) ? 32  : 8
                        }; 
    uint8_t presBit     {
                            (frequency < 244)  ? ((1 << CS22) | (1 << CS21) | (1 << CS20)) :
                            (frequency < 488)  ? ((1 << CS22) | (1 << CS21)) :
                            (frequency < 976)  ? ((1 << CS22) | (1 << CS20)) :
                            (frequency < 1953) ? (1 << CS22) :
                            (frequency < 7812) ? ((1 << CS21) | (1 << CS20)) : (1 << CS21)
                        };

    TCCR2A = (1 << WGM21);
    TCCR2B = presBit;
    OCR2A = static_cast<uint8_t>((16000000UL / (2UL * prescaler * frequency)) - 1);
    TIMSK2 |= (1 << OCIE2A);
}
template<uint8_t PIN> void tone(uint16_t frequency, uint32_t duration) {
    mega::tone<PIN>(frequency);

    if (duration > 0) {
        mega::privat::toggle_count = (2UL * frequency * duration) / 1000UL;
    } else { 
        mega::privat::toggle_count = 0; 
    }
}
template<uint8_t PIN> void noTone(void) {
    TIMSK2 &= ~(1 << OCIE2A);
    mega::digitalWrite<PIN>(_LOW);
}
template<uint8_t PIN, func_ptr USER_FUNC, uint8_t INT_MODE> void attachInterrupt(void) 
{
    static_assert(INT_MODE < 4, "CHANGE = 1, FALLING = 2, RISING = 3");
    static_assert(USER_FUNC != nullptr, "USER_FUNC must be a non-null function pointer; pass &myISR as USER_FUNC");
    uint8_t oldSREG{SREG};
    cli();
    if (PIN == 21) {
        EICRA &= ~((1 << ISC00) | (1 << ISC01));
        if (INT_MODE == FALLING)     { EICRA |= (1 << ISC01); }
        else if (INT_MODE == RISING) { EICRA |= (1 << ISC01) | (1 << ISC00); }
        else if (INT_MODE == CHANGE) { EICRA |= (1 << ISC00); }
        mega::privat::int_funcs[0] = USER_FUNC;
        EIMSK |= (1 << INT0);
    }
    else if (PIN == 20) {
        EICRA &= ~((1 << ISC10) | (1 << ISC11));
        if (INT_MODE == FALLING)     { EICRA |= (1 << ISC11); }
        else if (INT_MODE == RISING) { EICRA |= (1 << ISC11) | (1 << ISC10); }
        else if (INT_MODE == CHANGE) { EICRA |= (1 << ISC10); }
        mega::privat::int_funcs[1] = USER_FUNC;
        EIMSK |= (1 << INT1);
    }
    else if (PIN == 19) {
        EICRA &= ~((1 << ISC20) | (1 << ISC21));
        if (INT_MODE == FALLING)     { EICRA |= (1 << ISC21); }
        else if (INT_MODE == RISING) { EICRA |= (1 << ISC21) | (1 << ISC20); }
        else if (INT_MODE == CHANGE) { EICRA |= (1 << ISC20); }
        mega::privat::int_funcs[2] = USER_FUNC;
        EIMSK |= (1 << INT2);
    }
    else if (PIN == 18) {
        EICRA &= ~((1 << ISC30) | (1 << ISC31));
        if (INT_MODE == FALLING)     { EICRA |= (1 << ISC31); }
        else if (INT_MODE == RISING) { EICRA |= (1 << ISC31) | (1 << ISC30); }
        else if (INT_MODE == CHANGE) { EICRA |= (1 << ISC30); }
        mega::privat::int_funcs[3] = USER_FUNC;
        EIMSK |= (1 << INT3);
    }
    else if (PIN == 2) {
        EICRB &= ~((1 << ISC40) | (1 << ISC41));
        if (INT_MODE == FALLING)     { EICRB |= (1 << ISC41); }
        else if (INT_MODE == RISING) { EICRB |= (1 << ISC41) | (1 << ISC40); }
        else if (INT_MODE == CHANGE) { EICRB |= (1 << ISC40); }
        mega::privat::int_funcs[4] = USER_FUNC;
        EIMSK |= (1 << INT4);
    }
    else if (PIN == 3) {
        EICRB &= ~((1 << ISC50) | (1 << ISC51));
        if (INT_MODE == FALLING)     { EICRB |= (1 << ISC51); }
        else if (INT_MODE == RISING) { EICRB |= (1 << ISC51) | (1 << ISC50); }
        else if (INT_MODE == CHANGE) { EICRB |= (1 << ISC50); }
        mega::privat::int_funcs[5] = USER_FUNC;
        EIMSK |= (1 << INT5);
    }
    else if ((PIN >= 10 && PIN <= 13) || (PIN >= 50 && PIN <= 53)) {
        PCICR |= (1 << PCIE0);
        uint8_t bit{(PIN >= 50 && PIN <= 53) ? (53 - PIN) : (PIN - 6)};
        PCMSK0 |= (1 << bit);
        mega::privat::callbacks[PIN] = USER_FUNC;
        mega::privat::modes[PIN] = INT_MODE;
        mega::privat::last_port_B = PINB;
    }
    else if (PIN >= 62 && PIN <= 69) {
        PCICR |= (1 << PCIE2);
        PCMSK2 |= (1 << (PIN - 62));
        mega::privat::callbacks[PIN] = USER_FUNC;
        mega::privat::modes[PIN] = INT_MODE;
        mega::privat::last_port_K = PINK;
    }
    else if (PIN == 0 || PIN == 14 || PIN == 15) {
        PCICR |= (1 << PCIE1);
        if (PIN == 0)       { PCMSK1 |= (1 << PCINT8); }
        else if (PIN == 15) { PCMSK1 |= (1 << PCINT9); }
        else if (PIN == 14) { PCMSK1 |= (1 << PCINT10); }
        mega::privat::callbacks[PIN] = USER_FUNC;
        mega::privat::modes[PIN] = INT_MODE;
        mega::privat::last_port_E = PINE;
        mega::privat::last_port_J = PINJ;
    }
    SREG = oldSREG;
}
template<uint8_t PIN> void detachInterrupt(void) 
{
    uint8_t oldSREG{SREG};
    cli();
    if (PIN == 21) { EIMSK &= ~(1 << INT0); mega::privat::int_funcs[0] = nullptr; }
    else if (PIN == 20) { EIMSK &= ~(1 << INT1); mega::privat::int_funcs[1] = nullptr; }
    else if (PIN == 19) { EIMSK &= ~(1 << INT2); mega::privat::int_funcs[2] = nullptr; }
    else if (PIN == 18) { EIMSK &= ~(1 << INT3); mega::privat::int_funcs[3] = nullptr; }
    else if (PIN == 2)  { EIMSK &= ~(1 << INT4); mega::privat::int_funcs[4] = nullptr; }
    else if (PIN == 3)  { EIMSK &= ~(1 << INT5); mega::privat::int_funcs[5] = nullptr; }
    else if ((PIN >= 10 && PIN <= 13) || (PIN >= 50 && PIN <= 53)) {
        uint8_t bit{(PIN >= 50 && PIN <= 53) ? (53 - PIN) : (PIN - 6)};
        PCMSK0 &= ~(1 << bit);
        mega::privat::callbacks[PIN] = nullptr;
    }
    else if (PIN >= 62 && PIN <= 69) {
        PCMSK2 &= ~(1 << (PIN - 62));
        mega::privat::callbacks[PIN] = nullptr;
    }
    else if (PIN == 0) {
        PCMSK1 &= ~(1 << PCINT8);
        mega::privat::callbacks[0] = nullptr;
    }
    else if (PIN == 15) {
        PCMSK1 &= ~(1 << PCINT9);
        mega::privat::callbacks[15] = nullptr;
    }
    else if (PIN == 14) {
        PCMSK1 &= ~(1 << PCINT10);
        mega::privat::callbacks[14] = nullptr;
    }
    SREG = oldSREG;
}
#endif