#ifndef SCANNER_H
#define SCANNER_H

template <int R, int C>
class Scanner {
  public:
    virtual ~Scanner() {}
    virtual Matrix<R,C> Scan() = 0;
};

#endif
