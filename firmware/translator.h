#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "events.h"
#include "sender.h"

class Translator {
  public:
    virtual ~Translator() {}

    // Name of the translator, e.g. "vi".
    virtual const char* name() const = 0;

    // Input events to the converter. There can be many converted events to be
    // sent. Call the sender directly to avoid buffering too many events.
    // Returns true if the caller should exit the converter layer after the
    // input.
    virtual void Input(Events events, Sender* sender) = 0;

    virtual void AutoRepeat() = 0;
};

#endif
