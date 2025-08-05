pub struct Fenwick {
    a: Vec<i64>,
}

impl Fenwick {
    pub fn new(n: usize) -> Self {
        Self { a: vec![0; n + 1] }
    }

    pub fn update(&mut self, mut i: usize, v: i64) {
        i += 1;
        while i < self.a.len() {
            self.a[i] += v;
            i += i & i.wrapping_neg();
        }
    }

    pub fn query(&self, mut i: usize) -> i64 {
        i = min(i, self.a.len() - 1);
        let mut res = 0;
        while i != 0 {
            res += self.a[i];
            i -= i & i.wrapping_neg();
        }
        res
    }

    /// Returns lowest k such that query(k) >= v.
    /// If no such k, returns n + 1.
    /// Only works if all elements are nonnegative.
    pub fn lower_bound(&self, mut v: i64) -> usize {
        if v <= 0 {
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
