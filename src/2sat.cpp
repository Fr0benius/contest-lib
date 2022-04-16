// 2-SAT solver.
// Input: N, the number of variables.
// All variables must be in the range [0, N).
struct TwoSAT {
  vector<vector<int>> g;
  TwoSAT(int N) : g(2 * N) {}

  // Adds the constraint "v1=t1 OR v2=t2"
  void add(int v1, bool t1, int v2, bool t2) {
    g[2*v1 + !t1].pb(2 * v2 + t2);
    g[2*v2 + !t2].pb(2 * v1 + t1);
  }
  // Adds the constraint "v=t"
  void force(int v, bool t) {
    g[2*v + !t].pb(2 * v + t);
  }

  struct SCC {
    vector<vector<int>>& g;
    vector<vector<int>> gr;
    vector<int> seen, order;

    SCC(vector<vector<int>>& g) : g(g) {}

    void pseudo_topo(int v) {
      if (seen[v]) return;
      seen[v] = 1;
      for (int w : g[v]) pseudo_topo(w);
      order.pb(v);
    }

    void fill_comp(int v, vector<int>& comp) {
      if (seen[v]) return;
      seen[v] = 1;
      for (int w : gr[v]) fill_comp(w, comp);
      comp.pb(v);
    }

    vector<vector<int>> compute() {
      int N = g.size();
      gr = vector<vector<int>>(N);
      for (int v = 0; v < N; v++) {
        for (int w : g[v]) gr[w].pb(v);
      }

      seen = vector<int>(N, 0);
      order.clear();
      for (int v = 0; v < N; v++) pseudo_topo(v);
      reverse(order.begin(), order.end());

      seen.assign(N, 0);
      vector<vector<int>> comps;
      for (int v : order) {
        if (!seen[v]) {
          comps.pb({});
          fill_comp(v, comps.back());
        }
      }
      return comps;
    }
  };
  // Returns empty vector if no solution.
  // Otherwise returns value for each variable.
  vector<int> solve() {
    SCC scc(g);
    vector<int> res(g.size() / 2, -1);
    vector<vector<int>> comps = scc.compute();
    while (!comps.empty()) {
      auto& comp = comps.back();
      for (int x : comp) {
        int v = x/2, t = x&1;
        if (res[v] >= 2) return {};
        if (res[v] == -1) res[v] = 2 + t;
      }
      for (int x : comp) {
        int v = x/2;
        if (res[v] >= 2) res[v] -= 2;
      }
      comps.pop_back();
    }
    return res;
  }
};
