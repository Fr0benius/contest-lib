template <typename T>
struct persistent_segtree {
  // indices from L to R-1
  const int L, R;

  using ValT = typename T::ValT;
  using UpdT = typename T::UpdT;
  static constexpr ValT val_id = T::val_id;
  static constexpr UpdT upd_id = T::upd_id;

  struct node {
    ValT val = val_id;
    int left = -1;
    int right = -1;
  };

  vector<node> nodes;

  int new_node() {
    nodes.pb({});
    return nodes.size() - 1;
  }

  persistent_segtree(int L, int R, int cap = 0) : L(L), R(R), nodes(1) {
    if (cap > 0) nodes.reserve(cap);
  }

  // Returns new root index
  int update(int a, UpdT v,int ix=0,int l=0,int r=-1){
    if (r < l) l = L, r = R;
    if(a<l || a>=r) return ix;

    if(r-l == 1) {
      int ixn = new_node();
      nodes[ixn] = nodes[ix];
      T::apply(nodes[ixn].val, v);
      return ixn;
    }

    int ixl = nodes[ix].left;
    int ixr = nodes[ix].right;
    if (ixl == -1) ixl = new_node(), ixr = new_node();
    const int m = (l+r)/2;
    if (a < m) {
      ixl = update(a,v,ixl,l,m);
    } else {
      ixr = update(a,v,ixr,m,r);
    }
    int ixn = new_node();
    nodes[ixn].val = T::combine(nodes[ixl].val, nodes[ixr].val);
    nodes[ixn].left = ixl;
    nodes[ixn].right = ixr;
    return ixn;
  }

  // Queries interval [a,b)
  ValT query(int a,int b,int ix=0,int l=0,int r=-1){
    if (r < l) l = L, r = R;
    if(b<=l || a>=r) return T::val_id;
    if(a<=l && r<=b){
      return nodes[ix].val;
    }
    ValT resl=query(a,b,nodes[ix].left,l,(l+r)/2);
    ValT resr=query(a,b,nodes[ix].right,(l+r)/2,r);
    return T::combine(resl, resr);
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
