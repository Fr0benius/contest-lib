//Calculate max flow with Dinitz's algorithm.
//Vertices numbered from 1 to N.
//Directed graph.
struct maxflow{
  int N,src,snk;
  struct edge{
    int from,to;
    ll cap;
    ll flow=0;
  };


  vector<vector<int>> g;
  vector<int> lvl;
  vector<int> ptrs;
  vector<edge> edges;

  maxflow(int NN,int sr,int sk)
    : N(NN),g(N+1),lvl(N+1),ptrs(N+1),src(sr),snk(sk) {}

  void add(int x,int y,ll cap){
    edges.push_back({x,y,cap});
    g[x].push_back(edges.size()-1);
    edges.push_back({y,x,cap,cap});
    g[y].push_back(edges.size()-1);
  }

  ll dfs(int v,ll lim=LLONG_MAX){
    if(lim<=0) return 0;
    if(v==snk) return lim;
    for(;ptrs[v]<g[v].size();ptrs[v]++){
      int ix=g[v][ptrs[v]];
      auto& fw=edges[ix];
      auto& rev=edges[ix^1];
      if(lvl[fw.to]-lvl[fw.from]!=1) continue;
      ll cur=dfs(fw.to,min(lim,fw.cap-fw.flow));
      if(cur>0){
        fw.flow+=cur;
        rev.flow-=cur;
        return cur;
      }
    }
    return 0;
  }

  bool bfs(){
    queue<int> q;
    fill(lvl.begin(),lvl.end(),0);
    lvl[src]=1;
    q.push(src);
    while(!q.empty()){
      int v=q.front();
      q.pop();
      for(int ix:g[v]){
        auto& e=edges[ix];
        if(e.flow>=e.cap || lvl[e.to]) continue;
        lvl[e.to]=lvl[v]+1;
        q.push(e.to);
      }
    }

    if(lvl[snk]==0) return 0;
    return 1;
  }

  ll dinitz(){
    ll res=0;
    ll cur=0;
    while(bfs()){
      fill(ptrs.begin(),ptrs.end(),0);
      while(cur=dfs(src)) res+=cur;
    }
    return res;
  }
};
