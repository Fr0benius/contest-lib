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
    vals[ix] = T::apply(vals[ix], lazy[ix]);
    if (nonleaf) {
      lazy[2*ix] = T::combine_update(lazy[2*ix], lazy[ix]);
      lazy[2*ix+1] = T::combine_update(lazy[2*ix+1], lazy[ix]);
    }
    lazy[ix] = T::upd_id;
  }

  // Updates interval [a,b)
  void update(int a,int b,UpdT v,int ix=1,int l=0,int r=-1){
    if (r == -1) r = N;
    push(ix, r-l > 1);
    if(b<=l || a>=r) return;

    if(a<=l && r<=b){
      lazy[ix] = T::combine_update(lazy[ix], v);
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
struct rmq_sum_node {
  using ValT = ll;
  using UpdT = ll;
  static const ValT val_id = LLONG_MAX;
  static const UpdT upd_id = 0;

  static ValT combine(ValT l, ValT r) {
    return min(l, r);
  }
  static UpdT combine_update(UpdT a, UpdT b) {
    return a + b;
  }
  static ValT apply(ValT x, UpdT u) {
    return x + u;
  }
};

// Query: min
// Update: min
// Initialized with infinity by default.
struct rmq_node {
  using ValT = ll;
  using UpdT = ll;
  static const ValT val_id = LLONG_MAX;
  static const UpdT upd_id = LLONG_MAX;

  static ValT combine(ValT l, ValT r) {
    return min(l, r);
  }
  static UpdT combine_update(UpdT a, UpdT b) {
    return min(a, b);
  }
  static ValT apply(ValT x, UpdT u) {
    return min(x, u);
  }
};
