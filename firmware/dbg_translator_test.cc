#include "dbg_translator.h"

#include <assert.h>
#include <stdio.h>
#include "events.h"
#include "fake_sender.h"
#include "layouts/darknight2.h"

int main() {
  DbgTranslator<R,C> translator(row_pins, col_pins,
                                sizeof(layers)/sizeof(layers[0]), layers);
  FakeSender sender;
  Events events;

  // Show layers.
  events.keys[0].key = KEY_L;
  translator.Input(events, &sender);

  // Show pins.
  events.keys[0].key = KEY_P;
  translator.Input(events, &sender);

  puts("PASSED");
  return 0;
}
