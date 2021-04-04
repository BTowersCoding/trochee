#ifndef FAKE_SCANNER_H
#define FAKE_SCANNER_H

template <int R, int C>
class FakeScanner : public Scanner<R,C> {
  public:
    FakeScanner(int num_entries, const Entry* entries)
      : num_entries_(num_entries), entries_(entries) {}

    Matrix<R,C> Scan() override {
      if (step_ < num_entries_) {
        if (entries_[step_].pressed) {
          matrix_.Press(entries_[step_].row, entries_[step_].col);
        } else {
          matrix_.Release(entries_[step_].row, entries_[step_].col);
        }
        ++step_;
      }
      return matrix_;
    }

  private:
    const int num_entries_;
    const Entry* entries_;

    int step_ = 0;
    Matrix<R,C> matrix_;
};

#endif
