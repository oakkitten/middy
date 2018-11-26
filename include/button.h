#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

namespace button {
    class Button {
        public:
            enum class Kind {
                toggle,
                push,
                click,
            };

            enum class Event : byte {
                press = 127,
                release = 0,
                click = 64,
                toggle_on = 126,
                toggle_off = 1
            };

            inline static byte midi_value(Event e) {
                return static_cast<byte>(e) >= 64 ? 127 : 0;
            };

            const byte pin, tag, off_state;
            const Kind kind;

            explicit Button(byte pin, byte tag, byte off_state, Kind kind);
            void check();

            typedef void (*ButtonHandler)(Button* button, Event event);
            static void set_handler(Button::ButtonHandler handler);
        
        private:
            bool pressed, toggle_state;

            bool last_reading;
            unsigned long last_reading_change_time;
            
            void dispatch_change();
    };
}

#endif