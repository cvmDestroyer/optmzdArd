#include "pins_arduino.h"
#include "wiring_private.h"
#include <Arduino.h>
#include <stdint.h>
#include <util/delay.h>
#include "optmzdArd.h"
// clang-format off
#include <avr/io.h>

// THIS HAS HIGH RISK ONLY UNCOMMENT IF YOU KNOW WHAT UR DOING
// THIS COULD KILL OTHER LIBS if you uncomment '#define ENABLE_UNO_HIGH_RISK_HIGH_PRECISION_TIMER_0'
// timer 0 zero will run on ctc NOT fast-pwm and kill pin 5 and 6, the Servo lib and others
// #define ENABLE_UNO_HIGH_RISK_HIGH_PRECISION_TIMER_0
// same thing here just for the timer 5 on mega
// #define ENABLE_MEGA_HIGH_RISK_HIGH_PRECISION_TIMER_5
namespace gpio {
namespace ard 
{
    // normal versions
    void pinMode(uint8_t pin, uint8_t func)                            { ::pinMode(pin, func);                                           }
    void digitalWrite(uint8_t pin, bool val)                           { ::digitalWrite(pin, val);                                       }
    bool digitalRead(uint8_t pin)                                      { bool ret{::digitalRead(pin)};    return ret;                    }
    uint16_t analogRead(uint8_t pin)                                   { uint16_t ret{::analogRead(pin)}; return ret;                    }
    void analogReference(uint8_t mode)                                 { ::analogReference(mode);                                        }
    void analogWrite(uint8_t pin, uint8_t val)                         { ::analogWrite(pin, val);                                        }

    uint32_t millis(void)                                              { uint32_t ret{::millis()}; return ret;                           }
    uint32_t micros(void)                                              { uint32_t ret{::micros()}; return ret;                           }
    void delay(uint32_t ms)                                            { ::delay(ms);                                                    }
    void delayMicroseconds(uint32_t us)                                { ::delayMicroseconds(us);                                        }                   
    uint32_t pulseIn(uint8_t pin, uint8_t state)                       { uint32_t ret{::pulseIn(pin, state, 1000000UL)    }; return ret; }
    uint32_t pulseInLong(uint8_t pin, uint8_t state)                   { uint32_t ret{::pulseInLong(pin, state, 1000000UL)}; return ret; }
    uint32_t pulseIn(uint8_t pin, uint8_t state, uint32_t timeout)     { uint32_t ret{::pulseIn(pin, state, timeout)    };   return ret; }
    uint32_t pulseInLong(uint8_t pin, uint8_t state, uint32_t timeout) { uint32_t ret{::pulseInLong(pin, state, timeout)};   return ret; } 

    void tone(uint8_t pin, uint16_t frequency)                         { ::tone(pin, frequency);                                         }
    void tone(uint8_t pin, uint16_t frequency, uint32_t duration)      { ::tone(pin, frequency, duration);                               }
    void noTone(uint8_t pin)                                           { ::noTone(pin);                                                  }

    void attachInterrupt(uint8_t pin, func_ptr userFunc, uint8_t mode) { ::attachInterrupt(digitalPinToInterrupt(pin), userFunc, mode);  }
    void detachInterrupt(uint8_t pin)                                  { ::detachInterrupt(pin);                                         }
}
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
namespace uno
{
    namespace privat
    {
        volatile uint8_t current_pin{0};
        volatile uint32_t toggle_count{0};
        volatile uint32_t timer0_millis{0};
        volatile uint8_t timer0_fract{0};
        volatile uint32_t timer0_overflow_count{0};
        
        volatile uint8_t analog_refrece{0b010000}; // DEFAULT REFS0 is bit 6 REFS1 is bit 7
        
        volatile func_ptr callbacks[20] = {nullptr};
        volatile func_ptr int0_func = {nullptr};
        volatile func_ptr int1_func = {nullptr};
        volatile uint8_t modes[20] = {0};

        volatile uint8_t last_port_B = 0;
        volatile uint8_t last_port_C = 0;
        volatile uint8_t last_port_D = 0;
    }
    void pinMode(uint8_t pin, uint8_t func)
    {
        uint8_t oldSREG{SREG};
        cli();

        if (pin >= 0 && pin <= 7) 
        {
            if (func == _OUTPUT) {
                DDRD |= (1 << pin);
                PORTD &= ~(1 << pin);
            } else if (func == _INPUT) {
                DDRD &= ~(1 << pin);  
                PORTD &= ~(1 << pin);
            } else if (func == _INPUT_PULLUP) {
                DDRD &= ~(1 << pin);
                PORTD |= (1 << pin);
            }
        }
        else if (pin >= 8 && pin <= 13) 
        {
            if (func == _OUTPUT) {
                DDRB |= (1 << (pin - 8));
                PORTB &= ~(1 << (pin - 8));
            } else if (func == _INPUT) {
                DDRB &= ~(1 << (pin - 8));
                PORTB &= ~(1 << (pin - 8));
            } else if (func == _INPUT_PULLUP) {
                DDRB &= ~(1 << (pin - 8));
                PORTB |= (1 << (pin - 8));
            }
        }
        else if (pin >= 14 && pin <= 19) 
        {
            if (func == _OUTPUT) {
                DDRC |= (1 << (pin - 14));
                PORTC &= ~(1 << (pin - 14));
            } else if (func == _INPUT) {
                DDRC &= ~(1 << (pin - 14));
                PORTC &= ~(1 << (pin - 14));
            } else if (func == _INPUT_PULLUP) {
                DDRC &= ~(1 << (pin - 14));
                PORTC |= (1 << (pin - 14));
            }
        }

        SREG = oldSREG;
    }
    void digitalWrite(uint8_t pin, bool val)
    {
        uint8_t oldSREG{SREG};
        cli();

        util_uno::turnOffPWM(pin);

        if (pin >= 0 && pin <= 7) {
            if (val) {
                PORTD |= (1 << pin);
            } else { 
                PORTD &= ~(1 << pin);
            }
        } else if (pin >= 8 && pin <= 13) {
            if (val) {
                PORTB |= (1 << (pin - 8));
            } else {
                PORTB &= ~(1 << (pin - 8));
            }
        } else if (pin >= 14 && pin <= 19) {
            if (val) {
                PORTC |= (1 << (pin - 14));
            } else {
                PORTC &= ~(1 << (pin - 14));
            }
        }
        
        SREG = oldSREG;
    }
    bool digitalRead(uint8_t pin)
    {
        if (pin >= 0 && pin <= 7) {
            return (PIND & (1 << pin));
        }
        if (pin >= 8 && pin <= 13) {
            return (PINB & (1 << (pin - 8)));
        }
        if (pin >= 14 && pin <= 19) {
            return (PINC & (1 << (pin - 14)));
        }                         
        return LOW;
    }
    uint16_t analogRead(uint8_t pin)
    {
        if (pin >= 20 && pin <= 13) {
            return 0;
        }
        uint8_t oldSREG{SREG};
        cli();
        
        ADMUX = 0;
        ADMUX |= (uno::privat::analog_refrece) | (pin - 14);
        DIDR0 |= (1 << (pin - 14));
        ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

        ADCSRA |= (1 << ADSC);
        SREG = oldSREG;

        while (ADCSRA & (1 << ADSC))
            ;

        DIDR0 &= ~(1 << (pin - 14));
        
        return ADC;
    }
    void analogReference(uint8_t mode)
    {
        // okay this is pretty simpel there are the following register REFS0, REFS1 in ADMUX
        // 10 is 5V/Vcc(DEFAULT), 00 is AREF(EXTERNAL), 11 is 1.1V(INTERNAL)
        // THE FIRST NUM IST ALWAYS REFS0 AND THE SECOND IS REFS1
            
        if (mode == _INTERNAL) {        // INTERNAL (REFS0 = 1, REFS1 = 1)
            uno::privat::analog_refrece = 0b11000000; 
        } else if (mode == _EXTERNAL) { // EXTERNAL (REFS0 = 0, REFS1 = 0)
            uno::privat::analog_refrece = 0b00000000;
        } else {                        // DEFAULT  (REFS0 = 1, REFS1 = 0)
            uno::privat::analog_refrece = 0b01000000;
        }
    }
    void analogWrite(uint8_t pin, uint8_t val) // clang-format off
    { 
        uint8_t oldSREG{SREG};
        cli();
        
        switch (pin) // for explaination take a look in the analogUno file
        {
        case 3:                           
            if (val == 0) {
                TCCR2A &= ~(1 << COM2B1);
                PORTD &= ~(1 << PD3);
            } else if (val == 255) {
                TCCR2A &= ~(1 << COM2B1);
                PORTD |= (1 << PD3);
            } else {
                TCCR2A |= (1 << WGM21) | (1 << WGM20) | (1 << COM2B1);
                TCCR2B = (1 << CS22);
                OCR2B = val;
            } break;

        case 11:
            if (val == 0) {
                TCCR2A &= ~(1 << COM2B1);
                PORTB &= ~(1 << PB3);
            } else if (val == 255) {
                TCCR2A &= ~(1 << COM2B1);
                PORTB |= (1 << PB3);
            } else {
                TCCR2A |= (1 << WGM21) | (1 << WGM20) | (1 << COM2A1);
                TCCR2B = (1 << CS22);
                OCR2A = val;
            } break;

        case 5:
            if (val == 0) {
                TCCR0A &= ~(1 << COM0B1);
                PORTD &= ~(1 << PD5);
            } else if (val == 255) {
                TCCR0A &= ~(1 << COM0B1);
                PORTD |= (1 << PD5);
            } else {
                TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0B1);
                TCCR0B = (1 << CS01) | (1 << CS00);
                OCR0B = val;
            } break;

        case 6:
            if (val == 0) {
                TCCR0A &= ~(1 << COM0A1);
                PORTD &= ~(1 << PD6);
            } else if (val == 255) {
                TCCR0A &= ~(1 << COM0A1);
                PORTD |= (1 << PD6);
            } else {
                TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
                TCCR0B = (1 << CS01) | (1 << CS00);
                OCR0A = val;
            } break;

        case 10:
            if (val == 0) {
                TCCR1A &= ~(1 << COM1B1);
                PORTB &= ~(1 << PB2);
            } else if (val == 255) {
                TCCR1A &= ~(1 << COM1B1);
                PORTB |= (1 << PB2);
            } else {
                TCCR1A |= (1 << WGM10) | (1 << COM1B1);
                TCCR1B = (1 << CS11) | (1 << CS10) | (1 << WGM12);
                OCR1B = val;
            } break;

        case 9:
            if (val == 0) {
                TCCR1A &= ~(1 << COM1A1);
                PORTB &= ~(1 << PB1);
            } else if (val == 255) {
                TCCR1A &= ~(1 << COM1A1);
                PORTB |= (1 << PB1);
            } else {
                TCCR1A |= (1 << WGM10) | (1 << COM1A1);
                TCCR1B = (1 << CS11) | (1 << CS10) | (1 << WGM12);
                OCR1A = val;
            } break;

        default: // incase someone trys it on digital pin
            uno::digitalWrite(pin, (val <= 128) ? false : true);
            break;

        }
        SREG = oldSREG;
    }

    uint32_t millis() {
        uint8_t oldSREG{SREG};
        cli();
        uint32_t ms{uno::privat::timer0_millis};
        SREG = oldSREG;
        return ms;
    }
    uint32_t micros() {
        uint8_t oldSREG = SREG;
        cli();
        
        uint32_t m{uno::privat::timer0_overflow_count};
        uint8_t t{TCNT0};

        #ifdef ENABLE_UNO_HIGH_RISK_HIGH_PRECISION_TIMER_0
            // in ctc mode we look out for OCF0A NOT TOV0
            if ((TIFR0 & (1 << OCF0A)) && (t < 249)) {
                m++;
            }
            SREG = oldSREG;
            return ((m * 250 + t) * 4);
        #else
            // in fast pwm we look for TOV0
            if ((TIFR0 & (1 << TOV0)) && (t < 255)) {
                m++;
            }
            SREG = oldSREG;
            return (((m << 8) + t) * 4);
        #endif
    }
    void delay(uint32_t ms) {
        uint32_t temp{uno::millis()};
        while (uno::millis() - temp < ms)
            ;
    }
    void delayMicroseconds(uint16_t us) {
        // if we use the millis way the call alone talkes to long (2us)
        // and the precisest way is to use the delay_us() function
        // the other method is the following (still not perfectly precise)
        // #if F_CPU >= 16000000L
        //     if (us <= 2) return;
        //     us -= 2; 
        // #endif

        // while (us--) {
        //     __asm__ __volatile__ (
        //         "nop\n\t"
        //         "nop\n\t"
        //         "nop\n\t"
        //         "nop\n\t"
        //         "nop\n\t"
        //         "nop\n\t"
        //         "nop\n\t"
        //         "nop\n\t"
        //         "nop\n\t"
        //         "nop\n\t"
        //     );
        // }
        while (us--) {
            _delay_us(1);
        }
    }
    uint32_t pulseIn(uint8_t pin, bool state, uint32_t timeout)
    {
        uint8_t oldSREG{SREG};
        cli();

        if (pin >= 8 && pin <= 13)
        {
            uint8_t bit{(1 << (pin - 8))};
            uint8_t stateMask = state ? bit : 0;
            
            uint32_t cycles{0};
            uint32_t maxCycles{timeout * (F_CPU / 1000000L) / 16};
            
            while ((PINB & bit) == stateMask) {      // waiting for the unfinished pusle to finish so we can start with a new pulse
                if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;}; // other wise our calculated result might not be what the user wanted
            }
            
            while ((PINB & bit) != stateMask) { // now we can wait for the new pulse
                if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;};
            }
        
            // and now we can calculate the pulseCycles NOT US
            uint32_t pulseCycles{0};
            while ((PINB & bit) == stateMask) {
                if (pulseCycles++ >= maxCycles) {SREG = oldSREG; return 0;};
            }
        
            // here we calc the micros it is made prossible by looking at how 
            // many clock cycles the asm needs for one loop needs
            SREG = oldSREG;
            return (pulseCycles * 16) / (F_CPU / 1000000L);
        } 
        else if (pin >= 0 && pin <= 7) 
        {
            uint8_t bit{(1 << pin)};
            uint8_t stateMask{state ? bit : 0};
            
            uint32_t cycles{0};
            uint32_t maxCycles{timeout * (F_CPU / 1000000L) / 16};

            while ((PIND & bit) == stateMask) {
                if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;}
            } 
            while ((PIND & bit) != stateMask) {
                if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;}
            }

            uint32_t pulseCycles{0};
            while ((PIND & bit) == stateMask) {
                if (pulseCycles++ >= maxCycles) {SREG = oldSREG; return 0;}
            }

            SREG = oldSREG;
            return (pulseCycles * 16) / (F_CPU / 1000000L);
        }
        else if (pin >= 14 && pin <= 19) 
        {
            uint8_t bit{(1 << (pin - 14))};
            uint8_t stateMask{state ? bit : 0};
            
            uint32_t cycles{0};
            uint32_t maxCycles{timeout * (F_CPU / 1000000L) / 16};

            while ((PINC & bit) == stateMask) {
                if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;}
            } 
            while ((PINC & bit) != stateMask) {
                if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;}
            }

            uint32_t pulseCycles{0};
            while ((PINC & bit) == stateMask) {
                if (pulseCycles++ >= maxCycles) {SREG = oldSREG; return 0;}
            }

            SREG = oldSREG;
            return (pulseCycles * 16) / (F_CPU / 1000000L);
        }
    }
    uint32_t pulseInLong(uint8_t pin, bool state, uint32_t timeout) 
    {
        uint8_t oldSREG{SREG};
        cli();

        if (pin >= 8 && pin <= 13) 
        {
            uint8_t bit{(1 << (pin - 8))};
            uint8_t stateMask{state ? bit : 0};
            
            const uint32_t startMicros{uno::micros()};

            while ((PINB & bit) == stateMask) {
                if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
            }
            while ((PINB & bit) != stateMask) {
                if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
            }

            const uint32_t pulseStart{uno::micros()};

            while ((PINB & bit) == stateMask) {
                if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
            }

            SREG = oldSREG;
            return uno::micros() - pulseStart; 
        } 
        else if (pin >= 0 && pin <= 7) 
        {
            uint8_t bit{(1 << pin)};
            uint8_t stateMask{state ? bit : 0};
            const uint32_t startMicros{uno::micros()};

            while ((PIND & bit) == stateMask) {
                if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
            }
            while ((PIND & bit) != stateMask) {
                if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
            }

            const uint32_t pulseStart{uno::micros()};

            while ((PIND & bit) == stateMask) {
                if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
            }

            SREG = oldSREG;
            return uno::micros() - pulseStart; 
        }
        else if (pin >= 14 && pin <= 19) 
        {
            uint8_t bit{(1 << (pin - 14))};
            uint8_t stateMask{state ? bit : 0};
            const uint32_t startMicros{uno::micros()};

            while ((PINC & bit) == stateMask) {
                if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
            }
            while ((PINC & bit) != stateMask) {
                if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
            }

            const uint32_t pulseStart{uno::micros()};

            while ((PINC & bit) == stateMask) {
                if (uno::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
            }

            SREG = oldSREG;
            return uno::micros() - pulseStart; 
        }
    }

    void tone(uint8_t pin, uint16_t frequency)
    {
        // here we do the other method whith the ISR
        // but its basicly the same thing as 'void tone(uint16_t frequency);'
        if (frequency < 31) { TIMSK2 &= ~(1 << OCIE2A); uno::digitalWrite(pin ,_LOW); return; }
        uno::privat::current_pin = pin;
        uno::pinMode(pin, _OUTPUT);

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
    void tone(uint8_t pin, uint16_t frequency, uint32_t duration) {
        uno::tone(pin, frequency);

        if (duration > 0) {
            uno::privat::toggle_count = (2UL * frequency * duration) / 1000UL;
        } else {
            uno::privat::toggle_count = 0;
        }
    }
    void noTone(uint8_t pin) {
        TIMSK2 &= ~(1 << OCIE2A);
        uno::digitalWrite(pin, LOW);
    }
    void attachInterrupt(uint8_t pin, func_ptr userFunc, uint8_t mode) 
    {
        uint8_t oldSREG{SREG};
        cli();

        if(pin == 2) 
        {
            EICRA &= ~(1 << ISC00) | (1 << ISC01);
            if (mode == FALLING) {
                EICRA |= (1 << ISC01);
            } else if (mode == RISING) {
                EICRA |= (1 << ISC01) | (1 << ISC00);
            } else if (mode == CHANGE) {
                EICRA |= (1 << ISC00);
            } 
            uno::privat::int0_func = userFunc; 
            EIMSK |= (1 << INT0);
        }
        else if(pin == 3) 
        {
            EICRA &= ~(1 << ISC10) | (1 << ISC11);
            if (mode == FALLING) {
                EICRA |= (1 << ISC11);
            } else if (mode == RISING) {
                EICRA |= (1 << ISC11) | (1 << ISC10);
            } else if (mode == CHANGE) {
                EICRA |= (1 << ISC10);
            }
            uno::privat::int1_func = userFunc;
            EIMSK |= (1 << INT1);
        }
        else if (pin <= 7)
        {
            PCICR |= (1 << PCIE2);
            PCMSK2 |= (1 << pin);
            uno::privat::callbacks[pin] = userFunc;
            uno::privat::modes[pin] = mode;
            uno::privat::last_port_D = PIND;
        }
        else if (pin >= 8 && pin <= 13) 
        {
            PCICR |= (1 << PCIE0);
            PCMSK0 |= (1 << (pin - 8));
            uno::privat::callbacks[pin] = userFunc;
            uno::privat::modes[pin] = mode;
            uno::privat::last_port_B = PINB;
        }
        else if (pin >= 14 && pin <= 19) 
        {
            PCICR |= (1 << PCIE1);
            PCMSK1 |= (1 << (pin - 14));
            uno::privat::callbacks[pin] = userFunc;
            uno::privat::modes[pin] = mode;
            uno::privat::last_port_C = PINC;
        }
        SREG = oldSREG;
    }
    void detachInterrupt(uint8_t pin) 
    {
        uint8_t oldSREG{SREG};
        cli();
        
        if (pin == 2) {
            EIMSK &= ~(1 << INT0);
        } else if (pin == 3) {
            EIMSK &= ~(1 << INT1);
        } else if (pin <= 7) {
            PCMSK2 &= ~(1 << pin);
        } else if (pin >= 8 && pin <= 13) {
            PCMSK0 &= ~(1 << (pin - 8));
        } else if (pin >= 14 && pin <= 19) {
            PCMSK1 &= ~(1 << (pin - 14));
        }
        SREG = oldSREG;
    }
    // extras
    uint16_t analogRead(uint8_t pin, bool modePeformance)
    {
        if (!modePeformance) {
            return uno::analogRead(pin);
        }
        if (pin >= 20 && pin <= 13) {
            return 0;
        }
        uint8_t oldSREG{SREG};
        cli();
        
        ADMUX = 0;
        ADMUX |= (uno::privat::analog_refrece) | (pin - 14);
        DIDR0 |= (1 << (pin - 14));
        ADCSRA = 0;
        ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0);

        ADCSRA |= (1 << ADSC);
        SREG = oldSREG;
        
        while (ADCSRA & (1 << ADSC))
            ;

        DIDR0 &= ~(1 << (pin - 14));
        
        return ADC;
    }
    void tone(uint16_t frequency)
    {
        // this hole thing is aliitle complexer than the rest but not realy
        // so basicly you cant use ONE prescaler or it will be very out of tune so you just use one
        // till it gets unpresice than you just switch prescaler prescaler can be ajusted with
        // CS2 there are 3 CS20 CS21 CS22 and with them you can create prescaler from 1024 to 8
        // this only works on pin 3 because its the only pin which lets do actual waves BUT there is another way with
        // ISR (Interrupt Serviece Routine) which can be used with every pin cuz it just turns thing on and off wich makes the wave 

        DDRD |= (1 << PB3);
        if (frequency < 61) { // our prescaler is too bad for anthing under 61 so we just turn it off
            TIMSK2 &= ~(1 << OCR2A);
            PORTD &= ~(1 << 3);
            return;
        }

        uint32_t prescaler = (frequency < 244) ? 1024 : (frequency < 488) ? 256 :
                             (frequency < 976) ? 128  : (frequency < 1953) ? 64 :
                             (frequency < 7812) ? 32  : 8; 
        uint8_t presBit    = (frequency < 244)  ? ((1 << CS22) | (1 << CS21) | (1 << CS20)) :
                             (frequency < 488)  ? ((1 << CS22) | (1 << CS21)) :
                             (frequency < 976)  ? ((1 << CS22) | (1 << CS20)) :
                             (frequency < 1953) ? (1 << CS22) :
                             (frequency < 7812) ? ((1 << CS21) | (1 << CS20)) : (1 << CS21);

        uint32_t ocr {static_cast<uint8_t>((16000000UL / (prescaler * frequency)) - 1)};

        TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20); // WGM22 ist in control-register B weil
        TCCR2B = (1 << WGM22) | presBit;                      // weil kein platz mehr in A war (wurde im nachhinein hinzugefügt)
        OCR2A = static_cast<uint8_t>(ocr);
        OCR2B = static_cast<uint8_t>(ocr / 2); // macht ne welle draus das halt auch funktioniert
    }
    void noTone(void) {
        TIMSK2 &= ~(1 << OCR2A);
        uno::digitalWrite(3, LOW);
    }
    void initTimer(void) {
        #ifdef ENABLE_UNO_HIGH_RISK_HIGH_PRECISION_TIMER_0
            uint8_t oldSREG{SREG};
            cli();
            TCCR0A = (1 << WGM01); // ctc NOT pwm (not safe libs like servo might not work inlcuding pin 5 and 6)  
            TCCR0B = (1 << CS01) | (1 << CS00);
  
            OCR0A = 249; // (250 steps are exactly 1s (machines start at 0 so thats why 249))
  
            TIMSK0 |= (1 << OCIE0A);
            SREG = oldSREG;
        #else
            uint8_t oldSREG{SREG};
            cli();
            TCCR0A = (1 << WGM01) | (1 << WGM00); // fast pwm NOT ctc (safe)
            TCCR0B = (1 << CS01) | (1 << CS00);   
        
            TIMSK0 |= (1 << TOIE0);
            SREG = oldSREG;
        #endif
    }
}

ISR(TIMER2_COMPA_vect) { 
    uno::digitalWrite(uno::privat::current_pin, !uno::digitalRead(uno::privat::current_pin));

    if (uno::privat::toggle_count > 0) {
        uno::privat::toggle_count--;
        
        if (uno::privat::toggle_count == 0) {
            TIMSK2 &= ~(1 << OCIE2A);
            uno::digitalWrite(uno::privat::current_pin, LOW);
        }
    } 
} 

// uno timer0
#ifdef ENABLE_UNO_HIGH_RISK_HIGH_PRECISION_TIMER_0
    ISR(TIMER0_COMPA_vect) {
        uno::privat::timer0_millis++;
        uno::privat::timer0_overflow_count++;
    }
#else
    ISR(TIMER0_OVF_vect) {
        uno::privat::timer0_overflow_count++;
        uint32_t m = uno::privat::timer0_millis;
        uint8_t f = uno::privat::timer0_fract;

        m += 1;
        f += 3; // theres alittle error this is just the correction
        if (f >= 125) {
            f -= 125;
            m += 1;
        }

        uno::privat::timer0_fract = f;
        uno::privat::timer0_millis = m;
    }
#endif

ISR(PCINT0_vect) {
    uint8_t current = PINB;
    uint8_t changed = current ^ uno::privat::last_port_B;
    uno::privat::last_port_B = current;

    for (uint8_t bit = 0; bit < 6; bit++) {
        if (changed & (1 << bit)) {
            uint8_t pin = bit + 8;
            func_ptr cb = uno::privat::callbacks[pin];
            if (!cb) {continue;}

            uint8_t mode = uno::privat::modes[pin];
            bool pinState = (current & (1 << bit));

            if (mode == CHANGE || 
               (mode == RISING && pinState) || 
               (mode == FALLING && !pinState)) {
                cb();
            }
        }
    }
}
ISR(PCINT2_vect) {
    uint8_t current = PIND;
    uint8_t changed = current ^ uno::privat::last_port_D;
    uno::privat::last_port_D = current;

    for (uint8_t bit = 0; bit < 8; bit++) {
        if (changed & (1 << bit)) {
            uint8_t pin = bit;
            func_ptr cb = uno::privat::callbacks[pin];
            if (!cb) {continue;}

            uint8_t mode = uno::privat::modes[pin];
            bool pinState = (current & (1 << bit));

            if (mode == CHANGE || 
               (mode == RISING && pinState) || 
               (mode == FALLING && !pinState)) {
                cb();
            }
        }
    }
}
ISR(PCINT1_vect) {
    uint8_t current = PINC;
    uint8_t changed = current ^ uno::privat::last_port_C;
    uno::privat::last_port_C = current;

    for (uint8_t bit = 0; bit < 6; bit++) {
        if (changed & (1 << bit)) {
            uint8_t pin = bit + 14;
            func_ptr cb = uno::privat::callbacks[pin];
            if (!cb) {continue;}

            uint8_t mode = uno::privat::modes[pin];
            bool pinState = (current & (1 << bit));

            if (mode == CHANGE || 
               (mode == RISING && pinState) || 
               (mode == FALLING && !pinState)) {
                cb();
            }
        }
    }
}
ISR(INT0_vect) {
    uno::privat::int0_func();
}
ISR(INT1_vect) {
    uno::privat::int1_func();
}
#endif

#ifdef __AVR_ATmega2560__

#include <avr/pgmspace.h>

namespace mega {
    namespace privat 
    {
        volatile uint32_t timer_overflow_count{0};
        volatile uint8_t timer_fract{0};
        volatile uint32_t timer_millis{0};
        volatile uint8_t current_pin{};
        volatile uint32_t toggle_count{};

        uint8_t analog_reference {0b01000000};
        const PinMap pin_table[70] PROGMEM = {

            { &PORTE, &DDRE, &PINE, _BV(0), false,    0,       0,                  0,                   0,       0,    0   }, // 0
            { &PORTE, &DDRE, &PINE, _BV(1), false,    0,       0,                  0,                   0,       0,    0   }, // 1
            { &PORTE, &DDRE, &PINE, _BV(4), true,  &TCCR3A, &TCCR3B, (volatile uint16_t*)&OCR3B, (1 << COM3B1), 0x01, 0x0B }, // 2
            { &PORTE, &DDRE, &PINE, _BV(5), true,  &TCCR3A, &TCCR3B, (volatile uint16_t*)&OCR3C, (1 << COM3C1), 0x01, 0x0B }, // 3
            { &PORTG, &DDRG, &PING, _BV(5), true,  &TCCR0A, &TCCR0B, (volatile uint16_t*)&OCR0B, (1 << COM0B1), 0x03, 0x03 }, // 4
            { &PORTE, &DDRE, &PINE, _BV(3), true,  &TCCR3A, &TCCR3B, (volatile uint16_t*)&OCR3A, (1 << COM3A1), 0x01, 0x0B }, // 5
            { &PORTH, &DDRH, &PINH, _BV(3), true,  &TCCR4A, &TCCR4B, (volatile uint16_t*)&OCR4A, (1 << COM4A1), 0x01, 0x0B }, // 6
            { &PORTH, &DDRH, &PINH, _BV(4), true,  &TCCR4A, &TCCR4B, (volatile uint16_t*)&OCR4B, (1 << COM4B1), 0x01, 0x0B }, // 7
            { &PORTH, &DDRH, &PINH, _BV(5), true,  &TCCR4A, &TCCR4B, (volatile uint16_t*)&OCR4C, (1 << COM4C1), 0x01, 0x0B }, // 8
            { &PORTH, &DDRH, &PINH, _BV(6), true,  &TCCR2A, &TCCR2B, (volatile uint16_t*)&OCR2B, (1 << COM2B1), 0x03, 0x04 }, // 9
            { &PORTB, &DDRB, &PINB, _BV(4), true,  &TCCR2A, &TCCR2B, (volatile uint16_t*)&OCR2A, (1 << COM2A1), 0x03, 0x04 }, // 10
            { &PORTB, &DDRB, &PINB, _BV(5), true,  &TCCR1A, &TCCR1B, (volatile uint16_t*)&OCR1A, (1 << COM1A1), 0x01, 0x0B }, // 11
            { &PORTB, &DDRB, &PINB, _BV(6), true,  &TCCR1A, &TCCR1B, (volatile uint16_t*)&OCR1B, (1 << COM1B1), 0x01, 0x0B }, // 12
            { &PORTB, &DDRB, &PINB, _BV(7), true,  &TCCR0A, &TCCR0B, (volatile uint16_t*)&OCR0A, (1 << COM0A1), 0x03, 0x03 }, // 13

            { &PORTJ, &DDRJ, &PINJ, _BV(1), false,    0,       0,                  0,                   0,       0,    0   }, // 14
            { &PORTJ, &DDRJ, &PINJ, _BV(0), false,    0,       0,                  0,                   0,       0,    0   }, // 15
            { &PORTH, &DDRH, &PINH, _BV(1), false,    0,       0,                  0,                   0,       0,    0   }, // 16
            { &PORTH, &DDRH, &PINH, _BV(0), false,    0,       0,                  0,                   0,       0,    0   }, // 17
            { &PORTD, &DDRD, &PIND, _BV(3), false,    0,       0,                  0,                   0,       0,    0   }, // 18
            { &PORTD, &DDRD, &PIND, _BV(2), false,    0,       0,                  0,                   0,       0,    0   }, // 19
            { &PORTD, &DDRD, &PIND, _BV(1), false,    0,       0,                  0,                   0,       0,    0   }, // 20
            { &PORTD, &DDRD, &PIND, _BV(0), false,    0,       0,                  0,                   0,       0,    0   }, // 21

            { &PORTA, &DDRA, &PINA, _BV(0), false,    0,       0,                  0,                   0,       0,    0   }, // 22
            { &PORTA, &DDRA, &PINA, _BV(1), false,    0,       0,                  0,                   0,       0,    0   }, // 23
            { &PORTA, &DDRA, &PINA, _BV(2), false,    0,       0,                  0,                   0,       0,    0   }, // 24
            { &PORTA, &DDRA, &PINA, _BV(3), false,    0,       0,                  0,                   0,       0,    0   }, // 25
            { &PORTA, &DDRA, &PINA, _BV(4), false,    0,       0,                  0,                   0,       0,    0   }, // 26
            { &PORTA, &DDRA, &PINA, _BV(5), false,    0,       0,                  0,                   0,       0,    0   }, // 27
            { &PORTA, &DDRA, &PINA, _BV(6), false,    0,       0,                  0,                   0,       0,    0   }, // 28
            { &PORTA, &DDRA, &PINA, _BV(7), false,    0,       0,                  0,                   0,       0,    0   }, // 29

            { &PORTC, &DDRC, &PINC, _BV(7), false,    0,       0,                  0,                   0,       0,    0   }, // 30
            { &PORTC, &DDRC, &PINC, _BV(6), false,    0,       0,                  0,                   0,       0,    0   }, // 31
            { &PORTC, &DDRC, &PINC, _BV(5), false,    0,       0,                  0,                   0,       0,    0   }, // 32
            { &PORTC, &DDRC, &PINC, _BV(4), false,    0,       0,                  0,                   0,       0,    0   }, // 33
            { &PORTC, &DDRC, &PINC, _BV(3), false,    0,       0,                  0,                   0,       0,    0   }, // 34
            { &PORTC, &DDRC, &PINC, _BV(2), false,    0,       0,                  0,                   0,       0,    0   }, // 35
            { &PORTC, &DDRC, &PINC, _BV(1), false,    0,       0,                  0,                   0,       0,    0   }, // 36
            { &PORTC, &DDRC, &PINC, _BV(0), false,    0,       0,                  0,                   0,       0,    0   }, // 37

            { &PORTD, &DDRD, &PIND, _BV(7), false,    0,       0,                  0,                   0,       0,    0   }, // 38
            { &PORTG, &DDRG, &PING, _BV(2), false,    0,       0,                  0,                   0,       0,    0   }, // 39
            { &PORTG, &DDRG, &PING, _BV(1), false,    0,       0,                  0,                   0,       0,    0   }, // 40
            { &PORTG, &DDRG, &PING, _BV(0), false,    0,       0,                  0,                   0,       0,    0   }, // 41

            { &PORTL, &DDRL, &PINL, _BV(7), false,    0,       0,                  0,                   0,       0,    0   }, // 42
            { &PORTL, &DDRL, &PINL, _BV(6), false,    0,       0,                  0,                   0,       0,    0   }, // 43
            { &PORTL, &DDRL, &PINL, _BV(5), true,  &TCCR5A, &TCCR5B, (volatile uint16_t*)&OCR5C, (1 << COM5C1), 0x01, 0x0B }, // 44
            { &PORTL, &DDRL, &PINL, _BV(4), true,  &TCCR5A, &TCCR5B, (volatile uint16_t*)&OCR5B, (1 << COM5B1), 0x01, 0x0B }, // 45
            { &PORTL, &DDRL, &PINL, _BV(3), true,  &TCCR5A, &TCCR5B, (volatile uint16_t*)&OCR5A, (1 << COM5A1), 0x01, 0x0B }, // 46
            { &PORTL, &DDRL, &PINL, _BV(2), false,    0,       0,                  0,                   0,       0,    0   }, // 47
            { &PORTL, &DDRL, &PINL, _BV(1), false,    0,       0,                  0,                   0,       0,    0   }, // 48
            { &PORTL, &DDRL, &PINL, _BV(0), false,    0,       0,                  0,                   0,       0,    0   }, // 49

            { &PORTB, &DDRB, &PINB, _BV(3), false,    0,       0,                  0,                   0,       0,    0   }, // 50
            { &PORTB, &DDRB, &PINB, _BV(2), false,    0,       0,                  0,                   0,       0,    0   }, // 51
            { &PORTB, &DDRB, &PINB, _BV(1), false,    0,       0,                  0,                   0,       0,    0   }, // 52
            { &PORTB, &DDRB, &PINB, _BV(0), false,    0,       0,                  0,                   0,       0,    0   }, // 53

            { &PORTF, &DDRF, &PINF, _BV(0), false,    0,       0,                  0,                   0,       0,    0   }, // 54
            { &PORTF, &DDRF, &PINF, _BV(1), false,    0,       0,                  0,                   0,       0,    0   }, // 55
            { &PORTF, &DDRF, &PINF, _BV(2), false,    0,       0,                  0,                   0,       0,    0   }, // 56
            { &PORTF, &DDRF, &PINF, _BV(3), false,    0,       0,                  0,                   0,       0,    0   }, // 57 
            { &PORTF, &DDRF, &PINF, _BV(4), false,    0,       0,                  0,                   0,       0,    0   }, // 58
            { &PORTF, &DDRF, &PINF, _BV(5), false,    0,       0,                  0,                   0,       0,    0   }, // 59
            { &PORTF, &DDRF, &PINF, _BV(6), false,    0,       0,                  0,                   0,       0,    0   }, // 60
            { &PORTF, &DDRF, &PINF, _BV(7), false,    0,       0,                  0,                   0,       0,    0   }, // 61

            { &PORTK, &DDRK, &PINK, _BV(0), false,    0,       0,                  0,                   0,       0,    0   }, // 62
            { &PORTK, &DDRK, &PINK, _BV(1), false,    0,       0,                  0,                   0,       0,    0   }, // 63
            { &PORTK, &DDRK, &PINK, _BV(2), false,    0,       0,                  0,                   0,       0,    0   }, // 64
            { &PORTK, &DDRK, &PINK, _BV(3), false,    0,       0,                  0,                   0,       0,    0   }, // 65
            { &PORTK, &DDRK, &PINK, _BV(4), false,    0,       0,                  0,                   0,       0,    0   }, // 66
            { &PORTK, &DDRK, &PINK, _BV(5), false,    0,       0,                  0,                   0,       0,    0   }, // 67
            { &PORTK, &DDRK, &PINK, _BV(6), false,    0,       0,                  0,                   0,       0,    0   }, // 68
            { &PORTK, &DDRK, &PINK, _BV(7), false,    0,       0,                  0,                   0,       0,    0   }  // 69
        };
    }

    void pinMode(uint8_t pin, uint8_t func) {
        if (pin >= 70) {return;}

        volatile uint8_t* ddr  {reinterpret_cast<volatile uint8_t*>(pgm_read_byte(&mega::privat::pin_table[pin].ddr))};
        volatile uint8_t* port {reinterpret_cast<volatile uint8_t*>(pgm_read_byte(&mega::privat::pin_table[pin].port))};
        uint8_t mask{pgm_read_byte(&mega::privat::pin_table[pin].mask)};

        uint8_t oldSREG {SREG};
        cli();

        if (func == _OUTPUT) {
            *ddr |= mask;
            *port &= ~mask;
        } else if (func == _INPUT) {
            *ddr &= ~mask;
            *port &= ~mask;
        } else if (func == _INPUT_PULLUP) {
            *ddr &= ~mask;
            *port |= mask;
        }

        SREG = oldSREG;
    }

    void digitalWrite(uint8_t pin, bool val) {
        if (pin >= 70) {return;}

        volatile uint8_t* port {reinterpret_cast<volatile uint8_t*>(pgm_read_byte(&mega::privat::pin_table[pin].port))};
        uint8_t mask           {pgm_read_byte(&mega::privat::pin_table[pin].mask)};

        uint8_t oldSREG {SREG};
        cli();

        util_mega::turnOffPWM(pin);

        if (val) {
            *port |= mask;
        } else {
            *port &= ~mask;
        }

        SREG = oldSREG;
    }

    bool digitalRead(uint8_t pin) {
        if (pin >= 70) {return false;}

        volatile uint8_t* pin_reg {reinterpret_cast<volatile uint8_t*>(pgm_read_byte(&mega::privat::pin_table[pin].pin_reg))};
        uint8_t mask              {pgm_read_byte(&mega::privat::pin_table[pin].mask)};

        return (*pin_reg & mask);
    }
    uint16_t analogRead(uint8_t pin) 
    {
        // explaination is in the analogMega file
        // but is basicly the same as the uno variant only diffrence is
        // there are more pins
        uint8_t channel {pin - 54};
        if (channel > 15) {return 0;}

        uint8_t oldSREG {SREG};
        cli();

        if (channel >= 8) {
            ADCSRB |= (1 << MUX5);
            ADMUX = mega::privat::analog_reference | ((channel - 8) & 0x07);
            DIDR2 |= (1 << (channel - 8));
        } else {
            ADCSRB &= ~(1 << MUX5);
            ADMUX = mega::privat::analog_reference | (channel & 0x07);
            DIDR0 |= (1 << channel);
        }
    
        ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

        ADCSRA |= (1 << ADSC);
        SREG = oldSREG;
        while (ADCSRA & (1 << ADSC))
            ;

        if (channel >= 8) {
            DIDR2 &= ~(1 << (channel - 8));
        } else {
            DIDR0 &= ~(1 << channel);
        }

        return ADC;
    }
    void analogReference(uint8_t mode)
    {     
        if (mode == _INTERNAL2V56) {
            mega::privat::analog_reference = 0b11000000; 
        } else if (mode == _INTERNAL1V1) {
            mega::privat::analog_reference = 0b10000000;
        } else if (mode == _EXTERNAL) {
            mega::privat::analog_reference = 0b00000000;
        } else {
            mega::privat::analog_reference = 0b01000000;
        }
    }
    void analogWrite(uint8_t pin, uint8_t val)
    {
        if (pin >= 70) {return;}

        bool has_pwm{pgm_read_byte(&mega::privat::pin_table[pin].has_pwm)};
        
        if (!has_pwm) {
            mega::digitalWrite(pin, (val <= 128) ? false : true);
            return;
        }

        volatile uint8_t* tccra {reinterpret_cast<volatile uint8_t*>(pgm_read_byte(&mega::privat::pin_table[pin].tccra))};
        volatile uint8_t* tccrb {reinterpret_cast<volatile uint8_t*>(pgm_read_byte(&mega::privat::pin_table[pin].tccrb))};
        volatile uint16_t* ocr  {reinterpret_cast<volatile uint16_t*>(pgm_read_word(&mega::privat::pin_table[pin].ocr))};
        uint8_t com_bit         {pgm_read_byte(&mega::privat::pin_table[pin].com_bit)};
        uint8_t tccra_bits      {pgm_read_byte(&mega::privat::pin_table[pin].tccra_bits)};
        uint8_t tccrb_bits      {pgm_read_byte(&mega::privat::pin_table[pin].tccrb_bits)};

        uint8_t oldSREG{SREG};
        cli();

        if (val == 0) {
            *tccra &= ~com_bit;
            mega::digitalWrite(pin, LOW);
        } else if (val == 255) {
            *tccra &= ~com_bit;
            mega::digitalWrite(pin, HIGH);
        } else {
            *tccra |= com_bit | tccra_bits;
            *tccrb = tccrb_bits;
            *ocr = val;
        }

        SREG = oldSREG;
    }
    uint32_t millis(void) {
        uint8_t oldSREG{SREG};
        cli();
        uint32_t ms{mega::privat::timer_millis};
        SREG = oldSREG;
        return ms;
    }
    uint32_t micros(void) {
        uint8_t oldSREG{SREG};
        cli();

        uint32_t ms{mega::privat::timer_millis};
        #ifdef ENABLE_MEGA_HIGH_RISK_HIGH_PRECISION_TIMER_5
            uint16_t t{TCNT5};

            if ((TIFR5 & (1 << OCF5A)) && (t < 249)) {
                ms++;
            }
            SREG = oldSREG;
            return (ms * 1000UL) + (t * 4UL);
        #else
            uint8_t t{TCNT0};
            if ((TIFR0 & (1 << TOV0)) && (t < 255)) {
                ms++;
            }
            SREG = oldSREG;
            return (((ms << 8) + t) * 4);
        #endif
    }
    void delay(uint32_t ms) {
        uint32_t temp{mega::millis()};
        while (mega::millis() - temp < ms)
            ;
    }
    void delayMicroseconds(uint16_t us) {
        while (us--) {
            _delay_us(1);
        }
    }
    uint32_t pulseIn(uint8_t pin, bool state, uint32_t timeout)
    {
        uint8_t oldSREG{SREG};
        cli();

        volatile uint8_t* pin_reg{reinterpret_cast<volatile uint8_t *>(pgm_read_byte(&mega::privat::pin_table[pin].pin_reg))};
        uint8_t bit{pgm_read_byte(mega::privat::pin_table[pin].mask)};

        uint8_t stateMask{state ? bit : 0};
        
        uint32_t cycles{0};
        uint32_t maxCycles{timeout * (F_CPU / 1000000L) / 16};
        
        while ((*pin_reg & bit) == stateMask) { 
            if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;};
        }
        
        while ((*pin_reg & bit) != stateMask) {
            if (cycles++ >= maxCycles) {SREG = oldSREG; return 0;};
        }
    
        uint32_t pulseCycles{0};
        while ((*pin_reg & bit) == stateMask) {
            if (pulseCycles++ >= maxCycles) {SREG = oldSREG; return 0;};
        }
    
        SREG = oldSREG;
        return (pulseCycles * 16) / (F_CPU / 1000000L);
    }
    uint32_t pulseInLong(uint8_t pin, bool state, uint32_t timeout) 
    {
        uint8_t oldSREG{SREG};
        cli();

        volatile uint8_t* pin_reg{reinterpret_cast<volatile uint8_t *>(pgm_read_byte(&mega::privat::pin_table[pin].pin_reg))};
        uint8_t bit{pgm_read_byte(mega::privat::pin_table[pin].mask)};
        
        uint8_t stateMask{state ? bit : 0};
        const uint32_t startMicros{mega::micros()};

        while ((*pin_reg & bit) == stateMask) {
            if (mega::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
        }
        while ((*pin_reg & bit) != stateMask) {
            if (mega::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
        }

        const uint32_t pulseStart{mega::micros()};

        while ((*pin_reg & bit) == stateMask) {
            if (mega::micros() - startMicros >= timeout) {SREG = oldSREG; return 0;}
        }

        SREG = oldSREG;
        return mega::micros() - pulseStart; 
    }

    void tone(uint8_t pin, uint16_t frequency)
    {
        if (frequency < 31) { TIMSK2 &= ~(1 << OCIE2A); mega::digitalWrite(pin ,_LOW); return; }
        mega::privat::current_pin = pin;
        mega::pinMode(pin, _OUTPUT);

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
    void tone(uint8_t pin, uint16_t frequency, uint32_t duration) {
        mega::tone(pin, frequency);

        if (duration > 0) {
            mega::privat::toggle_count = (2UL * frequency * duration) / 1000UL;
        } else {
            mega::privat::toggle_count = 0;
        }
    }
    void noTone(uint8_t pin) {}

    void attachInterrupt(uint8_t pin, func_ptr userFunc, uint8_t mode) {}
    void detachInterrupt(uint8_t pin) {}


    void initTimer(void) {
        #ifdef ENABLE_MEGA_HIGH_RISK_HIGH_PRECISION_TIMER_5
            uint8_t oldSREG{SREG};
            cli();
            
            TCCR5A = 0; 
            TCCR5B = (1 << WGM52) | (1 << CS51) | (1 << CS50);

            OCR5A = 249;

            TIMSK5 |= (1 << OCIE5A);

            SREG = oldSREG;
        #else
            uint8_t oldSREG{SREG};
            cli();
            TCCR0A = (1 << WGM01) | (1 << WGM00); // fast pwm NOT ctc (safe)
            TCCR0B = (1 << CS01) | (1 << CS00);   
        
            TIMSK0 |= (1 << TOIE0);
            SREG = oldSREG;
        #endif
    }
}

#ifdef ENABLE_MEGA_HIGH_RISK_HIGH_PRECISION_TIMER_5
    ISR(TIMER5_COMPA_vect) {
        mega::privat::timer_millis++;
    }
#else
    ISR(TIMER0_OVF_vect) {
        mega::privat::timer_overflow_count++;
        uint32_t m{mega::privat::timer_millis};
        uint8_t f{mega::privat::timer_fract};

        m += 1;
        f += 3; // theres alittle error this is just the correction
        if (f >= 125) {
            f -= 125;
            m += 1;
        }

        mega::privat::timer_fract = f;
        mega::privat::timer_millis = m;
    }
    
#endif

ISR(TIMER2_COMPA_vect) { 
    mega::digitalWrite(mega::privat::current_pin, !mega::digitalRead(mega::privat::current_pin));

    if (mega::privat::toggle_count > 0) {
        mega::privat::toggle_count--;
        
        if (mega::privat::toggle_count == 0) {
            TIMSK2 &= ~(1 << OCIE2A);
            mega::digitalWrite(mega::privat::current_pin, LOW);
        }
    } 
}
}
#endif