pub trait LazySegmentTreeNode {
    type Base;
    type Update;
    fn new(x: &Self::Base) -> Self;
    fn id() -> Self;
    fn combine(l: &Self, r: &Self) -> Self;
    fn update(&mut self, u: &Self::Update, l: usize, r: usize);
    fn upd_id() -> Self::Update;
    fn upd_combine(l: &Self::Update, r: &Self::Update) -> Self::Update;
}

#[derive(Debug, Clone)]
pub struct LazySegmentTree<T: LazySegmentTreeNode> {
    n: usize,
    data: Vec<T>,
    updates: Vec<T::Update>,
}

impl<T: LazySegmentTreeNode + Clone> LazySegmentTree<T>
where
    T::Update: Clone,
{
    pub fn new(n: usize) -> Self {
        Self {
            n,
            data: vec![T::id(); 4 * (n + 1)],
            updates: vec![T::upd_id(); 4 * (n + 1)],
        }
    }
    pub fn from_base(arr: &[T::Base]) -> Self {
        let n = arr.len();
        let mut segtree = Self::new(n);
        segtree.build(arr, 1, 0, n);
        segtree
    }
    fn build(&mut self, arr: &[T::Base], ix: usize, l: usize, r: usize) {
        if r == l + 1 {
            self.data[ix] = T::new(&arr[l]);
            return;
        }
        let m = (l + r) / 2;
        self.build(arr, ix * 2, l, m);
        self.build(arr, ix * 2 + 1, m, r);
        self.data[ix] = T::combine(&self.data[2 * ix], &self.data[2 * ix + 1]);
    }

    fn push(&mut self, ix: usize, l: usize, r: usize) {
        if r - l > 1 {
            self.updates[2 * ix] = T::upd_combine(&self.updates[2 * ix], &self.updates[ix]);
            self.updates[2 * ix + 1] = T::upd_combine(&self.updates[2 * ix + 1], &self.updates[ix]);
        }
        self.data[ix].update(&self.updates[ix], l, r);
        self.updates[ix] = T::upd_id();
    }

    pub fn update(&mut self, a: usize, b: usize, u: &T::Update) {
        self.update_impl(a, b, u, 1, 0, self.n)
    }
    fn update_impl(&mut self, a: usize, b: usize, u: &T::Update, ix: usize, l: usize, r: usize) {
        if a <= l && r <= b {
            self.updates[ix] = T::upd_combine(&self.updates[ix], u);
            self.push(ix, l, r);
            return;
        }
        self.push(ix, l, r);
        if a >= r || b <= l {
            return;
        }
        let m = (l + r) / 2;
        self.update_impl(a, b, u, ix * 2, l, m);
        self.update_impl(a, b, u, ix * 2 + 1, m, r);
        self.data[ix] = T::combine(&self.data[ix * 2], &self.data[ix * 2 + 1]);
    }
    pub fn query(&mut self, a: usize, b: usize) -> T {
        self.query_impl(a, b, 1, 0, self.n)
    }
    fn query_impl(&mut self, a: usize, b: usize, ix: usize, l: usize, r: usize) -> T {
        self.push(ix, l, r);
        if a >= r || b <= l {
            return T::id();
        }
        if a <= l && r <= b {
            return self.data[ix].clone();
        }
        let m = (l + r) / 2;
        let left = self.query_impl(a, b, ix * 2, l, m);
        let right = self.query_impl(a, b, ix * 2 + 1, m, r);
        T::combine(&left, &right)
    }
}

#[derive(Clone)]
struct SumSum {
    x: i64,
}
impl LazySegmentTreeNode for SumSum {
    type Base = i64;
    type Update = i64;

    fn new(&x: &Self::Base) -> Self {
        Self { x }
    }

    fn id() -> Self {
        Self { x: 0 }
    }

    fn combine(l: &Self, r: &Self) -> Self {
        Self { x: l.x + r.x }
    }

    fn update(&mut self, u: &Self::Update, l: usize, r: usize) {
        self.x += u * (r - l) as i64;
    }

    fn upd_id() -> Self::Update {
        0
    }

    fn upd_combine(l: &Self::Update, r: &Self::Update) -> Self::Update {
        l + r
    }
}
