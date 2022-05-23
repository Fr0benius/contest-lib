// https://codeforces.com/contest/932/problem/F
// Solution with dynamic Li Chao Tree

#include <iostream>
#include <functional>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <climits>
#include <cmath>
#include <cstring>
#include <cassert>
#include <chrono>
#include <random>
#include <bitset>
#include <complex>
#include <unordered_set>
#include <unordered_map>
using namespace std;

#define pb push_back
#define fst first
#define snd second

#ifdef QLEG_DEBUG
template<typename T> ostream& operator<<(ostream& os, const vector<T>& v) { os << '{'; string sep; for (const auto& x : v) os << sep << x, sep = ", "; return os << '}'; }
template<typename A, typename B> ostream& operator<<(ostream& os, const pair<A, B>& p) { return os << '(' << p.first << ", " << p.second << ')'; }
void dbg_out() { cerr << endl; }
template<typename Head, typename... Tail> void dbg_out(Head H, Tail... T) { cerr << ' ' << H; dbg_out(T...); }

#define dbg(...) cerr << "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
#else
#define dbg(...)
#endif


template<class Fun>
class _y_combinator_result {
    Fun fun_;
public:
    template<class T>
    explicit _y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

    template<class ...Args>
    decltype(auto) operator()(Args &&...args) {
        return fun_(std::ref(*this), std::forward<Args>(args)...);
    }
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
    return _y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll, ll>;
template<typename T> using min_queue=priority_queue<T,vector<T>,greater<T>>;

// END OF TEMPLATE

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
  void add_line(T m, T b) { 
    dbg("adding line", m, b);
    add_line_impl(m, b, 0, left, right); }

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
  T query(T x) { 
    dbg("Querying", x);
    return query_impl(x, 0, left, right); }
};

template<typename T>
void swap(dynamic_li_chao<T>& a, dynamic_li_chao<T>& b) {
  swap(a.left, b.left);
  swap(a.right, b.right);
  swap(a.lix, b.lix);
  swap(a.rix, b.rix);
  swap(a.slp, b.slp);
  swap(a.icept, b.icept);
}

int main() {
  ios::sync_with_stdio(0);cin.tie(0);

  ll N; cin >> N;
  vector<ll> a(N); for (auto& _x : a) cin >> _x;
  vector<ll> b(N); for (auto& _x : b) cin >> _x;
  vector<vector<int>> g(N);
  for (int i = 0; i < N-1; i++) {
    int u, v; cin >> u >> v;
    u--, v--;
    g[u].pb(v);
    g[v].pb(u);
  }

  vector<ll> res(N);
  vector<vector<int>> sub(N);
  vector<dynamic_li_chao<ll>> trees;
  for (int i = 0; i < N; i++) trees.emplace_back(-100000, 100001);

  y_combinator([&] (auto self, int v, int p) -> void {
    for (int w : g[v]) {
      if (w == p) continue;
      self(w, v);
      if (sub[v].size() < sub[w].size()) {
        swap(sub[v], sub[w]);
        swap(trees[v], trees[w]);
      }
      for (int k : sub[w]) {
        sub[v].pb(k);
        trees[v].add_line(-b[k], -res[k]);
      }
    }
    if (v == 0 || g[v].size() > 1)
      res[v] = -trees[v].query(a[v]);
    sub[v].pb(v);
    trees[v].add_line(-b[v], -res[v]);
    dbg(v, sub[v], res[v]);
  })(0, -1);

  for (int i = 0; i < N; i++) cout << res[i] << ' ';
  cout << '\n';
}
