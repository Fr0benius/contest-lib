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
  using ValT = ll;
  using UpdT = ll;
  static constexpr ValT val_id = 0;
  static constexpr UpdT upd_id = 0;

  static ValT combine(ValT l, ValT r) {
    return l + r;
  }
  static void apply(ValT& x, UpdT u) {
    x += u;
  }
};
