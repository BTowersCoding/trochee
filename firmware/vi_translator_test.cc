#include "vi_translator.h"

#include <assert.h>
#include <stdio.h>
#include "events.h"
#include "fake_clock.h"
#include "fake_sender.h"

class Tester {
  public:
    Tester() : translator_(&clock_) {}

    void Emit(int key) {
      Press(key);
      Release(key);
    }

    void Press(int key) {
      events_.keys[0].key = key;
      translator_.Input(events_, &sender_);
      translator_.AutoRepeat();
      translator_.AutoRepeat();
    }

    void Release(int key) {
      events_.keys[0].key = 0;
      translator_.Input(events_, &sender_);
      translator_.AutoRepeat();
      translator_.AutoRepeat();
    }

    void PressModifier(int modifier) {
      events_.modifiers |= modifier;
      translator_.Input(events_, &sender_);
      translator_.AutoRepeat();
      translator_.AutoRepeat();
    }

    void ReleaseModifier(int modifier) {
      events_.modifiers &= ~modifier;
      translator_.Input(events_, &sender_);
      translator_.AutoRepeat();
      translator_.AutoRepeat();
    }

    FakeClock& clock() { return clock_; }

  private:
    Events events_;
    FakeClock clock_;
    FakeSender sender_;
    ViTranslator translator_;
};

void test_commands() {
  Tester t;
  puts("> 0");
  t.Emit(KEY_0);
  puts("> i");
  t.Emit(KEY_I);
  t.Emit(KEY_ESC);
  puts("> a");
  t.Emit(KEY_A);
  t.Emit(KEY_ESC);
  puts("> o");
  t.Emit(KEY_O);
  t.Emit(KEY_ESC);
  puts("> enter");
  t.Emit(KEY_ENTER);
  t.Emit(KEY_ESC);
  puts("> shift-esc");
  t.PressModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_ESC);
  t.ReleaseModifier(MODIFIERKEY_SHIFT);
  puts("> i shift-esc");
  t.Emit(KEY_I);
  t.PressModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_ESC);
  t.ReleaseModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_ESC);

  puts("> 3s");
  t.Emit(KEY_3);
  t.Emit(KEY_S);
  t.Emit(KEY_ESC);

  puts("> 2c3h");
  t.Emit(KEY_2);
  t.Emit(KEY_C);
  t.Emit(KEY_3);
  t.Emit(KEY_H);
  t.Emit(KEY_ESC);

  puts("> 2dd");
  t.Emit(KEY_2);
  t.Emit(KEY_D);
  t.Emit(KEY_D);

  puts("> dk");
  t.Emit(KEY_D);
  t.Emit(KEY_K);

  puts("> d0");
  t.Emit(KEY_D);
  t.Emit(KEY_0);

  puts("> Gygg");
  t.PressModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_G);
  t.ReleaseModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_Y);
  t.Emit(KEY_G);
  t.Emit(KEY_G);

  puts("> 3yy");
  t.Emit(KEY_3);
  t.Emit(KEY_Y);
  t.Emit(KEY_Y);

  puts("> 3y2k");
  t.Emit(KEY_3);
  t.Emit(KEY_Y);
  t.Emit(KEY_2);
  t.Emit(KEY_K);

  puts("> 2p");
  t.Emit(KEY_2);
  t.Emit(KEY_P);

  puts("> 2P");
  t.Emit(KEY_2);
  t.PressModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_P);
  t.ReleaseModifier(MODIFIERKEY_SHIFT);

  puts("> 3y2 esc");
  t.Emit(KEY_3);
  t.Emit(KEY_Y);
  t.Emit(KEY_2);
  t.Emit(KEY_ESC);

  puts("> C");
  t.PressModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_C);
  t.ReleaseModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_ESC);

  puts("> spc bks");
  t.Emit(KEY_SPACE);
  t.Emit(KEY_BACKSPACE);

  puts("> 2r@");
  t.Emit(KEY_2);
  t.Emit(KEY_R);
  t.PressModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_2);
  t.ReleaseModifier(MODIFIERKEY_SHIFT);

  puts("> J");
  t.PressModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_J);
  t.ReleaseModifier(MODIFIERKEY_SHIFT);

  puts("> 2J");
  t.Emit(KEY_2);
  t.PressModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_J);
  t.ReleaseModifier(MODIFIERKEY_SHIFT);

  puts("> 3J");
  t.Emit(KEY_3);
  t.PressModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_J);
  t.ReleaseModifier(MODIFIERKEY_SHIFT);

  puts("> /abc");
  t.Emit(KEY_SLASH);
  t.Emit(KEY_A);
  t.Emit(KEY_B);
  t.Emit(KEY_C);
  t.Emit(KEY_ESC);

  puts("> faFb");
  t.Emit(KEY_F);
  t.Emit(KEY_A);
  t.PressModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_F);
  t.ReleaseModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_B);

  puts("> taTb");
  t.Emit(KEY_T);
  t.Emit(KEY_A);
  t.PressModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_T);
  t.ReleaseModifier(MODIFIERKEY_SHIFT);
  t.Emit(KEY_B);
}

void test_repetition() {
  Tester t;
  t.clock().set_time(1000);

  puts("> j*");
  t.Press(KEY_J);
  t.clock().set_time(1001);
  t.Press(KEY_J);
  t.clock().set_time(1002);
  t.Press(KEY_J);
  t.clock().set_time(2000);
  t.Press(KEY_J);
  t.clock().set_time(2001);
  t.Press(KEY_J);
  t.clock().set_time(2100);
  t.Press(KEY_J);
  t.Release(KEY_J);

  puts("> j* time wrapped");
  t.clock().set_time(-10);
  t.Press(KEY_J);
  t.clock().set_time(10);
  t.Press(KEY_J);
  t.clock().set_time(1000);
  t.Press(KEY_J);

  puts("> j*k");
  t.clock().set_time(-10);
  t.Press(KEY_J);
  t.clock().set_time(10000);
  t.Press(KEY_J);
  t.clock().set_time(20000);
  t.Press(KEY_K);
  t.clock().set_time(20010);
  t.Press(KEY_K);
}

int main() {
  test_commands();
  test_repetition();
  puts("PASSED");
  return 0;
}
