// Kosaraju's SCC algorithm.
// Input is a graph, assuming vertices from 0 to length-1.
// Output is a vector of components in topological order.
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
