// Dir is 1 or -1 depending on direction.
// To complete the inverse fft, divide all coefficients by N.
// TODO: figure out precision

using cd = complex<double>;
void fft(vector<cd>& a, const int N, const int dir = 1) {
  for (int i = N; i < a.size(); i++) {
    a[i%N] += a[i];
  }
  a.resize(N);

  vector<cd> roots(N/2);
  static const double PI = acos(-1.0);
  roots[0] = 1.0;
  for (int i=1; i<N/2; i++) {
    roots[i] = polar(1.0, PI * 2 * i * dir / N);
  }

  auto rev = [N] (int n) {
    int res = 0;
    for (n += N;n > 1; n >>= 1) {
      res = res * 2 + (n&1); 
    }
    return res;
  };
   
  for (int i=0; i<N; i++) {
    int j = rev(i);
    if (i < j) swap(a[i], a[j]);
  }
  for (int gap = 1; gap < N; gap <<= 1) {
    for (int i = 0; i < N; i += gap*2) {
      for (int j = 0; j < gap; j++) {
        cd root = roots[N/(gap*2)*j];
        tie(a[i+j], a[i+j+gap]) = 
          pair{a[i+j] + a[i+j+gap]*root, a[i+j] - a[i+j+gap]*root};
      }
    }
  }
}

// Recursive version. Much slower so probably best to just use the other one.
void fft_rec(vector<cd>& a, const int N, const int dir = 1) {
  static const double PI = acos(-1.0);
  for (int i = N; i < a.size(); i++) {
    a[i%N] += a[i];
  }
  a.resize(N);
  if (N==1) return;
  vector<cd> l(N/2), r(N/2);
  for(int i=0; i<N/2; i++) {
    l[i] = a[2*i];
    r[i] = a[2*i+1];
  }
  fft_rec(l, N/2, dir);
  fft_rec(r, N/2, dir);
  for (int i = 0; i < N/2; i++) {
    cd root = {cos(PI * 2 * i * dir / N), sin(PI * 2 * i * dir / N)};
    a[i] = l[i] + root * r[i];
    a[i+N/2] = l[i] - root * r[i];
  }
}

vector<ll> mul(const vector<ll>& a, const vector<ll>& b) {
  int A = a.size(), B = b.size();
  int N = 1;
  while (N <= A+B-2) N <<= 1;
  vector<cd> ca(a.begin(), a.end());
  vector<cd> cb(b.begin(), b.end());
  fft(ca, N);
  fft(cb, N);
  for (int i=0; i<N; i++) ca[i] *= cb[i];
  fft(ca, N, -1);
  for (auto& z : ca) z /= N;
  vector<ll> res(N);
  transform(ca.begin(), ca.end(), res.begin(), [](auto z) {
      return llround(z.real());
  });
  return res;
}

