struct suffix_array {
  string s;
  int N;
  vector<int> sa;
  vector<int> inv;
  vector<int> lcp;

  suffix_array(string s) : s(s), N(s.size()), sa(N), inv(N), lcp(N-1) {
    buildsa();
    buildlcp();
  }

  void buildsa(){
    for(int i=0;i<N;i++){
      sa[i]=i;inv[i]=s[i];
    }

    vector<int> temp(N);
    int gap = 1;
    auto comp = [&] (int i, int j) {
      if(inv[i]!=inv[j]) return inv[i]<inv[j];
      i+=gap,j+=gap;
      if(i>=N||j>=N) return i>j;
      return inv[i]<inv[j];
    };
    for(;;gap*=2){
      sort(sa.begin(),sa.end(), comp);
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
        continue;
      }
      int j=sa[inv[i]+1];
      while(i+k<N && j+k<N && s[i+k]==s[j+k]) k++;
      lcp[inv[i]]=k;
      if(k>0) k--;
    }
  }
};
