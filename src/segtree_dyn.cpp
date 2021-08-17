// Not tested, may have memory issues
template <typename T>
struct dynamic_segtree {
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

  dynamic_segtree(int L, int R, int cap = 0) : L(L), R(R), nodes(1) {
    if (cap > 0) nodes.reserve(cap);
  }

  void push(int ix, bool nonleaf) {
    T::apply(nodes[ix].val, nodes[ix].lazy);
    if (nonleaf) {
      if (nodes[ix].left == -1) nodes[ix].left = nodes.size(), nodes.pb({});
      if (nodes[ix].right == -1) nodes[ix].right = nodes.size(), nodes.pb({});
      T::combine_update(nodes[nodes[ix].left].lazy, nodes[ix].lazy);
      T::combine_update(nodes[nodes[ix].right].lazy, nodes[ix].lazy);
    }
    nodes[ix].lazy = T::upd_id;
  }

  // Updates interval [a,b)
  void update(int a,int b,UpdT v,int ix=0,int l=0,int r=-1){
    if (r < l) l = L, r = R;
    push(ix, r-l > 1);
    if(b<=l || a>=r) return;

    if(a<=l && r<=b){
      T::combine_update(nodes[ix].lazy, v);
      push(ix, r-l > 1);
      return;
    }

    update(a,b,v,nodes[ix].left,l,(l+r)/2);
    update(a,b,v,nodes[ix].right,(l+r)/2,r);
    nodes[ix].val = T::combine(nodes[nodes[ix].left].val, nodes[nodes[ix].right].val);
  }

  // Queries interval [a,b)
  ValT query(int a,int b,int ix=0,int l=0,int r=-1){
    if (r < l) l = L, r = R;
    push(ix, r-l > 1);

    if(b<=l || a>=r) return T::val_id;
    if(a<=l && r<=b){
      return nodes[ix].val;
    }

    ValT resl=query(a,b,nodes[ix].left,l,(l+r)/2);
    ValT resr=query(a,b,nodes[ix].right,(l+r)/2,r);
    return T::combine(resl, resr);
  }
};
