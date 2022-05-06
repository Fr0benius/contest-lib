#include <iostream>
#include <functional>
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
#include <complex>
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

#define dbg(...) cerr << "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
#else
#define dbg(...)
#endif


template<class Fun>
class _y_combinator_result {
    Fun fun_;
public:
    template<class T>
    explicit _y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

    template<class ...Args>
    decltype(auto) operator()(Args &&...args) {
        return fun_(std::ref(*this), std::forward<Args>(args)...);
    }
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
    return _y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll, ll>;
template<typename T> using min_queue=priority_queue<T,vector<T>,greater<T>>;

// END OF TEMPLATE

int main() {
  ios::sync_with_stdio(0);cin.tie(0);

}
