template<typename T>
struct BIT {
  int N;
  vector<T> bit;

  BIT(int sz) : N(sz), bit(sz+1) { }

  inline void update(int ix,T v){
    assert(ix >= 0);
    assert(ix < N);
    ix++;
    for(;ix<=N;ix+=ix&-ix) bit[ix]+=v;
  }

  // queries the interval [0, ix)
  inline T query(int ix){
    assert(ix >= 0);
    assert(ix <= N);
    T res=0;
    for(;ix;ix-=ix&-ix) res+=bit[ix];
    return res;
  }

  // Find least k such that query(k)>=v
  // Only works if array elements are nondecreasing
  inline int lbound(T v){
    if(v<=0) return 0;
    int lg = 31 - __builtin_clz(N);
    int k = 0;
    for (int i = lg; i >= 0; i--) {
      T x = bit[k + (1<<i)];
      if (v > x) {
        v -= x;
        k += 1<<i;
      }
    }
    return k+1;
  }
};
