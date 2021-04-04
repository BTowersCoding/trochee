constexpr int R = 6, C = 14;
const int row_pins[R] = { 7, 8, 9, 10, 11, 12 };
const int col_pins[C] = { 14, 15, 16, 17, 18, 19, 20, 0, 1, 2, 3, 4, 5, 6};
Layer<R,C> layers[] = {
  { "qwerty", l0, l1, "",
    {
      {   0,  f1,  f2,  f3,  f4,  f5,  f6,  f7,  f8,  f9, f10, f11, f12,   0 },
      { cap, '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0','\\', del },
      { tab, '=', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']' },
      { esc, '-', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', ent },
      {   0, lsh, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', rsh,   0 },
      { fnl, lgu,   0, lct,   0, lal, bks, spc, ral,   0, rct,   0, mnu, fnl },
    }
  },
  { "fn-media", l1, l1, "",
    {
      {   0,  f1,  f2,  f3,  f4,  f5,  f6,  f7,  f8,  f9, f10, f11, f12,   0 },
      { cap, '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0','\\', del },
      { tab, '=',   0, mut, vdn, vup,   0, mdc, mlc, mmc, mrc,   0, '[', ']' },
      { esc, '-', ply, pau, rew, ffw, rec,  lt,  dn,  up,  rt,   0,'\'', ent },
      {   0, lsh,   0, stp, ptk, ntk, ejt, hom, pgd, pgu, end,   0, rsh,   0 },
      { fnl, lgu,   0, lct,   0, lal, bks, spc, ral,   0, rct,   0, mnu, fnl },
    }
  },
};
