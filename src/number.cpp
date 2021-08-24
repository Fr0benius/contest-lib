const ll MOD = 1e9+7;

ll pw(ll a, ll b) {
  ll res = 1;
  a %= MOD;
  for (; b; b /= 2) {
    if (b%2) res = res*a%MOD;
    a=a*a%MOD;
  }
  return res;
}

ll inv(ll a) {
  return pw(a, MOD-2);
}

const int MAX = 200200;
ll fac[MAX], ifac[MAX];
void init_fac() {
  fac[0] = 1;
  for (int i = 1; i < MAX; i++) fac[i] = fac[i-1] * i % MOD;
  ifac[MAX-1] = inv(fac[MAX-1]);
  for (int i = MAX-2; i >= 0; i--) ifac[i] = ifac[i+1] * (i+1) % MOD;
}

ll choose(ll a, ll b) {
  if (b < 0 || b > a) return 0LL;
  return fac[a] * ifac[b] % MOD * ifac[a-b] % MOD;
}

// returns vector of lowest prime divisors for [2, M]
vector<int> sieve(int M) {
  vector<int> low(M+1);
  vector<int> primes;
  for (int n = 2; n <= M; n++) {
    if (low[n] == 0) low[n] = n, primes.pb(n);
    for (int p : primes) {
      if (p*n > M || p > low[n]) break;
      low[p*n] = p;
    }
  }
  return low;
}
