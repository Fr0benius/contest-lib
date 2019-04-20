struct node{
  int val;
  int lazy;
};


const int MAXN=100100;

int N;
node seg[4*MAXN];

inline void combine(int ix){
  
}

//Array indices from 0 to N-1
void build(int ix=1,int l=0,int r=N){
  if(r-l<=1){
    //initiate singletons here
    return;
  }
  build(ix*2,l,(l+r)/2);
  build(ix*2+1,(l+r)/2,r);

  combine(ix);
}

inline void push(int ix,bool nonleaf){//apply lazy computation and push to children
  if(seg[ix].lazy==0) return;
}

void single_update(int k,int v,int ix=1,int l=0,int r=N){
  if(k<l || k>=r) return;
  if(r-l==1){
    //Update here
    return;
  }

  single_update(k,v,2*ix,l,(l+r)/2);
  single_update(k,v,2*ix+1,(l+r)/2,r);
  combine(ix);
}
void range_update(int a,int b,int v,int ix=1,int l=0,int r=N){
  if(b<=l || a>=r) return;

  if(a<=l && r<=b){
    //Update here
    push(ix,r-l>1);

    return;
  }

  push(ix,r-l>1);
  range_update(a,b,v,2*ix,l,(l+r)/2);
  range_update(a,b,v,2*ix+1,(l+r)/2,r);
  combine(ix);
}

int query(int a,int b,int ix=1,int l=0,int r=N){
  push(ix,r-l>1);

  if(b<=l || a>=r) return 0;//return identity here
  if(a<=l && r<=b){
    int res=0;
    //single segment query
    return res;
  }

  int resl=query(a,b,2*ix,l,(l+r)/2);
  int resr=query(a,b,2*ix+1,(l+r)/2,r);

  int res=0;
  //combine queries here

  return res;
}
