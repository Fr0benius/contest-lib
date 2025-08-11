pub trait SegmentTreeElem: Clone {
    fn identity() -> Self;
    fn op(&self, other: &Self) -> Self;

    type Update;
    fn identity_update() -> Self::Update;
    /// composition of updates: rhs is applied first
    fn op_update(lhs: &Self::Update, rhs: &Self::Update) -> Self::Update;
    /// Applies an update to the current element.
    /// "size" is the length of the interval corresponding to the node.
    /// It is useful for cases like assign update/sum query, so we don't have to keep the length in
    /// the node itself.
    fn apply(&self, update: &Self::Update, size: i64) -> Self;
}

/// An Al.Cash-style segment tree (https://codeforces.com/blog/entry/18051)
/// Allows both memory-optimized (exactly 2n nodes) and unoptimized (2 * ceiling power of 2)
/// versions.
/// The latter is needed if we want an efficient binary search.
/// Even the "unoptimized" version uses less memory than a classic recursive tree.
pub struct FastSegmentTree<T: SegmentTreeElem> {
    elems: Vec<T>,
    updates: Vec<T::Update>,
}

// TODO: implement ceiling-power-of-2 version
impl<T: SegmentTreeElem> FastSegmentTree<T>
where
    T::Update: Clone,
{
    pub fn new(n: usize) -> Self {
        Self {
            elems: vec![T::identity(); 2 * n],
            updates: vec![T::identity_update(); n],
        }
    }

    pub fn from_fn(n: usize, f: impl Fn(usize) -> T) -> Self {
        let mut elems = vec![T::identity(); 2 * n];
        for i in 0..n {
            elems[n + i] = f(i);
        }
        for i in (1..n).rev() {
            elems[i] = elems[i * 2].op(&elems[i * 2 + 1]);
        }
        Self {
            elems,
            updates: vec![T::identity_update(); n],
        }
    }

    /// Applies an update to a node. Only called when parents nodes' updates have been fully
    /// pushed.
    fn apply(&mut self, idx: usize, update: &T::Update, size: i64) {
        self.elems[idx] = self.elems[idx].apply(update, size);
        if idx < self.updates.len() {
            self.updates[idx] = T::op_update(update, &self.updates[idx])
        }
    }

    /// Push lazy updates for all ancestors of a node, starting at the root.
    fn push(&mut self, idx: usize) {
        let n = self.updates.len();
        let highest_pow2 = (n + 1).next_power_of_two() >> 1;
        let height = highest_pow2.trailing_zeros();
        for b in (1..=height).rev() {
            let k = idx >> b;
            let update = std::mem::replace(&mut self.updates[k], T::identity_update());
            self.apply(k * 2, &update, 1 << (b - 1));
            self.apply(k * 2 + 1, &update, 1 << (b - 1));
        }
    }

    /// Sets an element to the combination of its child elements. Assumes that the element at idx
    /// is fully pushed.
    fn combine(&mut self, idx: usize) {
        self.elems[idx] = self.elems[idx * 2].op(&self.elems[idx * 2 + 1]);
    }

    /// Applies the update to the interval [l, r).
    /// Requires that 0 <= l, r <= n.
    /// No-op if r <= l.
    pub fn update(&mut self, mut l: usize, mut r: usize, update: &T::Update) {
        let n = self.updates.len();
        l += n;
        r += n;
        self.push(l);
        if r - 1 > l {
            self.push(r - 1);
        }
        let mut l0 = 1;
        let mut r0 = 1;
        let mut size = 1;
        while l < r {
            if l & 1 == 1 {
                l0 = max(l0, l);
                self.apply(l, update, size);
                l += 1;
            }
            if r & 1 == 1 {
                r -= 1;
                r0 = max(r0, r);
                self.apply(r, update, size);
            }
            size <<= 1;
            l >>= 1;
            r >>= 1;
        }
        while l0 > 1 {
            l0 >>= 1;
            self.combine(l0);
        }
        while r0 > 1 {
            r0 >>= 1;
            self.combine(r0);
        }
    }

    /// Queries the interval [l, r).
    /// Requires that 0 <= l, r <= n.
    /// Returns the identity if r <= l.
    pub fn query(&mut self, mut l: usize, mut r: usize) -> T {
        let n = self.updates.len();
        l += n;
        r += n;
        self.push(l);
        if r - 1 > l {
            self.push(r - 1);
        }
        let mut l_res = T::identity();
        let mut r_res = T::identity();
        while l < r {
            if l & 1 == 1 {
                l_res = l_res.op(&self.elems[l]);
                l += 1;
            }
            if r & 1 == 1 {
                r -= 1;
                r_res = self.elems[r].op(&r_res);
            }
            l >>= 1;
            r >>= 1;
        }
        l_res.op(&r_res)
    }
}
