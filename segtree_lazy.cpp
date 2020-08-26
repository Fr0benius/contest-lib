// Node interface
// struct node {
//   using FromT = ...
//   using UpdateT = ...
//   Node();
//   Node(FromT v);
//   node combine(node other);
//   void push(node* l, node* r);
//   void update(UpdateT v);
// };

struct rmq_node {
  using ll = long long;
  using FromT = ll;
  using UpdateT = ll;
  const ll ident = LLONG_MAX;

  ll val, lazy;
  rmq_node() : val(ident), lazy(ident) {}
  rmq_node(ll v) : val(v), lazy(ident) {}
  rmq_node combine(rmq_node other) { other.val = min(val, other.val); return other; }
  void push(rmq_node* l, rmq_node* r) {
    val = min(val, lazy);
    if (l && r) {
      l->lazy = min(lazy, l->lazy);
      r->lazy = min(lazy, r->lazy);
    }
    lazy = ident;
  }
  void update(ll v) { lazy = min(lazy, update); }
};

template <typename Node, typename FromT = typename Node::FromT,
          typename UpdateT = typename Node::UpdateT>
struct segtree {
  //Array indices from 0 to N-1
  const int N;
  vector<Node> seg;

  segtree(int N_) : N(N_), seg(4*(N_+1)) {}
  segtree(const vector<FromT>& v) : N(v.size()) {
    build(1, 0, N, v);
  }

  void build(int ix,int l,int r, const vector<FromT>& v){
    if(r-l==1){
      seg[ix] = Node(v[l]);
      return;
    }

    build(ix*2,l,(l+r)/2, v);
    build(ix*2+1,(l+r)/2,r, v);
    seg[ix] = combine(seg[2*ix], seg[2*ix+1]);
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

    range_update(a,b,v,2*ix,l,(l+r)/2);
    range_update(a,b,v,2*ix+1,(l+r)/2,r);
    seg[ix] = combine(seg[2*ix], seg[2*ix+1]);
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
    return combine(resl, resr);
  }
  Node query(int a,int b) {return query(a,b,1,0,N);}
};
