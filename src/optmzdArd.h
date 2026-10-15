// clang-format off
/*
'                        ___                             ___   _______                 ___ 
'  _______   _______   _|   |_   _______   _______   ___|   | |       |  _______   ___|   | fast register Library for Arduino
' |       | |       | |_     _| |       | |___    | |       | |   |   | |    ___| |       | Version 2.0.1
' |   '   | |   '   |   |   |   |  |  | | |   ____| |   '   | |       | |   |     |   '   | Copyright (c) 2026 Tim Seiffer
' |_______| |    ___|   |___|   |__|__|_| |_______| |_______| |___|___| |___|     |_______| All right reserved.
'           |___|    
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#pragma once
#ifndef optmzd_ard_h
#define optmzd_ard_h

#include <Arduino.h>
#include <stdint.h>
#include "util.h"

#ifdef __AVR_ATmega2560__
namespace gpio = mega;
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
namespace gpio = uno;
#else
namespace gpio = ard;
#endif

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  constexpr uint8_t _DEFAULT           {0};
  constexpr uint8_t _EXTERNAL          {1};
  constexpr uint8_t _INTERNAL1V1       {2};
  constexpr uint8_t _INTERNAL{INTERNAL1V1};
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  constexpr uint8_t _DEFAULT              {0};
  constexpr uint8_t _EXTERNAL             {4};
  constexpr uint8_t _INTERNAL1V1          {8};
  constexpr uint8_t _INTERNAL   {INTERNAL1V1};
  constexpr uint8_t _INTERNAL2V56         {9};
  constexpr uint8_t _INTERNAL2V56_EXTCAP {13};
#else  
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
  constexpr uint8_t _INTERNAL1V1  {2};
  constexpr uint8_t _INTERNAL2V56 {3};
#else
  constexpr uint8_t _INTERNAL{3};
#endif
  constexpr uint8_t _DEFAULT {1};
  constexpr uint8_t _EXTERNAL{0};
#endif

constexpr uint8_t _INPUT         {0x00};
constexpr uint8_t _OUTPUT        {0x01};
constexpr uint8_t _INPUT_PULLUP  {0x02};

constexpr uint8_t _HIGH          {0x01};
constexpr uint8_t _LOW           {0x00};

constexpr bool performance       {true};

namespace ard // <- done
{
    void pinMode(uint8_t pin, uint8_t mode);
    void digitalWrite(uint8_t pin, bool val);
    bool digitalRead(uint8_t pin);
    uint16_t analogRead(uint8_t pin);
    void analogReference(uint8_t mode);
    void analogWrite(uint8_t pin, uint8_t val);

    uint32_t millis(void);
    uint32_t micros(void);
    void delay(uint32_t ms);
    void delayMicroseconds(uint32_t us);
    uint32_t pulseIn(uint8_t pin, uint8_t state);
    uint32_t pulseInLong(uint8_t pin, uint8_t state);
    uint32_t pulseIn(uint8_t pin, uint8_t state, uint32_t timeout);
    uint32_t pulseInLong(uint8_t pin, uint8_t state, uint32_t timeout);

    void tone(uint8_t pin, uint16_t frequency);
    void tone(uint8_t pin, uint16_t frequency, uint32_t duration);
    void noTone(uint8_t pin);

    void attachInterrupt(uint8_t pin, func_ptr userFunc, uint8_t mode);
    void detachInterrupt(uint8_t pin);                                  

    // templates 
    template<uint8_t PIN> void pinMode(uint8_t func)    { ::pinMode(PIN, func);                        }
    template<uint8_t PIN> void digitalWrite(bool val)   { ::digitalWrite(PIN, val);                    }
    template<uint8_t PIN> bool digitalRead(void)        { bool ret{::digitalRead(PIN)};    return ret; }
    template<uint8_t PIN> uint16_t analogRead(void)     { uint16_t ret{::analogRead(PIN)}; return ret; }
    template<uint8_t PIN> void analogWrite(uint8_t val) { ::analogWrite(PIN, val);                     }
    template<uint8_t PIN> uint32_t pulseIn(uint8_t state)                       { uint32_t ret{::pulseIn(PIN, state, 1000000UL)    }; return ret; }
    template<uint8_t PIN> uint32_t pulseInLong(uint8_t state)                   { uint32_t ret{::pulseInLong(PIN, state, 1000000UL)}; return ret; }
    template<uint8_t PIN> uint32_t pulseIn(uint8_t state, uint32_t timeout)     { uint32_t ret{::pulseIn(PIN, state, timeout)    };   return ret; }
    template<uint8_t PIN> uint32_t pulseInLong(uint8_t state, uint32_t timeout) { uint32_t ret{::pulseInLong(PIN, state, timeout)};   return ret; }
    template<uint8_t PIN> void tone(uint16_t frequency)                    { ::tone(PIN, frequency);           }
    template<uint8_t PIN> void tone(uint16_t frequency, uint32_t duration) { ::tone(PIN, frequency, duration); }
    template<uint8_t PIN> void noTone(void)                                { ::noTone(PIN);                    }
    template<uint8_t PIN, func_ptr USER_FUNC, uint8_t INT_MODE> void attachInterrupt(void) { ::attachInterrupt(digitalPinToInterrupt(PIN), USER_FUNC, INT_MODE); }
    template<uint8_t PIN> void detachInterrupt(void)                                       { ::detachInterrupt(digitalPinToInterrupt(PIN));                      }

    inline void initTimer(void) {;} // null statement
    // this is if someone wants to write a lib with optmzdArd.h included
    // the thought proccess is if someone writes a lib for every chip they might use something like this:
    // #if defined(__AVR_ATmega328P__)
    // namespace gpio = uno;
    // #else
    // namespace gpio = ard;
    // #endif
    // and than the template variants fall short and you are forced to use the 'normal' version
    // but if i do it like this both version work
}

namespace uno
{
    namespace privat
    {
        extern volatile uint8_t current_pin;
        extern volatile uint32_t toggle_count;
        extern volatile uint32_t timer0_millis;
        extern volatile uint8_t timer0_fract;
        extern volatile uint32_t timer0_overflow_count;

        extern volatile func_ptr callbacks[20];
        extern volatile func_ptr int0_func;
        extern volatile func_ptr int1_func;
        extern volatile uint8_t modes[20];

        extern volatile uint8_t last_port_B;
        extern volatile uint8_t last_port_C;
        extern volatile uint8_t last_port_D;
    }
    void pinMode(uint8_t pin, uint8_t func);     // <- done
    void digitalWrite(uint8_t pin, bool val);    // <- done
    bool digitalRead(uint8_t pin);               // <- done
    uint16_t analogRead(uint8_t pin);            // <- done
    void analogReference(uint8_t mode);          // <- done
    void analogWrite(uint8_t pin, uint8_t val); // <- done

    uint32_t millis(void);                       // <- done
    uint32_t micros(void);                       // <- done
    void delay(uint32_t ms);                     // <- done
    void delayMicroseconds(uint16_t us);         // <- done
    uint32_t pulseIn(uint8_t pin, bool state, uint32_t timeout);                   // <- done
    uint32_t pulseInLong(uint8_t pin, bool state, uint32_t timeout);               // <- done
    uint32_t pulseIn(uint8_t pin, bool state, uint32_t timeout = 1000000L);        // <- done
    uint32_t pulseInLong(uint8_t pin, bool state, uint32_t timeout = 1000000L);    // <- done
    
    void tone(uint8_t pin, uint16_t frequency);                    // <- done
    void tone(uint8_t pin, uint16_t frequency, uint32_t duration); // <- done
    void noTone(uint8_t pin);                                      // <- done
    
    void attachInterrupt(uint8_t pin, func_ptr userFunc, uint8_t mode); // <- done
    void detachInterrupt(uint8_t pin);                                  // <- done

    #include"templates/uno.h"

    uint16_t analogRead(uint8_t pin, bool modePeformance);
    void tone(uint16_t frequency);
    void noTone(void);
    void initTimer(void);
}

namespace mega {
    namespace privat
    {
        extern volatile uint32_t timer_millis;
        extern volatile uint32_t timer_overflow_count;
        extern volatile uint32_t timer_fract;
        extern volatile uint8_t current_pin;
        extern volatile uint32_t toggle_count;

        struct PinMap {
            volatile uint8_t* port;
            volatile uint8_t* ddr;
            volatile uint8_t* pin_reg;
            uint8_t mask;

            bool has_pwm;
            volatile uint8_t* tccra;
            volatile uint8_t* tccrb;
            volatile uint16_t* ocr;
            uint8_t com_bit;
            uint8_t tccra_bits;
            uint8_t tccrb_bits;
        };
        extern const PinMap pin_table[70] PROGMEM;
    }

    void pinMode(uint8_t pin, uint8_t func);     // <- done
    void digitalWrite(uint8_t pin, bool val);    // <- done
    bool digitalRead(uint8_t pin);               // <- done
    uint16_t analogRead(uint8_t pin);            // <- done
    void analogReference(uint8_t mode);          // <- done
    void analogWrite(uint8_t pin, uint8_t val);  // <- done

    uint32_t millis(void);                       // <- done
    uint32_t micros(void);                       // <- done
    void delay(uint32_t ms);                     // <- done
    void delayMicroseconds(uint16_t us);         // <- done
    uint32_t pulseIn(uint8_t pin, bool state, uint32_t timeout);                   // <- done
    uint32_t pulseInLong(uint8_t pin, bool state, uint32_t timeout);               // <- done
    uint32_t pulseIn(uint8_t pin, bool state, uint32_t timeout = 1000000L);        // <- done
    uint32_t pulseInLong(uint8_t pin, bool state, uint32_t timeout = 1000000L);    // <- done
    
    void tone(uint8_t pin, uint16_t frequency);                    // <- done
    void tone(uint8_t pin, uint16_t frequency, uint32_t duration); // <- done
    void noTone(uint8_t pin);                                      // <- x
    
    void attachInterrupt(uint8_t pin, func_ptr userFunc, uint8_t mode); // <- x
    void detachInterrupt(uint8_t pin);                                  // <- x

}

#endif