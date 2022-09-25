pub trait SegmentTreeNode {
    type Base;
    type Update;
    fn new(x: &Self::Base) -> Self;
    fn id() -> Self;
    fn combine(l: &Self, r: &Self) -> Self;
    fn update(&mut self, u: Self::Update);
}

#[derive(Debug, Clone)]
pub struct SegmentTree<T> {
    n: usize,
    data: Vec<T>,
}

impl<T: SegmentTreeNode + Clone> SegmentTree<T> {
    pub fn new(n: usize) -> Self {
        Self {
            n,
            data: vec![T::id(); 4 * (n + 1)],
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
        self.data[ix] = T::combine(&self.data[ix * 2], &self.data[ix * 2 + 1]);
    }

    pub fn update(&mut self, a: usize, u: T::Update) {
        self.update_impl(a, u, 1, 0, self.n)
    }
    fn update_impl(&mut self, a: usize, u: T::Update, ix: usize, l: usize, r: usize) {
        if r == l + 1 {
            self.data[ix].update(u);
            return;
        }
        let m = (l + r) / 2;
        if a < m {
            self.update_impl(a, u, ix * 2, l, m);
        } else {
            self.update_impl(a, u, ix * 2 + 1, m, r);
        }
        self.data[ix] = T::combine(&self.data[ix * 2], &self.data[ix * 2 + 1]);
    }
    pub fn query(&self, a: usize, b: usize) -> T {
        self.query_impl(a, b, 1, 0, self.n)
    }
    fn query_impl(&self, a: usize, b: usize, ix: usize, l: usize, r: usize) -> T {
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

