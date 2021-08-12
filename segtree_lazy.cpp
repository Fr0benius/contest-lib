template <typename T>
struct segtree {
  //Array indices from 0 to N-1
  const int N;

  using ValT = typename T::ValT;
  using UpdT = typename T::UpdT;
  const ValT val_id = T::val_id;
  const UpdT upd_id = T::upd_id;
  vector<ValT> vals;
  vector<UpdT> lazy;

  segtree(int N) : N(N), vals(4*(N+1), val_id), lazy(4*(N+1), upd_id) {}

  // Only use if x is idempotent
  segtree(int N, ValT x) : N(N), vals(4*(N+1), x), lazy(4*(N+1), upd_id) {}

  segtree(const vector<ValT>& v) : N(v.size()), vals(4*(N+1), val_id), lazy(4*(N+1), upd_id) {
    build(1, 0, N, v);
  }

  void build(int ix,int l,int r, const vector<ValT>& v){
    if(r-l==1){
      vals[ix] = v[l];
      return;
    }

    build(ix*2,l,(l+r)/2, v);
    build(ix*2+1,(l+r)/2,r, v);
    vals[ix] = T::combine(vals[2*ix], vals[2*ix+1]);
  }

  void push(int ix, bool nonleaf) {
    T::apply(vals[ix], lazy[ix]);
    if (nonleaf) {
      T::combine_update(lazy[2*ix], lazy[ix]);
      T::combine_update(lazy[2*ix+1], lazy[ix]);
    }
    lazy[ix] = T::upd_id;
  }

  // Updates interval [a,b)
  void update(int a,int b,UpdT v,int ix=1,int l=0,int r=-1){
    if (r == -1) r = N;
    push(ix, r-l > 1);
    if(b<=l || a>=r) return;

    if(a<=l && r<=b){
      T::combine_update(lazy[ix], v);
      push(ix, r-l > 1);
      return;
    }

    update(a,b,v,2*ix,l,(l+r)/2);
    update(a,b,v,2*ix+1,(l+r)/2,r);
    vals[ix] = T::combine(vals[2*ix], vals[2*ix+1]);
  }

  // Queries interval [a,b)
  ValT query(int a,int b,int ix=1,int l=0,int r=-1){
    if (r == -1) r = N;
    push(ix, r-l > 1);

    if(b<=l || a>=r) return T::val_id;
    if(a<=l && r<=b){
      return vals[ix];
    }

    ValT resl=query(a,b,2*ix,l,(l+r)/2);
    ValT resr=query(a,b,2*ix+1,(l+r)/2,r);
    return T::combine(resl, resr);
  }
};

// Query: min
// Update: add
// Initialized with infinity by default.
struct rmq_sum {
  using ValT = ll;
  using UpdT = ll;
  static const ValT val_id = LLONG_MAX;
  static const UpdT upd_id = 0;

  static ValT combine(ValT l, ValT r) {
    return min(l, r);
  }
  static void combine_update(UpdT& a, UpdT b) {
    a += b;
  }
  static void apply(ValT& x, UpdT u) {
    x += u;
  }
};

// Query: min
// Update: min
// Initialized with infinity by default.
struct rmq {
  using ValT = ll;
  using UpdT = ll;
  static const ValT val_id = LLONG_MAX;
  static const UpdT upd_id = LLONG_MAX;

  static ValT combine(ValT l, ValT r) {
    return min(l, r);
  }
  static void combine_update(UpdT& a, UpdT b) {
    a = min(a, b);
  }
  static void apply(ValT& x, UpdT u) {
    x = min(x, u);
  }
};

// Query: min
// Update: set
// Initialized with infinity by default.
// Uses a sentinel value (infinity) to represent no-op.
struct rmq_set {
  using ValT = ll;
  using UpdT = ll;
  static const ValT val_id = LLONG_MAX;
  static const UpdT upd_id = LLONG_MAX;

  static ValT combine(ValT l, ValT r) {
    return min(l, r);
  }
  static void combine_update(UpdT& a, UpdT b) {
    if (b != upd_id) a = b;
  }
  static void apply(ValT& x, UpdT u) {
    if (u != upd_id) x = u;
  }
};

// Query: max
// Update: set
// Initialized with -infinity by default.
// Uses a sentinel value (-infinity) to represent no-op.
struct rmxq_set {
  using ValT = ll;
  using UpdT = ll;
  static const ValT val_id = LLONG_MIN;
  static const UpdT upd_id = LLONG_MIN;

  static ValT combine(ValT l, ValT r) {
    return max(l, r);
  }
  static void combine_update(UpdT& a, UpdT b) {
    if (b != upd_id) a = b;
  }
  static void apply(ValT& x, UpdT u) {
    if (u != upd_id) x = u;
  }
};

