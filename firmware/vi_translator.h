#ifndef VI_TRANSLATOR_H
#define VI_TRANSLATOR_H

// For Firefox and Chrome

#include <limits.h>
#include <stdio.h>
#include "clock.h"
#include "key_map.h"
#include "translator.h"
#include "vi_command.h"

class ViTranslator : public Translator {
  public:
    ViTranslator(Clock* clock) : clock_(clock) {}

    const char* name() const { return "vi"; }

    void Input(Events events, Sender* sender) override {
      // TODO: Send button events directly.
      sender_ = sender;

      // If in insert mode, send events directly unless Esc is there.
      if (insert_mode_ && !IsEscaped(events)) {
        sender->Send(events);
        return;
      }
      bool shifted = IsShifted(events);
      unsigned now = clock_->Milliseconds();
      for (int i = 0; i < kMaxEvents; ++i) {
        // Skip if key is not pressed.
        auto& event = events.keys[i];
        if (event.key == 0) continue;

        auto& old_event = in_events_.keys[i];
        if (old_event.key == 0) {
          event.repeated = false;
          event.send_time = now;
          SendKey(event.key, shifted);
        } else {
          // Keep the old state and let AutoRepeat do its job.
          event.send_time = old_event.send_time;
          event.repeated = old_event.repeated;
        }
      }
      if (insert_mode_) in_events_.Reset();
      else in_events_ = events;
    }

    void AutoRepeat() override {
      if (insert_mode_) return;

      bool shifted = IsShifted(in_events_);
      unsigned now = clock_->Milliseconds();
      for (int i = 0; i < kMaxEvents; ++i) {
        auto& event = in_events_.keys[i];
        if (event.key == 0) continue;

        unsigned wait_until = event.send_time + (event.repeated ? 30 : 500);
        if (now >= wait_until || now - wait_until < INT_MAX) {
          event.repeated = true;
          event.send_time = now;
          SendKey(event.key, shifted);
        }
      }
    }

  private:
    bool IsEscaped(const Events& events) const {
      if (events.modifiers) return false;
      for (int i = 0; i < kMaxEvents; ++i)
        if (events.keys[i].key == KEY_ESC) return true;
      return false;
    }

    bool IsShifted(const Events& events) const {
      auto modifiers = events.modifiers & ~0xE000;
      return (modifiers & MODIFIERKEY_SHIFT) ||
        (modifiers & MODIFIERKEY_LEFT_SHIFT) ||
        (modifiers & MODIFIERKEY_RIGHT_SHIFT);
    }

    bool IsControlled(const Events& events) const {
      auto modifiers = events.modifiers & ~0xE000;
      return (modifiers & MODIFIERKEY_CTRL) ||
        (modifiers & MODIFIERKEY_LEFT_CTRL) ||
        (modifiers & MODIFIERKEY_RIGHT_CTRL);
    }

    bool IsAlted(const Events& events) const {
      auto modifiers = events.modifiers & ~0xE000;
      return (modifiers & MODIFIERKEY_ALT) ||
        (modifiers & MODIFIERKEY_LEFT_ALT) ||
        (modifiers & MODIFIERKEY_RIGHT_ALT);
    }

    void SendKey(int key_code, bool shifted) {
      // Shift-Escape sends a real escape, e.g. for closing a dialog.
      if (key_code == KEY_ESC && shifted) {
        Escape();
        return;
      }
      // Escape resets the command.
      if (key_code == KEY_ESC) {
        command_.Reset();
        insert_mode_ = false;
        return;
      }

      // Read recognized keys.
      auto key = ConvertKeyCode(key_code, shifted);
      if (key == 0) return;

      // Form the command sequence and execute it if complete.
      command_.Add(key);
      if (command_.completed) {
        Execute(command_);
      }
    }

    int ConvertKeyCode(int key_code, bool shifted) {
      switch (key_code) {
        case KEY_SPACE: return ' ';
        case KEY_BACKSPACE: return '\b';
        case KEY_ENTER: return '\n';
        case KEY_QUOTE: return shifted ? '"' : '\'';
        case KEY_COMMA: return shifted ? '<' : ',';
        case KEY_MINUS: return shifted ? '_' : '-';
        case KEY_PERIOD: return shifted ? '>' : '.';
        case KEY_SLASH: return shifted ? '?' : '/';
        case KEY_0: return shifted ? ')' : '0';
        case KEY_1: return shifted ? '!' : '1';
        case KEY_2: return shifted ? '@' : '2';
        case KEY_3: return shifted ? '#' : '3';
        case KEY_4: return shifted ? '$' : '4';
        case KEY_5: return shifted ? '%' : '5';
        case KEY_6: return shifted ? '^' : '6';
        case KEY_7: return shifted ? '&' : '7';
        case KEY_8: return shifted ? '*' : '8';
        case KEY_9: return shifted ? '(' : '9';
        case KEY_SEMICOLON: return shifted ? ':' : ';';
        case KEY_EQUAL: return shifted ? '+' : '=';
        case KEY_A: return shifted ? 'A' : 'a';
        case KEY_B: return shifted ? 'B' : 'b';
        case KEY_C: return shifted ? 'C' : 'c';
        case KEY_D: return shifted ? 'D' : 'd';
        case KEY_E: return shifted ? 'E' : 'e';
        case KEY_F: return shifted ? 'F' : 'f';
        case KEY_G: return shifted ? 'G' : 'g';
        case KEY_H: return shifted ? 'H' : 'h';
        case KEY_I: return shifted ? 'I' : 'i';
        case KEY_J: return shifted ? 'J' : 'j';
        case KEY_K: return shifted ? 'K' : 'k';
        case KEY_L: return shifted ? 'L' : 'l';
        case KEY_M: return shifted ? 'M' : 'm';
        case KEY_N: return shifted ? 'N' : 'n';
        case KEY_O: return shifted ? 'O' : 'o';
        case KEY_P: return shifted ? 'P' : 'p';
        case KEY_Q: return shifted ? 'Q' : 'q';
        case KEY_R: return shifted ? 'R' : 'r';
        case KEY_S: return shifted ? 'S' : 's';
        case KEY_T: return shifted ? 'T' : 't';
        case KEY_U: return shifted ? 'U' : 'u';
        case KEY_V: return shifted ? 'V' : 'v';
        case KEY_W: return shifted ? 'W' : 'w';
        case KEY_X: return shifted ? 'X' : 'x';
        case KEY_Y: return shifted ? 'Y' : 'y';
        case KEY_Z: return shifted ? 'Z' : 'z';
        case KEY_LEFT_BRACE: return shifted ? '{' : '[';
        case KEY_BACKSLASH: return shifted ? '|' : '\\';
        case KEY_RIGHT_BRACE: return shifted ? '}' : ']';
        case KEY_TILDE: return shifted ? '~' : '`';
        default: return 0;  // to be ignored
      }
    }

    bool Execute(const ViCommand& command) {
      switch (command.action) {
        case 0: Move(command.motion); break;
        case 'i': Insert(); break;
        case 'I': StartOfLine(); Insert(); break;
        case 'a': Append(); break;
        case 'A': EndOfLine(); Insert(); break;
        case 'J': JoinLines(command.count); break;
        case 'r': ReplaceByCharacter(command.count, command.character); break;
        case 's': Substitute(command.count); break;
        case 'S': Change(command.count, true, command.motion); break;
        case 'o': OpenLine(); break;
        case 'O': OpenLineAbove(); break;
        case 'u': Undo(command.count); break;
        case 'U': Redo(command.count); break;  // different from vi
        case 'p': PasteAfter(command.count); break;
        case 'P': PasteBefore(command.count); break;
        case 'x': Delete(1, false, ViMotion(command.count, 'l')); break;
        case 'X': Delete(1, false, ViMotion(command.count, 'h')); break;
        case 'c': Change(command.count, command.doubled, command.motion); break;
        case 'C': ChangeToEndOfLine(); break;
        case 'd': Delete(command.count, command.doubled, command.motion); break;
        case 'D': DeleteToEndOfLine(); break;
        case 'y': Yank(command.count, command.doubled, command.motion); break;
        case 'Y': Yank(command.count, true, command.motion); break;
        case '/': Find(); Insert(); break;
        case '\n': Insert(); NewLine(); break;
        default: return false;
      }
      return true;
    }

    bool Move(const ViMotion& motion) {
      for (int i = 0; i < motion.count; ++i) {
        switch (motion.move) {
          case 'h': Left(); break;
          case 'j': Down(); break;
          case 'k': Up(); break;
          case 'l': Right(); break;
          case 'b': PrevStartOfWord(); break;
          case 'w': NextStartOfWord(); break;
          case 'e': NextEndOfWord(); break;
          case '0': StartOfLine(); break;
          case '$': EndOfLine(); break;
          case '{': PrevParagraph(); break;
          case '}': NextParagraph(); break;
          case '\b': PageUp(); break;
          case ' ': PageDown(); break;
          case 'f': FindCharacter(motion.go, true); break;
          case 'F': FindCharacter(motion.go, false); break;
          case 't': TillCharacter(motion.go, true); break;
          case 'T': TillCharacter(motion.go, false); break;
          case 'g':
            switch (motion.go) {
              case 'g': StartOfDoc(); break;
              case 't': NextTab(); break;
              case 'T': PrevTab(); break;
              default: return false;
            }
            break;
          case 'G': EndOfDoc(); break;
          case '[': PrevPage(); break;
          case ']': NextPage(); break;
          default: return false;
        }
      }
      return true;
    }

    void Insert() {
      insert_mode_ = true;
    }

    void Append() {
      Right();
      Insert();
    }

    void JoinLines(int count) {
      count -= (count > 1);
      for (int i = 0; i < count; ++i) {
        EndOfLine();
        Delete();
        Emit(KEY_SPACE);
      }
    }

    void ReplaceByCharacter(int count, char character) {
      Select(1, ViMotion{count,'l'});
      for (int i = 0; i < count; ++i) Print(character);
    }

    void Substitute(int count) {
      Select(1, ViMotion{count,'l'});
      Copy();
      Insert();
    }

    void OpenLine() {
      EndOfLine();
      NewLine();
      Insert();
    }

    void OpenLineAbove() {
      StartOfLine();
      NewLine();
      Up();
      Insert();
    }

    void PasteAfter(int count) {
      if (copy_by_line_) EndOfLine();
      else Right();
      for (int i = 0; i < count; ++i) {
        if (copy_by_line_) NewLine();
        Paste();
      }
    }

    void PasteBefore(int count) {
      for (int i = 0; i < count; ++i) {
        if (copy_by_line_) {
          StartOfLine();
          NewLine();
          Up();
        }
        Paste();
      }
    }

    void Undo(int count) {
      for (int i = 0; i < count; ++i) Undo();
    }

    void Redo(int count) {
      for (int i = 0; i < count; ++i) Redo();
    }

    void Change(int count, bool doubled, const ViMotion& motion) {
      if (doubled) SelectLines(count);
      else Select(count, motion);
      Copy();
      Insert();
    }

    void ChangeToEndOfLine() {
      Select(1, ViMotion(1, '$'));
      Copy();
      Insert();
    }

    void Delete(int count, bool doubled, const ViMotion& motion) {
      if (doubled) SelectLines(count);
      else Select(count, motion);
      Copy();
      Emit(KEY_DELETE);
      if (copy_by_line_) Emit(KEY_DELETE);
    }

    void DeleteToEndOfLine() {
      Select(1, ViMotion(1, '$'));
      Copy();
      Emit(KEY_DELETE);
    }

    void Yank(int count, bool doubled, const ViMotion& motion) {
      if (doubled) SelectLines(count);
      else Select(count, motion);
      Copy();
      if (doubled || motion.IsForward()) Right();
      else Left();
    }

    void SelectLines(int count) {
      Select(count-1, ViMotion(1,'j'));
    }

    void Select(int count, const ViMotion& motion) {
      copy_by_line_ = motion.IsByLine();
      if (motion.IsByLine()) {
        if (motion.IsForward()) StartOfLine();
        else EndOfLine();
      }
      PressModifier(MODIFIERKEY_SHIFT);
      if (motion.IsByLine()) {
        if (motion.IsForward()) EndOfLine();
        else StartOfLine();
      }
      for (int i = 0; i < count; ++i) {
        Move(motion);
      }
      if (motion.IsByLine()) {
        if (motion.IsForward()) EndOfLine();
        else StartOfLine();
      }
      ReleaseModifier(MODIFIERKEY_SHIFT);
    }

    void FindCharacter(char character, bool forward) {
      Find();
      Print(character);
      if (forward) Emit(KEY_ENTER);
      else Emit(MODIFIERKEY_SHIFT, KEY_ENTER);
      Emit(KEY_ESC);
      Right();
      Left();
    }

    void TillCharacter(char character, bool forward) {
      Find();
      Print(character);
      if (forward) Emit(KEY_ENTER);
      else Emit(MODIFIERKEY_SHIFT, KEY_ENTER);
      Emit(KEY_ESC);
      if (forward) Left();
      else Right();
    }

    // Cursor-moving
    void Left() { Emit(KEY_LEFT); }
    void Right() { Emit(KEY_RIGHT); }
    void Up() { Emit(KEY_UP); }
    void Down() { Emit(KEY_DOWN); }
    void Delete() { Emit(KEY_DELETE); }
    void StartOfLine() { Emit(KEY_HOME); }
    void EndOfLine() { Emit(KEY_END); }
    void PageUp() { Emit(KEY_PAGE_UP); }
    void PageDown() { Emit(KEY_PAGE_DOWN); }
    // TODO: word movements are browser-dependent
    void PrevStartOfWord() { Emit(MODIFIERKEY_CTRL, KEY_LEFT); }
    void NextStartOfWord() { Emit(MODIFIERKEY_CTRL, KEY_RIGHT); }
    void NextEndOfWord() { Emit(MODIFIERKEY_CTRL, KEY_RIGHT); }
    void StartOfDoc() { Emit(MODIFIERKEY_CTRL, KEY_HOME); }
    void EndOfDoc() { Emit(MODIFIERKEY_CTRL, KEY_END); }

    // Special
    void NewLine() { Emit(KEY_ENTER); }
    void Escape() { Emit(KEY_ESC); }
    void Find() { Emit(MODIFIERKEY_CTRL, KEY_F); }
    void Copy() { Emit(MODIFIERKEY_CTRL, KEY_C); }
    void Paste() { Emit(MODIFIERKEY_CTRL, KEY_V); }
    void Undo() { Emit(MODIFIERKEY_CTRL, KEY_Z); }
    void Redo() { Emit(MODIFIERKEY_CTRL, KEY_Y); }
    void PrevParagraph() { Emit(MODIFIERKEY_CTRL, KEY_UP); }
    void NextParagraph() { Emit(MODIFIERKEY_CTRL, KEY_DOWN); }
    void PrevPage() { Emit(MODIFIERKEY_ALT, KEY_LEFT); }
    void NextPage() { Emit(MODIFIERKEY_ALT, KEY_RIGHT); }
    void PrevTab() { Emit(MODIFIERKEY_CTRL, KEY_PAGE_UP); }
    void NextTab() { Emit(MODIFIERKEY_CTRL, KEY_PAGE_DOWN); }

    // Send keys
    void Emit(int key) {
      // Only one key at a time.
      out_events_.keys[0].key = key;
      sender_->Delay(kDelayMs);
      sender_->Send(out_events_);
      out_events_.keys[0].key = 0;
      sender_->Delay(kDelayMs);
      sender_->Send(out_events_);
    }
    void Emit(int modifier, int key) {
      PressModifier(modifier);
      Emit(key);
      ReleaseModifier(modifier);
    }
    void PressModifier(int modifier) {
      out_events_.modifiers |= modifier;
      sender_->Delay(kDelayMs);
      sender_->Send(out_events_);
    }
    void ReleaseModifier(int modifier) {
      out_events_.modifiers &= ~modifier;
      sender_->Delay(kDelayMs);
      sender_->Send(out_events_);
    }
    void Print(char character) {
      char message[2] = {character, 0};
      sender_->Delay(kDelayMs);
      sender_->Send(message);
    }

    static const int kDelayMs = 10;
    ViCommand command_;
    bool copy_by_line_ = false;

    Clock* const clock_;
    Sender* sender_ = nullptr;
    Events in_events_;
    Events out_events_;
    bool insert_mode_ = false;
};

#endif
