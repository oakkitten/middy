#ifndef KNOB_H
#define KNOB_H

#include <Arduino.h>

namespace knob {
    class Knob {
        public:
            const byte pin, cc;

            explicit Knob(byte pin, byte cc);
            void check();

            typedef void (*Handler)(const Knob* knob, int value);
            static void set_handler(Knob::Handler handler);

        private:
            int old_value;
    };
}

#endif