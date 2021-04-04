#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdio.h>
#include <atomic>

#include "layout.h"
#include "matrix.h"
#include "scanner.h"
#include "sender.h"
#include "translator.h"

template <int R, int C>
class Controller {
  public:
    Controller(Layout<R,C>* layout, Scanner<R,C>* scanner, Sender* sender)
      : layout_(layout), scanner_(scanner), sender_(sender) {}

    void Scan() {
      Matrix<R, C> new_matrix = scanner_->Scan();
      Entry entries[kMaxEvents];
      int num_entries = matrix_.Difference(new_matrix, kMaxEvents, entries);
      auto translator = layout_->translator();
      if (num_entries > 0) {
        Events events = layout_->Interpret(num_entries, entries);
        do {
          if (translator) {
            translator->Input(events, sender_);
          } else {
            sender_->Send(events);
          }
        } while (events.ClearTapping());
      } else {
        if (translator) {
          translator->AutoRepeat();
        }
      }
    }

  private:
    Layout<R,C>* const layout_;
    Scanner<R,C>* const scanner_;
    Sender* const sender_;

    Matrix<R,C> matrix_;
};

#endif
