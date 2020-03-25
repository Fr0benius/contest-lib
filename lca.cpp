const int MAXN=100100;

int N;
vector<int> g[MAXN]; // The graph is assumed to have vertices 1,...,N.

// Vertices are labeled in preorder (enter times)
vector<int> lab, invlab; // vertex-to-label, label-to-vertex
vector<int> seq; // Euler tour of the tree, using vertex labels.
vector<int> lab_to_ix; // Vertex label to index of first occurrence in the tour

// Sparse table corresponding to seq
sparse<int> sp;

vector<int> dep; // Depth of each node. Used to calculate distance.
int par[MAXN][32];


void init_dfs(int v, int p) {
  int cur = invlab.size();
  dep[v] = dep[p]+1;
  lab[v] = cur;
  invlab.pb(v);
  lab_to_ix.pb(seq.size());
  seq.pb(cur);

  par[v][0] = p;

  for (int w: g[v]) {
    if (w==p) continue;
    init_dfs(w,v);
    seq.pb(cur);
  }
}

void init(){
  lab.resize(N+1);
  dep.resize(N+1);
  invlab.clear();
  seq.clear();

  init_dfs(1,0);

  sp.init(seq);

  //for going up the table
  for(int j=1;(1<<j)<=N;j++){
    for(int i=1;i<=N;i++){
      par[i][j]=par[par[i][j-1]][j-1];
    }
  }

}

inline int lca(int a, int b) {
  a = lab_to_ix[lab[a]];
  b = lab_to_ix[lab[b]];
  if(a>b) swap(a,b);
  return invlab[sp.query(a,b)];
}


// Go k steps up the tree from vertex v.
inline int up(int v,int k){
  int j=31-__builtin_clz(k);
  while(k>0){
    while((1<<j)>k) j--;
    v=par[v][j];
    k-=(1<<j);
  }
  return v;
}
