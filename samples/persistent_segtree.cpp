// https://codeforces.com/problemset/problem/786/C
// Solution using persistent segment tree

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

template <typename T, typename I=int>
struct persistent_segtree {
  // indices from L to R-1
  const I L, R;

  using ValT = typename T::ValT;
  using UpdT = typename T::UpdT;
  static constexpr ValT val_id = T::val_id;
  static constexpr UpdT upd_id = T::upd_id;

  struct node {
    ValT val = val_id;
    int left = -1;
    int right = -1;
  };

  vector<ValT> vals;
  vector<int> left, right;

  int new_node() {
    vals.pb(val_id);
    left.pb(-1);
    right.pb(-1);
    return vals.size() - 1;
  }

  persistent_segtree(I L, I R, int cap = 0) : L(L), R(R) {
    new_node();
    if (cap > 0) vals.reserve(cap), left.reserve(cap), right.reserve(cap);
  }

  // Returns new root index
  int update(I a, UpdT v,int ix=0,I l=0,I r=-1){
    if (r < l) l = L, r = R;
    if(a<l || a>=r) return ix;

    if(r-l == 1) {
      int ixn = new_node();
      vals[ixn] = vals[ix];
      T::apply(vals[ixn], v);
      return ixn;
    }

    int ixl = left[ix];
    int ixr = right[ix];
    if (ixl == -1) ixl = new_node(), ixr = new_node();
    const I m = (l+r)/2;
    if (a < m) {
      ixl = update(a,v,ixl,l,m);
    } else {
      ixr = update(a,v,ixr,m,r);
    }
    int ixn = new_node();
    vals[ixn] = T::combine(vals[ixl], vals[ixr]);
    left[ixn] = ixl;
    right[ixn] = ixr;
    return ixn;
  }

  // Queries interval [a,b)
  ValT query(I a,I b,int ix=0,I l=0,I r=-1){
    if (r < l) l = L, r = R;
    if(b<=l || a>=r) return T::val_id;
    if(a<=l && r<=b){
      return vals[ix];
    }
    ValT resl=query(a,b,left[ix],l,(l+r)/2);
    ValT resr=query(a,b,right[ix],(l+r)/2,r);
    return T::combine(resl, resr);
  }

  // returns lowest r s.t. query(L, r) satisfies predicate (R+1 otherwise)
  template<typename P>
  I lower_bound(P&& pred, int ix=0, I l=0, I r=-1, ValT acc=val_id) {
    if (r < l) {
      if (!pred(vals[ix])) return R+1;
      l = L, r = R;
    }
    
    if (left[ix] == -1) {
      return pred(acc) ? l : r;
    }
    const I m = (l+r)/2;
    ValT nacc = T::combine(acc, vals[left[ix]]);
    if (pred(nacc))
      return lower_bound(pred, left[ix], l, m, acc);
    else
      return lower_bound(pred, right[ix], m, r, nacc);
  }
};

struct seg_add {
  using ValT = int;
  using UpdT = int;
  static constexpr ValT val_id = 0;
  static constexpr UpdT upd_id = 0;

  static ValT combine(ValT l, ValT r) {
    return l + r;
  }
  static void apply(ValT& x, UpdT u) {
    x += u;
  }
};

int main(){
  ios::sync_with_stdio(0);cin.tie(0);

  int N; cin >> N;
  vector<int> a(N); for (auto& x : a) cin >> x;

  persistent_segtree<seg_add> seg(0, N);
  vector<int> roots(N);
  vector<int> prv(N+1, -1);
  int root = 0;
  for (int i = N-1; i >= 0; i--) {
    int x = a[i];
    root = seg.update(i, 1, root);
    if (prv[x] != -1)
      root = seg.update(prv[x], -1, root);
    roots[i] = root;
    prv[x] = i;
  }

  /* dbg(seg.query(3, 4, roots[3])); */
  /* dbg(seg.query(3, 5, roots[3])); */
  for (int k = 1; k <= N; k++) {
    int res = 0;
    int j = 0;
    while (j < N) {
      int j1 = seg.lower_bound([k] (int x) { 
          return x > k;
      }, roots[j]);
      /* dbg(k, j, j1); */
      j = j1-1;
      res++;
    }
    cout << res << ' ';
  }
  cout << '\n';
}
