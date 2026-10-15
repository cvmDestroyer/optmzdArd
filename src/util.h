// clang-format off
/*                       ___              
'              ___      |___|    ___                 ___
'  ___ ___   _|   |_   _____    |   |               |   |___
' |   |   | |_     _| |_    |   |   | __            |       |
' |   |   |   |   |    _|   |_  |   v   |    ___    |   |   |
' |_______|   |___|   |_______| \______7    |___|   |___|___|
'
  util libary for optmzdArd
*/

#include <Arduino.h>
#include <stdint.h>

#pragma once
#pragma once
#ifndef util_ard_h
#define util_ard_h

using func_ptr = void (*)();

namespace util_uno
{
    static inline bool notPwmPin(uint8_t pin)
    {
        switch (pin) {
            case 3 : return 0;
            case 5 : return 0;
            case 6 : return 0;
            case 9 : return 0;
            case 10: return 0;
            case 11: return 0;
            default: return 1;
        }
    }

    inline void turnOffPWM(uint8_t pin)
    {
        if (notPwmPin(pin))
            return;

        switch (pin) {
            case 3 : TCCR2A &= ~((1 << COM2B1) | (1 << COM2B0)); break;
            case 11: TCCR2A &= ~((1 << COM2A1) | (1 << COM2A0)); break;
            case 10: TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0)); break;
            case 9 : TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0)); break;
            case 5 : TCCR0A &= ~((1 << COM0B1) | (1 << COM0B0)); break;
            case 6 : TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0)); break;
        }
    }
    template <uint8_t PIN>
    struct template_util
    {
        static bool notPwmPin()
        {
            switch (PIN) {
                case 3 : return 0;
                case 5 : return 0;
                case 6 : return 0;
                case 9 : return 0;
                case 10: return 0;
                case 11: return 0;
                default: return 1;
            }
        }
        static void turnOffPWM()
        {
            switch (PIN) {
                case 3 : TCCR2A &= ~((1 << COM2B1) | (1 << COM2B0)); break;
                case 11: TCCR2A &= ~((1 << COM2A1) | (1 << COM2A0)); break;
                case 10: TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0)); break;
                case 9 : TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0)); break;
                case 5 : TCCR0A &= ~((1 << COM0B1) | (1 << COM0B0)); break;
                case 6 : TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0)); break;
            }
        }
    };

}

namespace util_mega
{
    static inline bool notPwmPin(uint8_t pin)
    {
        switch (pin) {
            case 2 : return 0;
            case 3 : return 0;
            case 4 : return 0;
            case 5 : return 0;
            case 6 : return 0;
            case 7 : return 0;
            case 8 : return 0;
            case 9 : return 0;
            case 10: return 0;
            case 11: return 0;
            case 12: return 0;
            case 13: return 0;
            case 44: return 0;
            case 45: return 0;
            case 46: return 0;
            default: return 1;
        }
    }

    inline void turnOffPWM(uint8_t pin)
    {
        if (notPwmPin(pin))
            return;

        switch (pin) {
            case 2 : TCCR3A &= ~(1 << COM3B1) | (1 << COM3B0); break;
            case 3 : TCCR3A &= ~(1 << COM3C1) | (1 << COM3C0); break;
            case 4 : TCCR0A &= ~(1 << COM0B1) | (1 << COM0B0); break;
            case 5 : TCCR3A &= ~(1 << COM3A1) | (1 << COM3A0); break;
            case 6 : TCCR4A &= ~(1 << COM4A1) | (1 << COM4A0); break;
            case 7 : TCCR4A &= ~(1 << COM4B1) | (1 << COM4B0); break;
            case 8 : TCCR4A &= ~(1 << COM4C1) | (1 << COM4C0); break;
            case 9 : TCCR2A &= ~(1 << COM2B1) | (1 << COM2B0); break;
            case 10: TCCR2A &= ~(1 << COM2A1) | (1 << COM2A0); break;
            case 11: TCCR1A &= ~(1 << COM1A1) | (1 << COM1A0); break;
            case 12: TCCR1A &= ~(1 << COM1B1) | (1 << COM1B0); break;
            case 13: TCCR0A &= ~(1 << COM0A1) | (1 << COM0A0); break;
            case 44: TCCR5A &= ~(1 << COM5C1) | (1 << COM5C0); break;
            case 45: TCCR5A &= ~(1 << COM5B1) | (1 << COM5B0); break;
            case 46: TCCR5A &= ~(1 << COM5A1) | (1 << COM5A0); break;
        }
    }
    template <uint8_t PIN>
    struct template_util
    {
        static bool notPwmPin()
        {
            switch (PIN) {
                case 2 : return 0;
                case 3 : return 0;
                case 4 : return 0;
                case 5 : return 0;
                case 6 : return 0;
                case 7 : return 0;
                case 8 : return 0;
                case 9 : return 0;
                case 10: return 0;
                case 11: return 0;
                case 12: return 0;
                case 13: return 0;
                case 44: return 0;
                case 45: return 0;
                case 46: return 0;
                default: return 1;
            }
        }
        static void turnOffPWM()
        {
            switch (PIN) {
                case 2 : TCCR3A &= ~(1 << COM3B1) | (1 << COM3B0); break;
                case 3 : TCCR3A &= ~(1 << COM3C1) | (1 << COM3C0); break;
                case 4 : TCCR0A &= ~(1 << COM0B1) | (1 << COM0B0); break;
                case 5 : TCCR3A &= ~(1 << COM3A1) | (1 << COM3A0); break;
                case 6 : TCCR4A &= ~(1 << COM4A1) | (1 << COM4A0); break;
                case 7 : TCCR4A &= ~(1 << COM4B1) | (1 << COM4B0); break;
                case 8 : TCCR4A &= ~(1 << COM4C1) | (1 << COM4C0); break;
                case 9 : TCCR2A &= ~(1 << COM2B1) | (1 << COM2B0); break;
                case 10: TCCR2A &= ~(1 << COM2A1) | (1 << COM2A0); break;
                case 11: TCCR1A &= ~(1 << COM1A1) | (1 << COM1A0); break;
                case 12: TCCR1A &= ~(1 << COM1B1) | (1 << COM1B0); break;
                case 13: TCCR0A &= ~(1 << COM0A1) | (1 << COM0A0); break;
                case 44: TCCR5A &= ~(1 << COM5C1) | (1 << COM5C0); break;
                case 45: TCCR5A &= ~(1 << COM5B1) | (1 << COM5B0); break;
                case 46: TCCR5A &= ~(1 << COM5A1) | (1 << COM5A0); break;
            }
        }
    };

}
#endif