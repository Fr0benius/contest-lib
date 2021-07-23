struct dsu {
  int N;
  int comps;
  vector<int> sz, par;
  dsu(int N) : N(N), comps(N), sz(N, 1), par(N) {
    iota(par.begin(), par.end(), 0);
  }
  int root(int n) { return n==par[n] ? n : par[n] = root(par[n]); }
  bool join(int a, int b) {
    a = root(a), b = root(b);
    if (a == b) return 0;
    if (sz[a] < sz[b]) swap(a, b);
    sz[a] += sz[b];
    par[b] = a;
    comps--;
    return 1;
  }
};

