#ifndef DBG_TRANSLATOR_H
#define DBG_TRANSLATOR_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "key_map.h"
#include "layout.h"
#include "translator.h"

template <int R, int C>
class DbgTranslator : public Translator {
  public:
    DbgTranslator(const int* row_pins, const int* col_pins, int num_layers,
                  const Layer<R,C>* layers)
      : row_pins_(row_pins), col_pins_(col_pins),
        num_layers_(num_layers), layers_(layers) {}

    const char* name() const { return "dbg"; }

    void Input(Events events, Sender* sender) override {
      switch (events.keys[0].key) {
        case KEY_L: ShowLayers(sender); break;
        case KEY_P: ShowPins(sender); break;
      }
    }

    void AutoRepeat() override {}

  private:
    void ShowLayers(Sender* sender) {
      char message[200];

      for (int l = 0; l < num_layers_; ++l) {
        const auto& layer = layers_[l];
        sprintf(message, "layer[%d] = {\n", l);
        sprintf(message + strlen(message), "  name = \"%s\",\n", layer.name);
        sprintf(message + strlen(message), "  layer_id = %d,\n", layer.layer_id);
        sprintf(message + strlen(message), "  fn_layer_id = %d,\n", layer.fn_layer_id);
        sprintf(message + strlen(message), "  translator = \"%s\",\n", layer.translator);
        sender->Send(message);
        for (int r = 0; r < R; ++r) {
          sprintf(message, "  row[%d] = { ", r);
          for (int c = 0; c < C; ++c) {
            auto key = layer.keys[r][c];
            if (isprint(key))
              sprintf(message + strlen(message), "'%c'%c ", key, (c < C - 1 ? ',' : ' '));
            else
              sprintf(message + strlen(message), "%3d%c ", key, (c < C - 1 ? ',' : ' '));
          }
          sprintf(message + strlen(message), "},\n");
          sender->Send(message);
        }
        sprintf(message, "  taps = { ");
        for (int t = 0; t < kMaxTaps; ++t) {
          if (layer.taps[t].modifier == 0) break;
          sprintf(message + strlen(message), "%d->%d ", layer.taps[t].modifier,
                  layer.taps[t].key);
        }
        sprintf(message + strlen(message), "},\n");
        sender->Send(message);

        sprintf(message, "};\n");
        sender->Send(message);
      }
    }

    void ShowPins(Sender* sender) {
      char message[200];

      sprintf(message, "row_pins[%d] = { ", R);
      for (int r = 0; r < R; ++r)
        sprintf(message + strlen(message), "%d%c ", row_pins_[r],
                (r < R-1 ? ',' : ' '));
      sprintf(message + strlen(message), "};\n");
      sender->Send(message);

      sprintf(message, "col_pins[%d] = { ", C);
      for (int c = 0; c < C; ++c)
        sprintf(message + strlen(message), "%d%c ", col_pins_[c],
                (c < C-1 ? ',' : ' '));
      sprintf(message + strlen(message), "};\n");
      sender->Send(message);
    }

    const int* row_pins_;
    const int* col_pins_;
    const int num_layers_;
    const Layer<R,C>* layers_;
};

#endif
