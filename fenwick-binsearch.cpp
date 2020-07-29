template<typename T>
struct BIT {
  int N;
  vector<T> bit;

  BIT(int N_) : N(N_), bit(N_) { }

  inline T peek(int ix){
    int res=0;
    for(;ix;ix-=ix&-ix) res+=bit[ix];
    return res;
  }

  inline void update(int ix,T v){
    for(;ix<=N;ix+=ix&-ix) bit[ix]+=v;
  }

  // Find least k such that peek(k)>=v
  inline int lbound(T v){
    if(v<=0) return 0;

    int cur=0;
    int p=1<<30;

    while(1){
      while(p>0 && (cur+p>N || bit[cur+p]>=v)) p>>=1;
      if(p==0) return cur+1;
      cur+=p,v-=bit[cur],p>>=1;
    }
    return cur;
  }
};
