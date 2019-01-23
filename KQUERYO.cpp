// Created by proofbycontradiction on 2019-01-22 17:11.
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

class segtree {
 public:
  vector<int> tree;
  vector<int> li;
  vector<int> ri;

  inline int alloc(int value, int L=-1, int R=-1) {
    tree.push_back(value);
    li.push_back(L);
    ri.push_back(R);
    return tree.size() - 1;
  }

  int build(int ll, int rr) {
    if (ll == rr) {
      return alloc(0);
    }
    int mid = (ll + rr) / 2;
    int L = build(ll, mid);
    int R = build(mid+1, rr);
    return alloc(tree[L] + tree[R], L, R);
  }

  int get(int c, int ll, int rr, int ql, int qr) const {
    if (ql <= ll and rr <= qr) {
      return tree[c];
    }
    int mid = (ll + rr) / 2;
    if (qr <= mid) return get(li[c], ll, mid, ql, qr);
    if (mid < ql) return get(ri[c], mid+1, rr, ql, qr);
    return get(li[c], ll, mid, ql, qr) + get(ri[c], mid+1, rr, ql, qr);
  };

  int modify(int c, int ll, int rr, int p) {
    if (ll == p and rr == p) {
      return alloc(1);
    }
    int mid = (ll + rr) / 2;
    int L = (p <= mid) ? modify(li[c], ll, mid, p) : li[c];
    int R = (p > mid) ? modify(ri[c], mid+1, rr, p) : ri[c]; 
    return alloc(tree[L] + tree[R], L, R);
  }
};

struct point {
  int value;
  int index;
  int root;

  inline auto operator<(const point &b) const {
    return value < b.value;
  }

};

auto operator<<(ostream &os, const point &a) {
  os << "(" << a.value << ", " << a.index << ", " << a.root << ")";
}

const int nn = 6e5;
segtree st;

struct Compare {
  bool operator()(const int x, const point &p) {
    return x < p.value;
  }
} cmp;


int32_t main() {
  ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int n; cin >> n;
  vector<point> pv(n);
  for (int i = 0; i < n; i++) {
    cin >> pv[i].value;
    pv[i].index = i;
    pv[i].root = -1;
  }
  sort(pv.rbegin(), pv.rend());
  // save
  st.tree.reserve(nn);
  st.li.reserve(nn);
  st.ri.reserve(nn);

  int base = st.build(0, n-1);

  for (int i = 0; i < (int)pv.size(); i++) {
    pv[i].root = st.modify(base, 0, n-1, pv[i].index);
    base = pv[i].root;
  }
  sort(pv.begin(), pv.end());

  int m; cin >> m;
  int last_ans = 0;
  for (int i = 0; i < m; i++) {
    int ll, rr, k; cin >> ll >> rr >> k;
    ll ^= last_ans;
    rr ^= last_ans;
    k ^= last_ans;
    ll--;
    rr--;
    ll = max(ll, 0LL);
    rr = min(rr, n - 1);
    if (ll > rr or pv[n-1].value < k) {
      last_ans = 0;
      cout << last_ans << endl;
      continue;
    }
    int it = (int)(upper_bound(pv.begin(), pv.end(), k, cmp) - pv.begin());
    last_ans = st.get(pv[it].root, 0, n-1, ll, rr);
    cout << last_ans << endl;
  }
}
