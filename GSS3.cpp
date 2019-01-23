// Created by proofbycontradiction on 2019-01-23 15:09.
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

  struct node {
    int left;
    int right;
    int total;
    int best;

    node (int x=0): left(x), right(x), total(x), best(x) {}

    friend node operator+(const node &a, const node &b) {
      node c;
      c.total = a.total + b.total;
      c.left = max(a.left, a.total + b.left);
      c.right = max(b.right, b.total + a.right);
      c.best = max(a.best, b.best);
      c.best = max(c.best, max(c.left, c.right));
      c.best = max(c.best, a.right + b.left);
      return c;
    }
  };

  vector<node> tree;
  vi li;
  vi ri;

  int alloc(int value, int L=-1, int R=-1) {
    tree.emplace_back(value);
    li.push_back(L);
    ri.push_back(R);
    return tree.size() - 1;
  }

 public:
  int build(int ll, int rr) {
    if (ll == rr) {
      return alloc(0);
    }
    int mid = (ll + rr) / 2;
    int L =  build(ll, mid);
    int R = build(mid+1, rr);
    return alloc(0, L, R);
  }

  node get(int c, int ll, int rr, int ql, int qr) const {
    if (ql <= ll and rr <= qr) {
      return tree[c];
    }
    int mid = (ll + rr) / 2;
    if (qr <= mid) return get(li[c], ll, mid, ql, qr);
    if (mid < ql) return get(ri[c], mid+1, rr, ql, qr);
    return get(li[c], ll, mid, ql, qr) + get(ri[c], mid+1, rr, ql, qr);
  }

  void modify(int c, int ll, int rr, int p, int val) {
    if (ll == rr) {
      assert(p == ll);
      tree[c] = val;
      return;
    }
    int mid = (ll + rr) / 2;
    if (p <= mid) modify(li[c], ll, mid, p, val);
    if (p > mid) modify(ri[c], mid+1, rr, p, val);
    tree[c] = tree[li[c]] + tree[ri[c]];
  }
};

int32_t main() {
  ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int n; cin >> n;
  segtree st;
  const int root = st.build(0, n-1);
  for (int i = 0; i < n; i++) {
    int x; cin >> x;
    st.modify(root, 0, n-1, i, x);
  }
  int m; cin >> m;
  for (int i = 0; i < m; i++) {
    int code; cin >> code;
    int x, y; cin >> x >> y;
    if (code == 0) {
      st.modify(root, 0, n-1, x-1, y);
    } else {
      cout << st.get(root, 0, n-1, x-1, y-1).best << endl;
    }
  }
}
