pub struct RangeUpdateFenwick {
    m: Fenwick,
    b: Fenwick,
}

impl RangeUpdateFenwick {
    pub fn new(n: usize) -> Self {
        Self {
            m: Fenwick::new(n),
            b: Fenwick::new(n),
        }
    }

    /// Queries sum of elements in the interval [0, i)
    pub fn query(&self, i: usize) -> i64 {
        self.m.query(i) * (i as i64) + self.b.query(i)
    }

    pub fn point_update(&mut self, i: usize, v: i64) {
        self.b.update(i, v)
    }

    /// Adds v to all elements in the interval [i,n)
    pub fn suffix_update(&mut self, i: usize, v: i64) {
        self.m.update(i, v);
        self.b.update(i, -v * (i as i64))
    }

    /// Adds v to all elements in the interval [l,r)
    pub fn range_update(&mut self, l: usize, r: usize, v: i64) {
        self.suffix_update(l, v);
        if r < self.m.a.len() {
            self.suffix_update(r, -v);
        }
    }
}
