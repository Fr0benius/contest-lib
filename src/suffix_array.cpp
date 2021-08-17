const int MAXN=1010;

string s;
int N;
int gap;
int sa[MAXN];
int inv[MAXN];
int lcp[MAXN];

bool comp(int i,int j){
  if(inv[i]!=inv[j]) return inv[i]<inv[j];
  i+=gap,j+=gap;
  if(i>=N||j>=N) return i>j;
  return inv[i]<inv[j];
}

void buildsa(){
  for(int i=0;i<N;i++){
    sa[i]=i;inv[i]=s[i];
  }

  int temp[MAXN]={};
  for(gap=1;;gap*=2){
    sort(sa,sa+N,comp);
    for(int i=0;i<N-1;i++){
      temp[i+1]=temp[i]+comp(sa[i],sa[i+1]);
    }
    for(int i=0;i<N;i++){
      inv[sa[i]]=temp[i];
    }
    if(temp[N-1]==N-1) break;
  }
}

void buildlcp(){
  for(int i=0,k=0;i<N;i++){
    if(inv[i]==N-1){
      lcp[inv[i]]=0;continue;
    }
    int j=sa[inv[i]+1];
    while(i+k<N && j+k<N && s[i+k]==s[j+k]) k++;
    lcp[inv[i]]=k;
    if(k>0) k--;
  }
}
