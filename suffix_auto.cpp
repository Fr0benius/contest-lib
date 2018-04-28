#include <bits/stdc++.h>
using namespace std;

struct suffix_auto{
  struct node{
    map<char,int> next;
    int link;
    int len;
  };

  int last;
  vector<node> nodes;

  void init(){
    nodes.push_back({});
    nodes[0].link=-1;
    nodes[0].len=0;
    last=0;
  }

  void extend(char c){
    int cur=nodes.size();
    nodes.push_back({});
    nodes[cur].len=nodes[last].len+1;

    int tmp=last;
    while(tmp!=-1 && !nodes[tmp].next.count(c)){
      nodes[tmp].next[c]=cur;
      tmp=nodes[tmp].link;
    }

    if(tmp==-1){
      nodes[cur].link=0;
    }else{
      int nxt=nodes[tmp].next[c];
      if(nodes[nxt].len==nodes[tmp].len+1) st[cur].link=nxt;
      else{
	int cpy=nodes.size();
	nodes.push_back({});
	nodes[cpy].next=nodes[nxt].next;
	nodes[cpy].link=nodes[nxt].link;
	nodes[cpy].len=nodes[tmp].len+1;
	while(tmp!=-1 && !nodes[tmp].next.count(c)){
	  nodes[tmp].next[c]=cpy;
	  tmp=nodes[tmp].link;
	}
	nodes[nxt].link=cpy;
	nodes[cur].link=cpy;
      }
    }
    
    last=cur;
  }


};
