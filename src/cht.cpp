// Basic Convex Hull Trick data structure.
// Can be used for maximum or minimum queries.
// Lines have to be added in the correct order
// (increasing slope for max, decreasing slope for min)
// Slopes should be distinct.
template<typename T>
struct CHT {
  vector<T> xvals;
  // lines are pairs {m, b}
  vector<pair<T, T>> lines;

  inline static T isect(T m1, T b1, T m2, T b2) {
    T y = b1 - b2;
    T x = m2 - m1;
    if (x < 0) x = -x, y = -y;
    return (y > 0 ? y : y - x + 1) / x;  //round down
  }

  void add_line(T m, T b) {
    if (lines.empty()) {
      lines.pb({m,b});
      return;
    }
    T x;
    while (1) {
      auto [m0, b0] = lines.back();
      x = isect(m0, b0, m, b);
      if (xvals.empty() || xvals.back() < x)
        break;
      xvals.pop_back();
      lines.pop_back();
    }
    xvals.pb(x);
    lines.pb({m, b});
  }
  T query (T x) {
    assert(!lines.empty());
    int j = 0;
    if (xvals.size() >= 1) {
      j = lower_bound(xvals.begin(), xvals.end(), x)
        - xvals.begin();
    }
    auto [m, b] = lines[j];
    return m * x + b;
  }
};
