#ifndef FAKE_CLOCK_H
#define FAKE_CLOCK_H

#include "clock.h"

class FakeClock : public Clock {
  public:
    unsigned Milliseconds() override { return time_; }

    void set_time(unsigned new_time) { time_ = new_time; }

  private:
    unsigned time_ = 12345;
};

#endif
