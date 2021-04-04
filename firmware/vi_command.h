#ifndef VI_COMMAND_H
#define VI_COMMAND_H

#include <ctype.h>
#include <string.h>

constexpr char kSimpleActions[] = "iIaAJrsSoOuUpPxXCDY/\n";
constexpr char kCompoundActions[] = "cdy";
constexpr char kSimpleMotions[] = "hjklwbe${}\b G[]";
constexpr char kCompoundMotions[] = "gfFtT";

struct ViMotion {
  ViMotion() { Reset(); }
  ViMotion(int count, char move) : count(count), move(move), go(0) {}
  void Reset() {
    count = 0;
    move = 0;
    go = 0;
  }
  bool IsForward() const { return strchr("jlwe G", move); }
  bool IsByLine() const { return strchr("jkgG", move); }

  int count = 0;
  char move = 0;
  char go = 0;
};

struct ViCommand {
  ViCommand& Add(const char* keys) {
    while (*keys) Add(*keys++);
    return *this;
  }

  ViCommand& Add(int key) {
    if (completed) Reset();

    if (action == 'r') {
      character = key;
      Complete();
    } else if (motion.move && strchr(kCompoundMotions, motion.move)) {
      motion.go = key;
      Complete();
    } else if (key == '0' && !count && !motion.count) {
      motion.move = key;
      Complete();
    } else if (isdigit(key)) {
      if (!action) {
        auto new_count = count * 10 + key - '0';
        if (new_count <= kMaxCount) count = new_count;
      } else {
        auto new_count = motion.count * 10 + key - '0';
        if (new_count <= kMaxCount) motion.count = new_count;
      }
    } else if (strchr(kSimpleActions, key)) {
      if (!action) {
        action = key;
        if (action != 'r') Complete();
      } else {
        Reset();
      }
    } else if (strchr(kCompoundActions, key)) {
      if (!action) {
        action = key;
      } else if (action == key) {
        doubled = true;
        Complete();
      } else {
        Reset();
      }
    } else if (strchr(kSimpleMotions, key) && !motion.move) {
      if (!action) {
        motion.count = count;
        count = 0;
      }
      motion.move = key;
      Complete();
    } else if (strchr(kCompoundMotions, key) && !motion.move) {
      if (!action) {
        motion.count = count;
        count = 0;
      }
      motion.move = key;
    } else
      Reset();
    return *this;
  }

  void Complete() {
    if (count == 0 && action) count = 1;
    if (motion.count == 0 && motion.move) motion.count = 1;
    completed = true;
  }

  void Reset() {
    count = 0;
    action = 0;
    doubled = false;
    completed = false;
    motion.Reset();
  }

  const int kMaxCount = 99;

  int count = 0;
  char action = 0;
  bool doubled = false;
  char character = 0;  // for 'r' command
  ViMotion motion;
  bool completed = false;
};

#endif
