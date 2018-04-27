const int MAXN=100100;

int N;
vector<int> g[MAXN];
vector<int> eul;
int lab[MAXN];
int inv[MAXN*2];
int sp[MAXN*2][20];
int par[MAXN][20];

void dfs1(int v,int p){
  if(lab[v]==-1) lab[v]=eul.size(),inv[lab[v]]=v;
  par[v][0]=p;
  
  eul.pb(lab[v]);

  for(int w:g[v]){
    if(w==p) continue;
    dfs1(w,v);
    eul.pb(lab[v]);
  }
}

void init(){
  memset(lab,-1,sizeof lab);
  dfs1(1,0);
  
  for(int i=0;i<eul.size();i++) sp[i][0]=eul[i];

  for(int j=1;(1<<j)<=eul.size();j++){
    for(int i=0;i+(1<<j)<=eul.size();i++){
      sp[i][j]=min(sp[i][j-1],sp[i+(1<<(j-1))][j-1]);
    }
  }

  //for going up the table
  for(int j=1;(1<<j)<=N;j++){
    for(int i=1;i<=N;i++){
      par[i][j]=par[par[i][j-1]][j-1];
    }
  }

}


inline int lca(int v,int w){
  v=lab[v],w=lab[w];
  if(v>w) swap(v,w);
  int j=31-__builtin_clz(w-v+1);
  // cout<<v<<' '<<w<<' '<<j<<'\n';
  return inv[min(sp[v][j],sp[w-(1<<j)+1][j])];
}

inline int up(int v,int k){
  int j=31-__builtin_clz(k);
  while(k>0){
    while((1<<j)>k) j--;
    v=par[v][j];
    k-=(1<<j);
  }
  return v;
}
