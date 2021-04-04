#include "vi_command.h"
#include <assert.h>
#include <stdio.h>

void Expect(const ViCommand& c, int count, char action, bool doubled,
            int motion_count, char move, char go, bool completed, int line) {
  printf("line %d: expecting %d%c%c%d%c%c %d \t receiving %d%c%c%d%c%c %d\n",
         line, count, action, (doubled ? action : 0), motion_count, move, go,
         completed, c.count, c.action, (doubled ? c.action : 0), c.motion.count,
         c.motion.move, c.motion.go, c.completed);
  assert(c.count == count);
  assert(c.action == action);
  assert(c.doubled == doubled);
  assert(c.motion.count == motion_count);
  assert(c.motion.move == move);
  assert(c.completed == completed);
}

int main() {
  ViCommand c;

  for (char action : kSimpleActions) {
    if (!action) break;
    if (action == 'r') {
      c.Add("34").Add(action).Add('5');
      Expect(c, 34, action, false, 0, 0, 0, true, __LINE__);
      assert(c.character == '5');
    } else {
      c.Add(action);
      Expect(c, 1, action, false, 0, 0, 0, true, __LINE__);
      c.Add("10").Add(action);
      Expect(c, 10, action, false, 0, 0, 0, true, __LINE__);
      c.Add("567").Add(action);
      Expect(c, 56, action, false, 0, 0, 0, true, __LINE__);
    }
  }

  c.Add("0");
  Expect(c, 0, 0, false, 1, '0', 0, true, __LINE__);
  c.Add("d0");
  Expect(c, 1, 'd', false, 1, '0', 0, true, __LINE__);

  for (char move : kSimpleMotions) {
    if (!move) break;
    c.Add(move);
    Expect(c, 0, 0, false, 1, move, 0, true, __LINE__);
    c.Add("10").Add(move);
    Expect(c, 0, 0, false, 10, move, 0, true, __LINE__);
    c.Add("567").Add(move);
    Expect(c, 0, 0, false, 56, move, 0, true, __LINE__);
  }

  for (char move : kCompoundMotions) {
    if (!move) break;
    c.Add("56").Add(move).Add('z');
    Expect(c, 0, 0, false, 56, move, 'z', true, __LINE__);
  }

  for (char action : kCompoundActions) {
    if (!action) break;

    c.Add(action).Add(action);
    Expect(c, 1, action, true, 0, 0, 0, true, __LINE__);

    c.Add("20").Add(action).Add(action);
    Expect(c, 20, action, true, 0, 0, 0, true, __LINE__);

    for (char move : kSimpleMotions) {
      if (!move) break;
      c.Add(action).Add(move);
      Expect(c, 1, action, false, 1, move, 0, true, __LINE__);
      c.Add("5").Add(action).Add(move);
      Expect(c, 5, action, false, 1, move, 0, true, __LINE__);
      c.Add(action).Add("50").Add(move);
      Expect(c, 1, action, false, 50, move, 0, true, __LINE__);
      c.Add("987").Add(action).Add("71").Add(move);
      Expect(c, 98, action, false, 71, move, 0, true, __LINE__);
    }
    for (char move : kCompoundMotions) {
      if (!move) break;
      c.Add("987").Add(action).Add("71").Add(move).Add('0');
      Expect(c, 98, action, false, 71, move, '0', true, __LINE__);
    }
  }

  c.Add("z");
  Expect(c, 0, 0, false, 0, 0, 0, false, __LINE__);
  c.Add("5q");
  Expect(c, 0, 0, false, 0, 0, 0, false, __LINE__);
  c.Add("c2r");
  Expect(c, 0, 0, false, 0, 0, 0, false, __LINE__);
  c.Add("3yx");
  Expect(c, 0, 0, false, 0, 0, 0, false, __LINE__);

  puts("PASSED");
  return 0;
}
