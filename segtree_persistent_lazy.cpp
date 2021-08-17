// TODO: Doesn't work - need to fix lazy propagation logic
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
    UpdT lazy = upd_id;
    int left = -1;
    int right = -1;
  };

  vector<node> nodes;

  int new_node() {
    nodes.pb({});
    return nodes.size() - 1;
  }

  persistent_segtree(int L, int R, int cap = 0) : L(L), R(R), nodes(1) {
    nodes[0].val = T::new_val(L, R);
    if (cap > 0) nodes.reserve(cap);
  }

  void push(int ix, int l, int r) {
    T::apply(nodes[ix].val, nodes[ix].lazy);
    if (r - l > 1) {
      int m = (l+r)/2;
      if (nodes[ix].left == -1) {
        nodes[ix].left = new_node();
        nodes[nodes[ix].left].val = T::new_val(l, m);
        nodes[ix].right = new_node();
        nodes[nodes[ix].right].val = T::new_val(m, r);
      }
      T::combine_update(nodes[nodes[ix].left].lazy, nodes[ix].lazy);
      T::combine_update(nodes[nodes[ix].right].lazy, nodes[ix].lazy);
    }
    nodes[ix].lazy = T::upd_id;
  }

  // Updates interval [a,b), returning new root index
  int update(int a,int b,UpdT v,int ix=0,int l=0,int r=-1){
    if (r < l) l = L, r = R;
    push(ix, l, r);
    if(b<=l || a>=r) return ix;

    if(a<=l && r<=b){
      int ixn = new_node();
      nodes[ixn] = nodes[ix];
      if (nodes[ixn].left != -1) {
        int ixl = new_node(), ixr = new_node();
        nodes[ixl] = nodes[nodes[ixn].left];
        nodes[ixr] = nodes[nodes[ixn].right];
        nodes[ixn].left = ixl;
        nodes[ixn].right = ixr;
      }
      T::combine_update(nodes[ixn].lazy, v);
      push(ixn, l, r);
      return ixn;
    }

    int ixl = update(a,b,v,nodes[ix].left,l,(l+r)/2);
    int ixr = update(a,b,v,nodes[ix].right,(l+r)/2,r);
    int ixn = new_node();
    nodes[ixn].val = T::combine(nodes[ixl].val, nodes[ixr].val);
    nodes[ixn].left = ixl;
    nodes[ixn].right = ixr;
    return ixn;
  }

  // Queries interval [a,b)
  ValT query(int a,int b,int ix=0,int l=0,int r=-1){
    if (r < l) l = L, r = R;
    push(ix, l, r);

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
  using ValT = pll;
  static constexpr ValT val_id = {0, 0};
  using UpdT = ll;
  static constexpr UpdT upd_id = 0;

  static ValT new_val(ll l, ll r) {
    return {0, r - l};
  }
  static ValT combine(ValT l, ValT r) {
    return {l.fst + r.fst, l.snd + r.snd};
  }
  static void combine_update(UpdT& a, UpdT b) {
    a += b;
  }
  static void apply(ValT& x, UpdT u) {
    x.fst += x.snd * u;
  }
};

