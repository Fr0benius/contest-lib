template <typename T>
struct sparse {
  vector<T> table[32];

  sparse() {}
  sparse(const vector<T>& v) { init(v); }
  void init(const vector<T>& v) {
    table[0] = v;
    for (size_t i = 1; (1 << i) <= v.size(); i++) {
      table[i].resize(v.size() - (1 << i) + 1);
      for (int j = 0; j < table[i].size(); j++) {
        table[i][j] = min(table[i - 1][j], table[i - 1][j + (1 << (i - 1))]);
      }
    }
  }

  // Query the range [l,r).
  T query(int l, int r) {
    int i = 31 - __builtin_clz(r - l);  // This is the floor of log_2(r-l+1)
    return min(table[i][l], table[i][r - (1 << i)]);
  }
};
