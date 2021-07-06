struct implicit_treap {
  mt19937_64 rng;
  struct node {
    ll priority;
    ll val;
    int sz = 1;
    node* l = nullptr;
    node* r = nullptr;
  };

  static int sz(const node* nd) {
    return nd ? nd->sz : 0;
  }

  void fix(node* a) {
    if (a) {
      a->sz = sz(a->l) + sz(a->r) + 1;
    }
  }

  node* merge(node* a, node* b) {
    if (!a) return b;
    if (!b) return a;
    node* res = nullptr;
    if (a->priority > b->priority) {
      a->r = merge(a->r, b);
      res = a;
    } else {
      b->l = merge(a, b->l);
      res = b;
    }
    fix(res);
    return res;
  }

  // The left split will have n elements
  pair<node*, node*> split(node* a, int n) {
    if (!a) return {a, a};
    if (n == 0) {
      return {nullptr, a};
    }
    if (sz(a->l) >= n) {
      auto [l, r] = split(a->l, n);
      a->l = nullptr;
      fix(a);
      return {l, merge(r, a)};
    } else {
      auto [l, r] = split(a->r, n - sz(a->l) - 1);
      a->r = nullptr;
      fix(a);
      return {merge(a, l), r};
    }
  }

  node* new_node(ll val) {
    return new node{(ll)rng(), val};
  }

  implicit_treap(ll seed = -1) : rng(chrono::steady_clock::now().time_since_epoch().count()) {
    if (seed != -1) {
      rng.seed(seed);
    }
  }
};
