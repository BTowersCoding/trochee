// The layout has 8 rows and 8 columns.
constexpr int R = 8, C = 8;

// The row pins, to be modified according to the exact wiring.
const int row_pins[R] = { 20, 19, 18, 17, 16, 15, 14, 13 };

// The column pins, to be modified according to the exact wiring.
const int col_pins[C] = { 7, 6, 5, 4, 3, 2, 1, 0};

// Layers of this layout. The keyboard always loads layer 0 to start.
// See key_map.h for definitions of keys, e.g. lsh <=> left shift.
Layer<R,C> layers[] = {
  { "gengyun",  // name of this layer
    l0,  // this is layer 0
    l2,  // Fn layer is layer 2
    "",  // use no translator
    {
      {   esc,  '1',  '2',  '3',  '/',  '\\',  fn,  del },
      { '`', '4', '5', '6', '[', ']', '\'', bks },
      { tab,'7', '8', '9', '0', '-', '=', ';' },
      { 'q', 'j', 'l', 'm', 'f', 'y', 'p', bks },
      {   'z', '.', 'o', 'r', 's', 'u', 'c', 'b' },
      { 'x', 'a', 'e', 'h', 't', 'd', 'g', 'k' },
      { lsh, ',', 'i', 'n', 'w', 'v', up, rsh },
      { ctl, alt, spc, spc, ent, lt, dn, rt }
    }
  },
  { "gengyun-vi",  // name of this layer
    l1,  // this is layer 1
    l2,  // Fn layer is layer 2
    "vi",  // use vi translator
    {
      {   esc,  '1',  '2',  '3',  '/',  '\\',  fn,  del },
      { tab, '4', '5', '6', '[', ']', '\'', bks },
      { '`','7', '8', '9', '0', '-', '=', ';' },
      { 'q', 'j', 'l', 'm', 'f', 'y', 'p', ent },
      {   'z', '.', 'o', 'r', 's', 'u', 'c', 'b' },
      { 'x', 'a', 'e', 'h', 't', 'd', 'g', 'k' },
      { lsh, ',', 'l', 'n', 'w', 'v', up, rsh },
      { ctl, lgu, alt, spc, spc, lt, dn, rt }
    }
  },
  { "gengyun-fn",
    l2,  // this is layer 2
    l2,  // Fn layer is itself
    "",  // use no translator
    {
      {   esc,  '1',  '2',  '3',  '/',  '\\',  fn,  del },
      { tab, '4', '5', '6', '[', ']', '\'', bks },
      { '`','7', '8', '9', '0', '-', '=', ';' },
      { 'q', 'j', 'l', 'm', 'f', 'y', 'p', ent },
      {   'z', '.', 'o', 'r', 's', 'u', 'c', 'b' },
      { 'x', 'a', 'e', 'h', 't', 'd', 'g', 'k' },
      { lsh, ',', 'l', 'n', 'w', 'v', up, rsh },
      { ctl, lgu, alt, spc, spc, lt, dn, rt }
    }
  },
  { "soho",  // name of this layer
    l3,  // this is layer 3
    l5,  // Fn layer is layer 5
    "",  // use no translator
    {
      {   esc,  '1',  '2',  '3',  '/',  '\\',  fn,  del },
      { tab, '4', '5', '6', '[', ']', '\'', bks },
      { '`','7', '8', '9', '0', '-', '=', ';' },
      { 'q', 'j', 'l', 'm', 'f', 'y', 'p', ent },
      {   'z', '.', 'o', 'r', 's', 'u', 'c', 'b' },
      { 'x', 'a', 'e', 'h', 't', 'd', 'g', 'k' },
      { lsh, ',', 'l', 'n', 'w', 'v', up, rsh },
      { ctl, lgu, alt, spc, spc, lt, dn, rt }
    }
  },
  { "soho-vi",  // name of this layer
    l4,  // this is layer 4
    l5,  // Fn layer is layer 5
    "vi",  // use vi translator
    {
      {   esc,  '1',  '2',  '3',  '/',  '\\',  fn,  del },
      { tab, '4', '5', '6', '[', ']', '\'', bks },
      { '`','7', '8', '9', '0', '-', '=', ';' },
      { 'q', 'j', 'l', 'm', 'f', 'y', 'p', ent },
      {   'z', '.', 'o', 'r', 's', 'u', 'c', 'b' },
      { 'x', 'a', 'e', 'h', 't', 'd', 'g', 'k' },
      { lsh, ',', 'l', 'n', 'w', 'v', up, rsh },
      { ctl, lgu, alt, spc, spc, lt, dn, rt }
    }
  },
  { "soho-fn",
    l5,  // this is layer 5
    l5,  // Fn layer is itself
    "",  // use no translator
    {
      {   esc,  '1',  '2',  '3',  '/',  '\\',  fn,  del },
      { tab, '4', '5', '6', '[', ']', '\'', bks },
      { '`','7', '8', '9', '0', '-', '=', ';' },
      { 'q', 'j', 'l', 'm', 'f', 'y', 'p', ent },
      {   'z', '.', 'o', 'r', 's', 'u', 'c', 'b' },
      { 'x', 'a', 'e', 'h', 't', 'd', 'g', 'k' },
      { lsh, ',', 'l', 'n', 'w', 'v', up, rsh },
      { ctl, lgu, alt, spc, spc, lt, dn, rt }
    }
  },
};
