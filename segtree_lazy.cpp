template <typename Node, typename FromT = typename Node::FromT,
          typename UpdateT = typename Node::UpdateT>
struct segtree {
  //Array indices from 0 to N-1
  const int N;
  vector<Node> seg;

  segtree(int N_) : N(N_), seg(4*(N_+1)) {}
  segtree(const vector<FromT>& v) : N(v.size()), seg(4*(v.size()+1)) {
    build(1, 0, N, v);
  }

  void build(int ix,int l,int r, const vector<FromT>& v){
    if(r-l==1){
      seg[ix] = Node(v[l]);
      return;
    }

    build(ix*2,l,(l+r)/2, v);
    build(ix*2+1,(l+r)/2,r, v);
    seg[ix] = seg[2*ix].combine(seg[2*ix+1]);
  }

  // Updates interval [a,b)
  void update(int a,int b,UpdateT v,int ix,int l,int r){
    Node* left_node = r-l>1 ? &seg[2*ix] : nullptr;
    Node* right_node = r-l>1 ? &seg[2*ix+1] : nullptr;
    seg[ix].push(left_node, right_node);
    if(b<=l || a>=r) return;

    if(a<=l && r<=b){
      seg[ix].update(v);
      seg[ix].push(left_node, right_node);
      return;
    }

    update(a,b,v,2*ix,l,(l+r)/2);
    update(a,b,v,2*ix+1,(l+r)/2,r);
    seg[ix] = seg[2*ix].combine(seg[2*ix+1]);
  }
  void update(int a,int b,UpdateT v) {update(a,b,v,1,0,N);}

  // Queries interval [a,b)
  Node query(int a,int b,int ix,int l,int r){
    Node* left_node = r-l>1 ? &seg[2*ix] : nullptr;
    Node* right_node = r-l>1 ? &seg[2*ix+1] : nullptr;
    seg[ix].push(left_node, right_node);

    if(b<=l || a>=r) return Node();//return identity here
    if(a<=l && r<=b){
      return seg[ix];
    }

    Node resl=query(a,b,2*ix,l,(l+r)/2);
    Node resr=query(a,b,2*ix+1,(l+r)/2,r);
    return resl.combine(resr);
  }
  Node query(int a,int b) {return query(a,b,1,0,N);}
};

// Query: min
// Update: add
// Initialized with infinity by default.
struct rmq_sum_node {
  using FromT = ll;
  using UpdateT = ll;
  static const ll INF = LLONG_MAX;

  ll val, lazy;
  rmq_sum_node() : val(INF), lazy(0) {}
  rmq_sum_node(FromT v) : val(v), lazy(0) {}
  rmq_sum_node combine(rmq_sum_node other) {
    assert(lazy==0);assert(other.lazy==0);
    other.val = min(val, other.val);
    return other;
  }
  void push(rmq_sum_node* l, rmq_sum_node* r) {
    val += lazy;
    if (l && r) {
      l->lazy += lazy;
      r->lazy += lazy;
    }
    lazy = 0;
  }
  void update(UpdateT v) { lazy += v; }
};

// Query: min
// Update: min
// Initialized with infinity by default.
struct rmq_node {
  using FromT = ll;
  using UpdateT = ll;
  static const ll INF = LLONG_MAX;

  ll val, lazy;
  rmq_node() : val(INF), lazy(INF) {}
  rmq_node(FromT v) : val(v), lazy(INF) {}
  rmq_node combine(rmq_node other) {
    other.val = min(val, other.val);
    return other;
  }
  void push(rmq_node* l, rmq_node* r) {
    val = min(val, lazy);
    if (l && r) {
      l->lazy = min(lazy, l->lazy);
      r->lazy = min(lazy, r->lazy);
    }
    lazy = INF;
  }
  void update(UpdateT v) { lazy = min(lazy, v); }
};
