#ifndef REAL_CLOCK_H
#define REAL_CLOCK_H

#include "clock.h"

class RealClock : public Clock {
  public:
    unsigned Milliseconds() override { return millis(); }
};

#endif
