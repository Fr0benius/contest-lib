struct matcher {
  int N, M;
  // The bipartite parts are given explicitly - i is in [0, N) and g[i] is in [0,M).
  const vector<vector<int>>& g;
  vector<int> l_to_r, r_to_l, seen;


  matcher(int N, int M, const vector<vector<int>>& g)
    : N(N), M(M) , g(g), l_to_r(N, -1), r_to_l(M, -1), seen(N) {}

  bool dfs(int v) {
    if (seen[v]) return 0;
    seen[v] = 1;
    for (int w : g[v]) {
      if (r_to_l[w] == -1) {
        l_to_r[v] = w;
        r_to_l[w] = v;
        return 1;
      }
    }
    for (int w : g[v]) {
      if (dfs(r_to_l[w])) {
        l_to_r[v] = w;
        r_to_l[w] = v;
        return 1;
      }
    }
    return 0;
  }

  int match() {
    bool done = 0;
    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    shuffle(order.begin(), order.end(), rng);
    while (!done) {
      done = 1;
      seen.assign(N, 0);
      for (int v : order) {
        if (l_to_r[v] == -1 && dfs(v)) done = 0;
      }
    }
    int res = 0;
    for (int r : l_to_r) if (r != -1) res++;
    return res;
  }
};
