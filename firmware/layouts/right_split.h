constexpr int R = 6, C = 9;
const int row_pins[R] = { 21, 22, 14, 20, 19, 8 };
const int col_pins[C] = { 9, 7, 23, 18, 17, 15, 10, 11, 12 };
Layer<R,C> layers[] = {
  { "qwerty", l0, l1, "",
    {
      {  f6,  f7,  f8,  f9, f10, f11, f12, bks,  fn },
      { '6', '7', '8', '9', '0', '-', '=', mlc, mrc },
      { 'y', 'u', 'i', 'o', 'p', '[', ']',   0, pgu },
      { 'h', 'j', 'k', 'l', ';', ent,   0, del, pgd },
      { 'n', 'm', ',', '.', '/', rsh,   0, hom, end },
      { spc,   0, ral, rct,   0,  lt,  dn,  up,  rt },
    }
  },
  { "fn-motion", l1, l1, "",
    {
      {  l7,  l6,  l5,  l4,  l3,  l2,  l1,  l0,  fn },
      {   0,   0,   0,   0,   0,   0,   0,   0,   0 },
      { mdc, mlc, mmc, mrc, del,   0,   0,   0,   0 },
      {  lt,  dn,  up,  rt, bks, ent,   0, del,   0 },
      { hom, pgd, pgu, end,   0, rsh,   0,   0,   0 },
      { spc,   0, ral, rct,   0,   0,   0,   0,   0 },
    }
  },
  { "dvorak", l2, l1, "",
    {
      {  f6,  f7,  f8,  f9, f10, f11, f12, bks,  fn },
      { '6', '7', '8', '9', '0', '-', '=', mlc, mrc },
      { 'f', 'g', 'c', 'r', 'l', '[', ']',   0, pgu },
      { 'd', 'h', 't', 'n', 's', ent,   0, del, pgd },
      { 'b', 'm', 'w', 'v', 'z', rsh,   0, hom, end },
      { spc,   0, ral, rct,   0,  lt,  dn,  up,  rt },
    }
  },
};
