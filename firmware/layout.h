#ifndef LAYOUT_H
#define LAYOUT_H

#include <cstring>

#include "events.h"
#include "matrix.h"
#include "translator.h"

static const int kMaxLayers = 10;
static const int kMaxTaps = 8;

// Tapping on a modifier (including fn) results in a regular key.
struct Tap {
  unsigned char modifier;
  unsigned char key;
};

template <int R, int C>
struct Layer {
  const char* name;
  int layer_id;
  int fn_layer_id;
  const char* translator;
  unsigned char keys[R][C];
  Tap taps[kMaxTaps];
};

template <int R, int C>
class Layout {
  public:
    Layout(int num_layers, const Layer<R, C>* layers,
           int num_translators, Translator* translators[])
      : num_layers_(num_layers),
        num_translators_(num_translators),
        translators_(translators) {
      for (int i = 0; i < num_layers_; ++i) layers_[i] = &layers[i];
      cur_layer_ = FindLayer(l0);
    }

    Events Interpret(int num_entries, const Entry* entries) {
      events_.ClearTapping();
      UpdateFnState(num_entries, entries);
      const auto* layer = fn_pressed_ ? FindLayer(cur_layer_->fn_layer_id)
                                      : cur_layer_;
      for (int i = 0; i < num_entries; ++i) Interpret(entries[i], layer);
      return events_;
    }

    Translator* translator() {
      for (int i = 0; i < num_translators_; ++i) {
        if (translators_[i] &&
            strcmp(cur_layer_->translator, translators_[i]->name()) == 0)
          return translators_[i];
      }
      return nullptr;
    }

  private:
    void UpdateFnState(int num_entries, const Entry* entries) {
      for (int i = 0; i < num_entries; ++i) {
        int key = key_map[cur_layer_->keys[entries[i].row][entries[i].col]];
        if (key == fn) {
          fn_pressed_ = entries[i].pressed;
          if (fn_pressed_) tapping_modifier_ = fn;
          else CheckTapping(fn);
          break;
        }
        if (key == fnl && entries[i].pressed) {
          if (cur_layer_->fn_layer_id != cur_layer_->layer_id) {
            const auto* fn_layer = FindLayer(cur_layer_->fn_layer_id);
            base_layer_ = cur_layer_;
            cur_layer_ = fn_layer;
          } else {
            cur_layer_ = base_layer_;
          }
          break;
        }
      }
    }

    void CheckTapping(int modifier) {
      if (tapping_modifier_ != modifier) return;
      const auto& taps = cur_layer_->taps;
      for (int i = 0; i < kMaxTaps; ++i) {
        if (taps[i].modifier == 0) return;
        if (key_map[taps[i].modifier] == modifier) {
          events_.AddTappedKey(key_map[taps[i].key]);
          tapping_modifier_ = 0;
          return;
        }
      }
    }

    void Interpret(const Entry& entry, const Layer<R,C>* layer) {
      int key = key_map[layer->keys[entry.row][entry.col]];
      if (!key || key == fn || key == fnl) return;

      if (l0 <= key && key < l0 + kMaxLayers) {
        events_.RemoveReleasedKey(entry.row, entry.col);
        auto new_layer = FindLayer(key);
        if (new_layer != cur_layer_) {
          cur_layer_ = new_layer;
          fn_pressed_ = false;
          tapping_modifier_ = 0;
          events_.Reset();
        }
        return;
      }
      if (mlc <= key && key <= msd) {
        tapping_modifier_ = 0;
        events_.buttons[key - mlc] = entry.pressed;
        return;
      }
      if (IsModifier(key)) {
        // TODO: Handle releasing a modifier that doesn't exist in fn layer.
        if (entry.pressed) {
          events_.modifiers |= key;
          tapping_modifier_ = key;
        } else {
          events_.modifiers &= ~key;
          CheckTapping(key);
        }
      } else {
        tapping_modifier_ = 0;
        if (entry.pressed) events_.AddPressedKey(key, entry.row, entry.col);
        else events_.RemoveReleasedKey(entry.row, entry.col);
      }
    }

    bool IsModifier(int key) const {
      return MODIFIERKEY_CTRL <= key && key <= MODIFIERKEY_RIGHT_GUI;
    }

    const Layer<R,C>* FindLayer(int layer_id) const {
      for (int i = 0; i < num_layers_; ++i) {
        if (layers_[i]->layer_id == layer_id) return layers_[i];
      }
      return nullptr;
    }

    const int num_layers_ = 0;
    const Layer<R,C>* layers_[kMaxLayers];
    const Layer<R,C>* cur_layer_ = nullptr;
    const Layer<R,C>* base_layer_ = nullptr;
    const int num_translators_ = 0;
    Translator** const translators_ = nullptr;
    bool fn_pressed_ = false;
    int tapping_modifier_ = 0;
    Events events_;
};

#endif
