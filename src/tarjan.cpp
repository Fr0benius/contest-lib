const int MAXN=100100;

int N;
vector<int> g[MAXN];
bool onst[MAXN];
vector<int> stk;
int low[MAXN],ord[MAXN],cur=1;

int K; //number of SCCs
vector<int> comps[MAXN];
int comp[MAXN];


//vertices 1 to N
void tarj(int v){
  low[v]=ord[v]=cur++;
  stk.pb(v);
  onst[v]=1;
  for(int w:g[v]){
    if(ord[w]==0) tarj(w),low[v]=min(low[v],low[w]);
    if(onst[w])
      low[v]=min(low[v],ord[w]);
  }
  if(low[v]==ord[v]){
    int w;
    do{
      w=stk.back();
      stk.pop_back();
      onst[w]=0;
      comps[K].pb(w);
      comp[w]=K;
    }while(w!=v);
    K++;
  }
}
