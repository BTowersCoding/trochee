#ifndef SENDER_H
#define SENDER_H

#include "events.h"

class Sender {
  public:
    virtual ~Sender() {}
    virtual void Send(const Events& events) = 0;
    virtual void Send(const char* message) = 0;
    virtual void Delay(int ms) = 0;
};

#endif
