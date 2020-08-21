#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <climits>
#include <cmath>
#include <cstring>
#include <cassert>
#include <chrono>
#include <random>
#include <bitset>
#include <unordered_set>
#include <unordered_map>
using namespace std;

#define pb push_back
#define fst first
#define snd second

#ifdef QLEG_DEBUG
template<typename T> ostream& operator<<(ostream& os, const vector<T>& v) { os << '{'; string sep; for (const auto& x : v) os << sep << x, sep = ", "; return os << '}'; }
template<typename A, typename B> ostream& operator<<(ostream& os, const pair<A, B>& p) { return os << '(' << p.first << ", " << p.second << ')'; }
void dbg_out() { cerr << endl; }
template<typename Head, typename... Tail> void dbg_out(Head H, Tail... T) { cerr << ' ' << H; dbg_out(T...); }

#define dbg(...) cerr << "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__), cerr << endl;
#else
#define dbg(...)
#endif

using ll = long long;
using pii = pair<int,int>;
template<typename T> using min_queue=priority_queue<T,vector<T>,greater<T>>;

int main(){
  ios::sync_with_stdio(0);cin.tie(0);

}
