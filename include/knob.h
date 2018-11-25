#ifndef KNOB_H
#define KNOB_H

#include <Arduino.h>

namespace knob {
    class Knob {
        private:
            byte pin, cc;
            int old_value = -100;
        public:
            explicit Knob(byte pin, byte cc);
            byte get_cc();
            void check();

            typedef void (*KnobHandler)(Knob* knob, int value);
            static void set_handler(Knob::KnobHandler handler);
    };
}

#endif