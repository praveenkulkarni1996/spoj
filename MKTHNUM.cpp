// Created by proofbycontradiction on 2019-01-22 15:01.
// https://www.spoj.com/problems/MKTHNUM/en/
#include <bits/stdc++.h>
using namespace std;

class segtree {
public:

  vector<int> li;
  vector<int> ri;
  vector<int> tree;

  inline int alloc(int val, int lval=-1, int rval=-1) {
    tree.push_back(val);
    li.push_back(lval);
    ri.push_back(rval);
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
  }

  int modify(const int c, const int ll, const int rr, const int p) {
    if (ll == p and rr == p) {
      return alloc(1);
    };
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

int32_t main() {
  ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int n, m; cin >> n >> m;
  vector<point> pv(n);
  for (int i = 0; i < n; i++) {
    cin >> pv[i].value;
    pv[i].index = i;
    pv[i].root = -1;
  }
  sort(pv.begin(), pv.end());
  segtree st;
  int base = st.build(0, n-1);
  for (int i = 0; i < n; i++) {
    pv[i].root = st.modify(base, 0, n - 1, pv[i].index);
    base = pv[i].root;
  }
  for (int i = 0; i < m; i++) {
    int ll, rr, k; cin >> ll >> rr >> k;
    ll--; rr--;
    int lo = 0, hi = n - 1;
    if (rr - ll + 1 < k) {
      cout << -1 << "\n";
      continue;
    }
    while (lo < hi) {
      int mid = (lo + hi) / 2;
      int less = st.get(pv[mid].root, 0, n-1, ll, rr);
      if (less < k) {
        lo = mid + 1;
      } else {
        hi = mid;
      }
    }
    cout << pv[lo].value << "\n";
  }
}
