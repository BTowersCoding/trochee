#ifndef MATRIX_H
#define MATRIX_H

struct Entry {
  int row;
  int col;
  bool pressed;
};

template <int R, int C>
class Matrix {
  public:
    Matrix() {
      for (int r = 0; r < R; ++r) rows_[r] = 0;
    }

    // Calculate the difference between this matrix and a new matrix.
    // Returns the number of different entries. This matrix is updated.
    int Difference(const Matrix& new_matrix, int max_entries, Entry* entries) {
      int num_entries = 0;
      for (int r = 0; r < R && num_entries < max_entries; ++r) {
        int diff = rows_[r] ^ new_matrix.rows_[r];
        if (diff == 0) continue;

        for (int c = 0; c < C && num_entries < max_entries; ++c) {
          if (diff & (1<<c)) {
            bool pressed = new_matrix.IsPressed(r, c);
            entries[num_entries].row = r;
            entries[num_entries].col = c;
            entries[num_entries].pressed = pressed;
            ++num_entries;

            // Keep up with the new matrix for the difference.
            if (pressed) Press(r, c);
            else Release(r, c);
          }
        }
      }
      return num_entries;
    }

    bool IsPressed(int row, int col) const {
      return rows_[row] & (1<<col);
    }

    void Press(int row, int col) {
      rows_[row] |= (1<<col);
    }

    void Release(int row, int col) {
      rows_[row] &= ~(1<<col);
    }

  private:
    // Bitmap indicating pressed keys in the matrix.
    int rows_[R];
};

#endif
