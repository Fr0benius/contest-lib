struct segtree{
  using T = long long;
  struct node{
    T val;
    T lazy;
  };

  //Array indices from 0 to N-1
  int N;
  vector<node> seg;

  segtree(int NN) : N(NN), seg(4*(N+1)) {}

  inline node combine(node l, node r){

  }

  void build(int ix,int l,int r){
    if(r-l==1){
      //initiate singletons here
      return;
    }

    build(ix*2,l,(l+r)/2);
    build(ix*2+1,(l+r)/2,r);
    seg[ix] = combine(seg[2*ix], seg[2*ix+1]);
  }
  void build() {build(1,0,N);}

  inline void push(int ix,bool leaf){//apply lazy computation and push to children
    if(seg[ix].lazy==0) return;
    if(!leaf) {
      // Propagate to children
    }
    seg[ix].lazy = 0;
  }

  // Not for lazy propagation
  void single_update(int k,T v,int ix,int l,int r){
    if(k<l || k>=r) return;
    if(r-l==1){
      //Update here
      return;
    }

    single_update(k,v,2*ix,l,(l+r)/2);
    single_update(k,v,2*ix+1,(l+r)/2,r);
    seg[ix] = combine(seg[2*ix], seg[2*ix+1]);
  }
  void single_update(int k, T v) {single_update(k,v,1,0,N);}

  // Updates interval [a,b)
  void range_update(int a,int b,T v,int ix,int l,int r){
    push(ix,r-l==1);
    if(b<=l || a>=r) return;

    if(a<=l && r<=b){
      //Update here
      push(ix,r-l==1);
      return;
    }

    range_update(a,b,v,2*ix,l,(l+r)/2);
    range_update(a,b,v,2*ix+1,(l+r)/2,r);
    seg[ix] = combine(seg[2*ix], seg[2*ix+1]);
  }
  void range_update(int a,int b,T v) {range_update(a,b,v,1,0,N);}

  // Queries interval [a,b)
  node query(int a,int b,int ix,int l,int r){
    push(ix,r-l==1);

    if(b<=l || a>=r) return node();//return identity here
    if(a<=l && r<=b){
      //single segment query
      return seg[ix];
    }

    node resl=query(a,b,2*ix,l,(l+r)/2);
    node resr=query(a,b,2*ix+1,(l+r)/2,r);
    return combine(resl, resr);
  }
  T query(int a,int b) {return query(a,b,1,0,N).val;}
};
