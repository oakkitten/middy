#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

namespace button {
    enum Kind {
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

    inline byte midi_value(Event e) {
        return static_cast<byte>(e) >= 64 ? 127 : 0;
    };

    class Button {
        private:
            byte pin, tag, off_state;
            Kind kind;
            
            bool pressed;
            bool toggle_state;

            int last_reading;
            unsigned long last_reading_change_time;
            
            void dispatch_change();

        public:
            explicit Button(byte pin, byte tag, byte off_state, Kind kind);
            byte get_tag();
            Kind get_kind();
            void check();

            typedef void (*ButtonHandler)(Button* button, Event event);
            static void set_handler(Button::ButtonHandler handler);


    };
}

#endif