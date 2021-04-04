#ifndef EVENTS_H
#define EVENTS_H

#include "key_map.h"

static const int kMaxEvents = 6;
static const int kMaxButtons = msd - mlc + 1;

struct Events {
  Events() { Reset(); }

  void Reset() {
    modifiers = 0;
    for (int i = 0; i < kMaxEvents; ++i) keys[i].key = 0;
    for (int i = 0; i < kMaxButtons; ++i) buttons[i] = 0;
  }

  void AddTappedKey(int key) {
    AddPressedKey(key, -1, -1);
  }

  bool ClearTapping() {
    bool cleared = false;
    for (int i = 0; i < kMaxEvents; ++i) {
      if (keys[i].key && keys[i].row == -1) {
        keys[i].key = 0;
        cleared = true;
      }
    }
    return cleared;
  }

  void AddPressedKey(int key, int row, int col) {
    for (int i = 0; i < kMaxEvents; ++i) {
      if (keys[i].key != 0 &&
          keys[i].row == row && keys[i].col == col) return;
    }
    for (int i = 0; i < kMaxEvents; ++i) {
      if (keys[i].key == 0) {
        keys[i].key = key;
        keys[i].row = row;
        keys[i].col = col;
        // Keyboard.println(key);
        return;
      }
    }
  }

  void RemoveReleasedKey(int row, int col) {
    for (int i = 0; i < kMaxEvents; ++i) {
      if (keys[i].row == row && keys[i].col == col) {
        // Keyboard.println(-keys[i].key);
        keys[i].key = 0;
        return;
      }
    }
  }

  int modifiers = 0;
  struct {
    int key;
    int row;
    int col;
    bool repeated = false;
    unsigned send_time;
  } keys[kMaxEvents];
  int buttons[msd - mlc + 1];
};

#endif
