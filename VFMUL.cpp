// Created by proofbycontradiction on 2019-01-23 14:50.
#include <bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

template<class T, size_t n> ostream &operator<<(ostream &os, array<T, n> V) {
  os << "["; for (auto vv : V) os << vv << ","; return os << "]";
}
template<class L, class R> ostream &operator<<(ostream &os, pair<L,R> P) {
  return os << "(" << P.first << "," << P.second << ")";
}
template<class T> ostream &operator<<(ostream &os, set<T> V) {
  os << "["; for (auto vv : V) os << vv << ","; return os << "]";
}
template<class T> ostream &operator<<(ostream& os, vector<T> V) {
  os << "["; for (auto vv : V) os << vv << ","; return os << "]";
}
template<class K, class X> ostream &operator<<(ostream& os, map<K,X> V) {
  os << "["; for (auto vv : V) os << vv << ","; return os << "]";
}

void debug_out() { cerr << endl; }

template <typename Head, typename... Tail>
void debug_out(Head H, Tail...T) { cerr << " " << H; debug_out(T...); }

#ifdef LOCAL
#define debug(...) cerr << "[" << #__VA_ARGS__ << "]:", debug_out(__VA_ARGS__)
#else
#define debug(...) 42
#endif

using ii = pair<int, int>;
using vb = vector<bool>;
using vi = vector<int>;
using vii = vector<ii>;
using vvi = vector<vi>;
using vvii = vector<vii>;

const long double PI = acosl(-1);

class fourier {
  using cd = std::complex<long double>;

  static void fft(vector<cd> & a, bool invert) {
    const int n = a.size();

     for (int i = 1, j = 0; i < n; i++) {
       int bit = n >> 1;
       for (; j & bit; bit >>= 1) {
         j ^= bit;
       }
       j ^= bit;
       if (i < j)
         swap(a[i], a[j]);
     }

     for (int len = 2; len <= n; len <<= 1) {
       long double ang = (2L * PI / len) * (invert ? -1 : 1);
       cd wlen(cosl(ang), sinl(ang));
       for (int i = 0; i < n; i += len) {
         cd w(1, 0);
         for (int j = 0; j < len / 2; j++) {
           cd u = a[i + j];
           cd t = a[i + j + len/2] * w;
           a[i + j] = u + t;
           a[i + j + len/2] = u - t;
           w *= wlen;
         }
       }
     }

     if (invert) {
       for (auto &x: a) {
         x /= n;
       }
     }
  }

 public:
  static vi multiply(const vi &a, const vi &b) {
    vector<cd> fa(a.begin(), a.end());
    vector<cd> fb(b.begin(), b.end());
    int n = 1;
    while (n < (int)fa.size() + (int)fb.size()) {
      n <<= 1;
    }
    fa.resize(n);
    fb.resize(n);
    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++) {
      fa[i] *= fb[i];
    }
    fft(fa, true);
    vi ans(n);
    for (int i = 0; i < n; i++) {
      ans[i] = std::roundl(fa[i].real());
    }
    return ans;
  }
};

int32_t main() {
  ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int t; cin >> t;
  while (t--) {
    string num1, num2; cin >> num1 >> num2;
    vi a, b;
    std::reverse(num1.begin(), num1.end());
    std::reverse(num2.begin(), num2.end());
    for (char ch: num1) {
      a.push_back(ch - '0');
    }
    for (char ch: num2) {
      b.push_back(ch- '0');
    }
    vi res = fourier::multiply(a, b);

    int carry = 0;
    for (int i = 0; i < (int)res.size(); i++) {
      res[i] += carry;
      carry = res[i] / 10;
      res[i] %= 10;
    }

    for (int i = res.size() - 1; i > 0; i--) {
      if (res[i] == 0) {
        res.pop_back();
      } else {
        break;
      }
    }
    std::string ans;
    for (int num: res) {
      ans += to_string(num);
    }
    std::reverse(ans.begin(), ans.end());
    cout << ans << endl;
  }
}
