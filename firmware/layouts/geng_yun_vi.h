// The layout has 6 rows and 14 columns.
constexpr int R = 6, C = 14;

// The row pins, to be modified according to the exact wiring.
const int row_pins[R] = { 7, 8, 9, 10, 11, 12 };

// The column pins, to be modified according to the exact wiring.
const int col_pins[C] = { 14, 15, 16, 17, 18, 19, 20, 0, 1, 2, 3, 4, 5, 6};

// Layers of this layout. The keyboard always loads layer 0 to start.
// See key_map.h for definitions of keys, e.g. lsh <=> left shift.
Layer<R,C> layers[] = {
  { "qwerty",  // name of this layer
    l0,  // this is layer 0
    l2,  // Fn layer is layer 2
    "",  // use no translator
    {
      {   0,  f1,  f2,  f3,  f4,  f5,  f6,  f7,  f8,  f9, f10, f11, f12,   0 },
      { esc, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', bks },
      { tab,'\\', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']' },
      { lct, '`', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', rct },
      {   0, lsh, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', rsh,   0 },
      { ins, lgu,   0, lct,   0, lal,  fn, spc, ral,   0, rct,   0, rgu, del },
    },
    // Tapping on modifiers to get normal keys.
    { {fn,  bks}, {lct, esc}, {rct, ent} }
  },
  { "vi-emu",  // name of this layer
    l1,  // this is layer 1
    l2,  // Fn layer is layer 2
    "vi",  // use vi translator
    {
      {   0,  f1,  f2,  f3,  f4,  f5,  f6,  f7,  f8,  f9, f10, f11, f12,   0 },
      { esc, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', bks },
      { tab,'\\', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']' },
      { lct, '`', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', rct },
      {   0, lsh, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', rsh,   0 },
      { ins, lgu,   0, lct,   0, lal,  fn, spc, ral,   0, rct,   0, rgu, del },
    },
    // Tapping on modifiers to get normal keys.
    { {fn,  bks}, {lct, esc}, {rct, ent} }
  },
  { "media",
    l2,  // this is layer 2
    l2,  // Fn layer is itself
    "",  // use no translator
    {
      {   0,  f1,  f2,  f3,  f4,  f5,  f6,  f7,  f8,  f9,  l0,  l1,  l2,   0 },
      { esc, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', bks },
      { tab,'\\',   0, mut, vdn, vup,   0, mdc, mlc, mmc, mrc,   0, '[', ']' },
      { lct, '`', ply, pau, rew, ffw, rec,  lt,  dn,  up,  rt,   0,'\'', rct },
      {   0, lsh,   0, stp, ptk, ntk, ejt, hom, pgd, pgu, end,   0, rsh,   0 },
      { ins, lgu,   0, lct,   0, lal,  fn, spc, ral,   0, rct,   0, rgu, del },
    },
    // Tapping on modifiers to get normal keys.
    { {fn,  bks}, {lct, esc}, {rct, ent} }
  },
};
