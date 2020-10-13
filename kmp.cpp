// Returns the prefix function of s.
// s can be a string or vector.
template<typename T>
vector<int> kmp(const T& s) {
  int N = s.size();
  vector<int> res(N);
  for (int i=1; i<N; i++) {
    res[i] = res[i-1];
    while (res[i] > 0 && s[res[i]] != s[i])
      res[i] = res[res[i]-1];
    if (s[res[i]] == s[i]) res[i]++;
  }
  return res;
}
