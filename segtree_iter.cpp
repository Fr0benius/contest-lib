const int MAXN=100100;

struct node{
  int val;
};

int N;
node seg[2*MAXN];
int a[MAXN];

inline node combine(const node& a,const node& b){
  //TODO: write combination code

  return {0};
}

inline void init(){
  for(int i=0;i<N;i++) seg[i+N]={a[i]};

  for(int i=N-1;i>0;i--) seg[i]=combine(seg[2*i],seg[2*i+1]);
}

inline void update(int i,const node& v){
  i+=N;//0-indexed
  seg[i]=v;
  while(i>0){
    i/=2;
    seg[i]=combine(seg[2*i],seg[2*i+1]);
  }
}

inline int query(int l,int r){
  l+=N,r+=N;//0-indexed, [l,r)
  node resl={-1},resr={-1};
  while(l<r){
    if(l&1) resl=combine(resl,seg[l++]);
    if(r&1) resr=combine(seg[--r],resr);
    l/=2,r/=2;
  }
  return combine(resl,resr).val;
}
