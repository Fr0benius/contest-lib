struct node{
  int val;
  int lazy;
};


const int MAXN=100100;

int N;
node seg[4*MAXN];

void build(int ix=1,int l=0,int r=N){
  if(r-l<=1){
    //initiate singletons here
    return;
  }
  build(ix*2,l,(l+r)/2);
  build(ix*2+1,(l+r)/2,r);

  //Combine ix*2 and ix*2+1 into ix here
}

inline void push(int ix){//push lazy computation to children
  if(seg[ix].lazy==0) return;
}

void update(int a,int b,int v,int ix=1,int l=0,int r=N){
  if(r-l>1) push(ix);

  if(b<=l || a>=r) return;

  if(a<=l && r<=b){
    //lazy update here

    return;
  }

  update(a,b,v,2*ix,l,(l+r)/2);
  update(a,b,v,2*ix+1,(l+r)/2,r);

  //Combine ix*2 and ix*2+1 into ix here
}

int query(int a,int b,int ix=1,int l=0,int r=N){
  push(ix);

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
