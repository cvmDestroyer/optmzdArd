// clang-format off
/*
'    ___                                   ___                   ___             
'  _|   |_   _______   _______   _______  |   |      _______   _|   |_   _______ 
' |_     _| |       | |       | |       | |   | __  |____   | |_     _| |       |
'   |   |   |   ' __| |  |  | | |   '   | |   v   | |   .   |   |   |   |   ' __|
'   |___|   |_______| |__|__|_| |    ___| \______7  |_____|_|   |___|   |_______|
'                               |___|    
'                                          ___
'  ___ ___   _______   _______            |   |___
' |   |   | |       | |       |           |       |
' |   |   | |   |   | |   |   |    ___    |   |   |
' |_______| |___|___| |_______|   |___|   |___|___|
'
  unos template libary for optmzdArd
*/
#include <Arduino.h>
#include <stdint.h>
#include "util.h"

#pragma once
#ifndef templates_uno_h
#define templates_uno_h

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

// this took me TWO hours ✌️🫩
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
//         pin  reg  bit  adc   pwm      OCR      TCCRA     TCCRB        COM     tccra bits  tccrb bits
DEFINE_PIN( 0,   D,   0,  255, false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 1,   D,   1,  255, false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 2,   D,   2,  255, false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 3,   D,   3,  255, true ,  &OCR2B,  &TCCR2A,  &TCCR2B,  (1 << COM2B1),  0x03,       0x04   )
DEFINE_PIN( 4,   D,   4,  255, false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 5,   D,   5,  255, true ,  &OCR0B,  &TCCR0A,  &TCCR0B,  (1 << COM0B1),  0x03,       0x03   )
DEFINE_PIN( 6,   D,   6,  255, true ,  &OCR0A,  &TCCR0A,  &TCCR0B,  (1 << COM0A1),  0x03,       0x03   )
DEFINE_PIN( 7,   D,   7,  255, false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 8,   B,   0,  255, false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 9,   B,   1,  255, true ,  &OCR1A,  &TCCR1A,  &TCCR1B,  (1 << COM1A1),  0x01,       0x0B   )
DEFINE_PIN( 10,  B,   2,  255, true ,  &OCR1B,  &TCCR1A,  &TCCR1B,  (1 << COM1B1),  0x01,       0x0B   )
DEFINE_PIN( 11,  B,   3,  255, true ,  &OCR2A,  &TCCR2A,  &TCCR2B,  (1 << COM2A1),  0x03,       0x04   )
DEFINE_PIN( 12,  B,   4,  255, false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 13,  B,   5,  255, false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 14,  C,   0,   0,  false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 15,  C,   1,   1,  false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 16,  C,   2,   2,  false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 17,  C,   3,   3,  false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 18,  C,   4,   4,  false,     0,        0,        0,          0,         0,          0     )
DEFINE_PIN( 19,  C,   5,   5,  false,     0,        0,        0,          0,         0,          0     )

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
template<uint8_t PIN> void digitalWrite(bool val) 
{
    uint8_t oldSREG{SREG};
    cli();

    util_uno::template_util<PIN>::turnOffPWM();
    
    volatile uint8_t* reg{reinterpret_cast<volatile uint8_t*>(hardwearLvl<PIN>::PORT)};
    constexpr uint8_t mask{(1 << hardwearLvl<PIN>::BIT)};

    if (val) {
        *reg |= mask;
    } else {
        *reg &= ~mask;
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
    static_assert(hardwearLvl<PIN>::ADC_CH != 255, "ERROR: analogRead can only read anlog pins PWM DOES NOT COUNT(~pin) onyl A0 - A5");
    uint8_t oldSREG{SREG};
    cli();
    constexpr uint8_t channel{hardwearLvl<PIN>::ADC_CH};
    constexpr uint8_t bit{hardwearLvl<PIN>::BIT};
    
    ADMUX = 0;
    ADMUX |= uno::privat::analog_refrece | channel;
    DIDR0 |= (1 << bit);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    ADCSRA |= (1 << ADSC);
    SREG = oldSREG;
    
    while (ADCSRA & (1 << ADSC))
        ; // null statement

    DIDR0 &= ~(1 << bit);
    
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

    uint32_t cycles = 0;
        
    uint32_t maxCycles = timeout * (F_CPU / 1000000L) / 16;

    while ((*reg & mask) == (state ? mask : 0)) {
        if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;}
    } 
    while ((*reg & mask) != (state ? mask : 0)) {
        if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;}
    }

    uint32_t pulseCycles = 0;
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
    
    const uint32_t startMicros = uno::micros();

    while ((*reg & mask) == (state ? mask : 0)) {
        if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
    }
    while ((*reg & mask) != (state ? mask : 0)) {
        if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
    }

    const uint32_t pulseStart = uno::micros();

    while ((*reg & mask) == (state ? mask : 0)) {
        if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
    }

    SREG = oldSREG;
    return uno::micros() - pulseStart;
}
template<uint8_t PIN> uint32_t pulseIn(bool state) {
    return uno::pulseIn<PIN>(state, 1000000L);
}
template<uint8_t PIN> uint32_t pulseInLong(bool state) {
    return uno::pulseInLong<PIN>(state, 1000000UL);
}
template<uint8_t PIN> void tone(uint16_t frequency) 
{
    if (frequency < 31) { TIMSK2 &= ~(1 << OCIE2A); digitalWrite<PIN>(_LOW); return; }
    
    uno::privat::current_pin = PIN;
    uno::pinMode<PIN>(_OUTPUT);

    uint32_t prescaler = (frequency < 244) ? 1024 : (frequency < 488) ? 256 :
                            (frequency < 976) ? 128  : (frequency < 1953) ? 64 :
                            (frequency < 7812) ? 32  : 8; 
    uint8_t presBit    = (frequency < 244)  ? ((1 << CS22) | (1 << CS21) | (1 << CS20)) :
                            (frequency < 488)  ? ((1 << CS22) | (1 << CS21)) :
                            (frequency < 976)  ? ((1 << CS22) | (1 << CS20)) :
                            (frequency < 1953) ? (1 << CS22) :
                            (frequency < 7812) ? ((1 << CS21) | (1 << CS20)) : (1 << CS21);

    TCCR2A = (1 << WGM21);
    TCCR2B = presBit;
    OCR2A = static_cast<uint8_t>((16000000UL / (2UL * prescaler * frequency)) - 1);
    TIMSK2 |= (1 << OCIE2A);
}
template<uint8_t PIN> void tone(uint16_t frequency, uint32_t duration) {
    uno::tone<PIN>(frequency);

    if (duration > 0) {
        uno::privat::toggle_count = (2UL * frequency * duration) / 1000UL;
    } else { 
        uno::privat::toggle_count = 0; 
    }
}
template<uint8_t PIN> void noTone(void) {
    TIMSK2 &= ~(1 << OCIE2A);
    uno::digitalWrite<PIN>(_LOW);
}
template<uint8_t PIN, func_ptr USER_FUNC, uint8_t INT_MODE> void attachInterrupt(void) 
{
    static_assert(INT_MODE < 4, "CHANGE = 1, FALLING = 2, RISING = 3");
    static_assert(USER_FUNC != nullptr, "USER_FUNC must be a non-null function pointer; pass &myISR as USER_FUNC");
    uint8_t oldSREG{SREG};
    cli();
    uint8_t bit{hardwearLvl<PIN>::BIT};

    if(PIN == 2)
    {
        EICRA &= ~((1 << ISC00) | (1 << ISC01));
        if (INT_MODE == FALLING) {
            EICRA |= (1 << ISC01);
        } else if (INT_MODE == RISING) {
            EICRA |= (1 << ISC01) | (1 << ISC00);
        } else if (INT_MODE == CHANGE) {
            EICRA |= (1 << ISC00);
        }
        uno::privat::int0_func = USER_FUNC; 
        EIFR |= (1 << INTF0);
        EIMSK |= (1 << INT0);
    }
    else if(PIN == 3) 
    {
        EICRA &= ~((1 << ISC10) | (1 << ISC11));
        if (INT_MODE == FALLING) {
            EICRA |= (1 << ISC11);
        } else if (INT_MODE == RISING) {
            EICRA |= (1 << ISC11) | (1 << ISC10);
        } else if (INT_MODE == CHANGE) { 
            EICRA |= (1 << ISC10);
        }
        uno::privat::int1_func = USER_FUNC;
        EIFR |= (1 << INTF1);
        EIMSK |= (1 << INT1);
    }
    else if (PIN <= 7)
    {
        PCICR |= (1 << PCIE2);
        PCMSK2 |= (1 << bit);
        uno::privat::callbacks[PIN] = USER_FUNC;
        uno::privat::modes[PIN] = INT_MODE;
        uno::privat::last_port_D = PIND;
    }
    else if (PIN >= 8 && PIN <= 13) 
    {
        PCICR |= (1 << PCIE0);
        PCMSK0 |= (1 << bit);
        uno::privat::callbacks[PIN] = USER_FUNC;
        uno::privat::modes[PIN] = INT_MODE;
        uno::privat::last_port_B = PINB;
    }
    else if (PIN >= 14 && PIN <= 19) 
    {
        PCICR |= (1 << PCIE1);
        PCMSK1 |= (1 << bit);
        uno::privat::callbacks[PIN] = USER_FUNC;
        uno::privat::modes[PIN] = INT_MODE;
        uno::privat::last_port_C = PINC;
    }
    SREG = oldSREG;
}
template<uint8_t PIN> void detachInterrupt(void) 
{
    uint8_t oldSREG{SREG};
    cli();
    uint8_t bit{hardwearLvl<PIN>::BIT};

    if (PIN == 2) {
        EIMSK &= ~(1 << INT0);
    } else if (PIN == 3) {
        EIMSK &= ~(1 << INT1);
    } else if (PIN <= 7) {
        PCMSK2 &= ~(1 << bit);
    } else if (PIN >= 8 && PIN <= 13) {
        PCMSK0 &= ~(1 << bit);
    } else if (PIN >= 14 && PIN <= 19) {
        PCMSK1 &= ~(1 << bit);
    }
    SREG = oldSREG;
}
#endif