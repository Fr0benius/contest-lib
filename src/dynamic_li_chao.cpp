// Dynamic Li Chao tree on the interval [left, right)
// T should be an integer type.
// Max queries. For min queries, flip signs on all lines and on query result.
template<typename T>
struct dynamic_li_chao {
  const T MIN = std::numeric_limits<T>::min();
  T left, right;
  vector<T> slp, icept;
  vector<int> lix, rix;

  dynamic_li_chao(T left, T right) 
    : left(left), right(right) {
      new_node();
    }
  int new_node() {
    slp.pb(0);
    icept.pb(MIN);
    lix.pb(-1);
    rix.pb(-1);
    return slp.size() - 1;
  }

  void add_line_impl(T m, T b, int ix, T l, T r) {
    if (r <= l) return;
    // Round down, not towards 0
    const ll mid = l + r >= 0 ? (l + r) / 2 : (l + r - 1) / 2;
    if (m * mid + b > slp[ix] * mid + icept[ix]) {
      swap(slp[ix], m);
      swap(icept[ix], b);
    }
    if (m > slp[ix]) {
      if (rix[ix] == -1) rix[ix] = new_node();
      add_line_impl(m, b, rix[ix], mid + 1, r);
    } else if (m < slp[ix]) {
      if (lix[ix] == -1) lix[ix] = new_node();
      add_line_impl(m, b, lix[ix], l, mid);
    }
  }
  void add_line(T m, T b) { add_line_impl(m, b, 0, left, right); }

  T query_impl(T x, int ix, T l, T r) {
    if (r <= l || ix == -1) return MIN;
    const ll mid = l + r >= 0 ? (l + r) / 2 : (l + r - 1) / 2;
    ll res = slp[ix] * x + icept[ix];
    if (x > mid) {
      res = max(res, query_impl(x, rix[ix], mid + 1, r));
    } else if (x < mid) {
      res = max(res, query_impl(x, lix[ix], l, mid));
    }
    return res;
  }
  T query(T x) { return query_impl(x, 0, left, right); }
};
