#ifndef REAL_SENDER_H
#define REAL_SENDER_H

#include "sender.h"

class RealSender : public Sender {
  public:
    void Send(const Events& events) override {
      if (events.modifiers) {
        Keyboard.set_modifier(events.modifiers | 0xE000);
      } else {
        Keyboard.set_modifier(0);
      }
      for (int i = 0; i < 6; ++i) {
        auto key = events.keys[i].key;
        if (IsMediaKey(key) || IsSystemKey(key)) {
          Keyboard.press(key);
          Keyboard.release(key);
          SetKey(i, 0);
        } else {
          SetKey(i, key);
        }
      }
      Keyboard.send_now();

      Mouse.set_buttons(events.buttons[0], events.buttons[1], events.buttons[2]);
      if (events.buttons[mdc - mlc]) {
        Mouse.click();
        Mouse.click();
      }
      if (events.buttons[msu - mlc]) {
        Mouse.scroll(5);
      }
      if (events.buttons[msd - mlc]) {
        Mouse.scroll(-5);
      }
    }

    void Send(const char* message) override { Keyboard.print(message); }
    void Delay(int ms) override { delay(ms); }

  private:
    bool IsMediaKey(int key) const {
      return KEY_MEDIA_PLAY <= key && key <= KEY_MEDIA_VOLUME_DEC;
    }

    bool IsSystemKey(int key) const {
      return KEY_SYSTEM_POWER_DOWN <= key && key <= KEY_SYSTEM_WAKE_UP;
    }

    void SetKey(int index, int key) {
      switch (index) {
        case 0: Keyboard.set_key1(key); break;
        case 1: Keyboard.set_key2(key); break;
        case 2: Keyboard.set_key3(key); break;
        case 3: Keyboard.set_key4(key); break;
        case 4: Keyboard.set_key5(key); break;
        case 5: Keyboard.set_key6(key); break;
      }
    }
};

#endif
