pub struct Fenwick<T> {
    a: Vec<T>,
}

impl<T: Copy + Default + std::ops::AddAssign> Fenwick<T> {
    pub fn new(n: usize) -> Self {
        Self {
            a: vec![Default::default(); n + 1],
        }
    }

    pub fn update(&mut self, mut i: usize, v: T) {
        i += 1;
        while i < self.a.len() {
            self.a[i] += v;
            i += i & i.wrapping_neg();
        }
    }

    pub fn query(&self, mut i: usize) -> T {
        i = min(i, self.a.len() - 1);
        let mut res: T = Default::default();
        while i != 0 {
            res += self.a[i];
            i -= i & i.wrapping_neg();
        }
        res
    }
}

impl<T: Copy + Default + std::ops::SubAssign + PartialOrd> Fenwick<T> {
    /// Returns lowest k such that query(k) >= v.
    /// If no such k, returns n + 1.
    /// Only works if all elements are nonnegative.
    pub fn lower_bound(&self, mut v: T) -> usize {
        if v <= Default::default() {
            return 0;
        }
        let n = self.a.len() - 1;
        let mut k = 0;
        // TODO: start at the log_2(n)'th bit
        for j in (0..31).rev() {
            let nxt = k + (1 << j);
            if nxt <= n && self.a[nxt] < v {
                k = nxt;
                v -= self.a[nxt];
            }
        }
        k + 1
    }
}
