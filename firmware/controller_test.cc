#include "controller.h"

#include <assert.h>
#include "fake_clock.h"
#include "fake_scanner.h"
#include "fake_sender.h"
#include "vi_translator.h"

void test_with_fn() {
  constexpr int R = 2, C = 2;

  Layer<R,C> layers[] = {
    { "qwerty", l0, l1, "",
      {
        {'q','w'},
        {shf,fn}
      }
    },
    { "fn1", l1, l1, "",
      {
        {l0,esc},
        {l2,fn}
      }
    },
    { "dvorak", l2, l3, "",
      {
        {'d','v'},
        {'o',fn}
      }
    },
    { "fn2", l3, l3, "",
      {
        {l0,ent},
        {l2,fn}
      }
    }
  };

  Layout<R,C> layout(4, layers, 0, nullptr);

  Entry entries[] = {
    // l1
    {1,1, true},
    {0,0, true},
    {0,0, false},
    {1,1, false},

    {0,0, true},
    {0,1, true},
    {0,1, false},
    {1,0, true},
    {0,0, false},
    {1,0, false},

    {1,1, true},
    {0,1, true},
    {1,1, false},
    {0,1, false},

    // l2
    {1,1, true},
    {1,0, true},
    {1,0, false},
    {1,1, false},

    {1,1, true},
    {0,1, true},
    {1,1, false},
    {0,1, false},

    {0,0, true},
    {0,1, true},
    {0,1, false},
    {1,0, true},
    {0,0, false},
    {1,0, false},

    // switch back to l1
    {0,0, true},
    {1,1, true},
    {0,0, false},
    {1,1, false},
  };

  int num_entries = sizeof(entries)/sizeof(entries[0]);
  FakeScanner<R,C> scanner(num_entries, entries);
  FakeSender sender;

  Controller<R,C> controller(&layout, &scanner, &sender);
  for (int i = 0; i < num_entries * 2; ++i) {
    controller.Scan();
  }

  auto events = sender.events();
  assert(events.modifiers == 0);
  for (int i = 0; i < kMaxEvents; ++i) {
    assert(events.keys[i].key == 0);
  }
}

void test_with_fnl() {
  constexpr int R = 2, C = 2;

  Layer<R,C> layers[] = {
    { "qwerty", l0, l1, "",
      {
        {'q','w'},
        {shf,fnl}
      }
    },
    { "fn1", l1, l1, "",
      {
        {l0,esc},
        {l2,fnl}
      }
    },
    { "dvorak", l2, l3, "",
      {
        {'d','v'},
        {'o',fnl}
      }
    },
    { "fn2", l3, l3, "",
      {
        {l0,ent},
        {l2,fnl}
      }
    }
  };

  Layout<R,C> layout(4, layers, 0, nullptr);

  Entry entries[] = {
    // l1
    {1,1, true},
    {0,0, true},
    {0,0, false},
    {1,1, false},

    {0,0, true},
    {0,1, true},
    {0,1, false},
    {1,0, true},
    {0,0, false},
    {1,0, false},

    {1,1, true},
    {0,1, true},
    {1,1, false},
    {0,1, false},

    // l2
    {1,0, true},
    {1,0, false},

    {1,1, true},
    {0,1, true},
    {1,1, false},
    {0,1, false},

    {1,1, true},
    {1,1, false},

    {0,0, true},
    {0,1, true},
    {0,1, false},
    {1,0, true},
    {0,0, false},
    {1,0, false},

    // switch back to l1
    {1,1, true},
    {1,1, false},
    {0,0, true},
    {0,0, false},
  };

  int num_entries = sizeof(entries)/sizeof(entries[0]);
  FakeScanner<R,C> scanner(num_entries, entries);
  FakeSender sender;

  Controller<R,C> controller(&layout, &scanner, &sender);
  for (int i = 0; i < num_entries * 2; ++i) {
    controller.Scan();
  }

  auto events = sender.events();
  assert(events.modifiers == 0);
  for (int i = 0; i < kMaxEvents; ++i) {
    assert(events.keys[i].key == 0);
  }
}

void test_with_vi() {
  constexpr int R = 2, C = 3;

  Layer<R,C> layers[] = {
    { "qwerty", l0, l1, "",
      {
        {'1',fnl, esc},
        {'c','w', ctl}
      }
    },
    { "vi-emu", l1, l1, "vi",
      {
        {'2',fnl, esc},
        {'c','w', ctl}
      }
    }
  };

  FakeClock clock;
  ViTranslator vi_translator(&clock);
  Translator* translators[] = {&vi_translator};
  Layout<R,C> layout(2, layers, 1, translators);

  Entry entries[] = {
    // switch to l1
    {0,1, true},
    {0,1, false},
    // c
    {1,0, true},
    {1,0, false},
    // w
    {1,1, true},
    {1,1, false},
    // 2
    {0,0, true},
    {0,0, false},
    // ctl down
    {1,2, true},
    // w
    {1,1, true},
    {1,1, false},
    // ctl up
    {1,2, false},
    // esc
    {0,2, true},
    {0,2, false},
    // w
    {1,1, true},
    {1,1, false},
    // switch to l0
    {0,1, true},
    {0,1, false},
    // 1
    {0,0, true},
    {0,0, false},
    // w
    {1,1, true},
    {1,1, false},
  };

  int num_entries = sizeof(entries)/sizeof(entries[0]);
  FakeScanner<R,C> scanner(num_entries, entries);
  FakeSender sender;

  Controller<R,C> controller(&layout, &scanner, &sender);
  for (int i = 0; i < num_entries * 2; ++i) {
    clock.set_time(i*200);
    controller.Scan();
    vi_translator.AutoRepeat();
  }

  auto events = sender.events();
  assert(events.modifiers == 0);
  for (int i = 0; i < kMaxEvents; ++i) {
    assert(events.keys[i].key == 0);
  }
}

void test_with_tapping() {
  constexpr int R = 2, C = 2;

  Layer<R,C> layers[] = {
    { "qwerty", l0, l1, "",
      {
        {'q',ctl},
        {shf,fn}
      },
      { {fn, bks}, {ctl, ent} }
    },
    { "fn", l1, l1, "",
      {
        {lt,'w'},
        {shf,fn}
      },
      //{ {fn, bks}, {ctl, ent} }
    }
  };

  Layout<R,C> layout(2, layers, 0, nullptr);

  Entry entries[] = {
    // fn --> bks
    {1,1, true},
    {1,1, false},

    // ctl --> ent
    {0,1, true},
    {0,1, false},

    // shift-fn --> shift-bks
    {1,0, true},
    {1,1, true},
    {1,1, false},
    {1,0, false},

    // ctrl-fn --> ctrl-bks
    {0,1, true},
    {1,1, true},
    {1,1, false},
    {0,1, false},

    // fn-ctrl --> w
    {1,1, true},
    {0,1, true},
    {0,1, false},
    {1,1, false},

    // ctrl-shift-q --> ctrl-shift-q
    {0,1, true},
    {1,0, true},
    {0,0, true},
    {0,0, false},
    {0,1, false},
    {1,0, false},

    // q-fn --> ??
    {0,0, true},
    {1,1, true},
    {0,0, false},
    {1,1, false},

    // shift-fn-q --> shift-left
    {1,0, true},
    {1,1, true},
    {0,0, true},
    {0,0, false},
    {1,0, false},
    {1,1, false},

    // Can't handle releasing a modifier that doesn't exist in fn layer.
#if 0
    // ctrl-fn-q --> ctrl-left
    {0,1, true},
    {1,1, true},
    {0,0, true},
    {0,0, false},
    {0,1, false},
    {1,1, false},
#endif

    // fn --> bks
    {1,1, true},
    {1,1, false},
  };

  int num_entries = sizeof(entries)/sizeof(entries[0]);
  FakeScanner<R,C> scanner(num_entries, entries);
  FakeSender sender;

  Controller<R,C> controller(&layout, &scanner, &sender);
  for (int i = 0; i < num_entries * 2; ++i) {
    controller.Scan();
  }

  auto events = sender.events();
  assert(events.modifiers == 0);
  for (int i = 0; i < kMaxEvents; ++i) {
    assert(events.keys[i].key == 0);
  }
}


int main() {
  puts("test_with_fn");
  test_with_fn();
  puts("test_with_fnl");
  test_with_fnl();
  puts("test_with_vi");
  test_with_vi();
  puts("test_with_tapping");
  test_with_tapping();
  puts("PASSED");
  return 0;
}
