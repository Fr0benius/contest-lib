// Returns the prefix function of s.
// s can be any indexable structure (array, vector, string, etc)
template<typename V>
vector<int> kmp(const V& s) {
  int N = size(s);
  vector<int> res(N);
  for (int i=1; i<N; i++) {
    res[i] = res[i-1];
    while (res[i] > 0 && s[res[i]] != s[i])
      res[i] = res[res[i]-1];
    if (s[res[i]] == s[i]) res[i]++;
  }
  return res;
}

template<typename V>
vector<int> zfunc(const V& s) {
  int N = size(s);
  vector<int> z(N);
  int l = 0, r = 1;
  for (int i = 1; i < N; i++) {
    if (i < r)  
      z[i] = min(z[i-l], r - i);
    while (i+z[i] < N && s[z[i]] == s[i+z[i]])
      z[i]++;
    if (i+z[i] > r)
      l = i, r = i+z[i];
  }
  return z;
}

