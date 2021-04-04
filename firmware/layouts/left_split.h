constexpr int R = 6, C = 7;
const int row_pins[R] = { 1, 0, 2, 7, 9, 22 };
const int col_pins[C] = { 3, 8, 19, 23, 18, 20, 21 };
Layer<R,C> layers[] = {
  { "qwerty", l0, l1, "",
    {
      {  fn, prt,  f1,  f2,  f3,  f4,  f5 },
      {'\\', '`', '1', '2', '3', '4', '5' },
      { tab,'\'', 'q', 'w', 'e', 'r', 't' },
      { esc,   0, 'a', 's', 'd', 'f', 'g' },
      { lsh,   0, 'z', 'x', 'c', 'v', 'b' },
      { cap, lgu, lct,   0, lal, bks,   0 },
    }
  },
  { "fn-media", l1, l1, "",
    {
      {  fn,  l0,  l1,  l2,  l3,  l4,  l5 },
      {   0,   0,   0,   0,   0,   0,   0 },
      {   0,   0,   0, mut, vdn, vup,   0 },
      {   0,   0, ply, pau, rew, ffw, rec },
      {   0,   0,   0, stp, ptk, ntk, ejt },
      {   0,   0,   0,   0,   0,   0,   0 },
    }
  },
  { "dvorak", l2, l1, "",
    {
      {  fn, prt,  f1,  f2,  f3,  f4,  f5 },
      {'\\', '`', '1', '2', '3', '4', '5' },
      { tab, '/','\'', ',', '.', 'p', 'y' },
      { esc,   0, 'a', 'o', 'e', 'u', 'i' },
      { lsh,   0, ';', 'q', 'j', 'k', 'x' },
      { cap, lgu, lct,   0, lal, bks,   0 },
    }
  },
};
