pub struct DSU {
    par: Vec<usize>,
    size: Vec<i32>,
    comps: usize,
}

impl DSU {
    pub fn new(n: usize) -> Self {
        Self {
            par: (0..n).collect(),
            size: vec![1; n],
            comps: n,
        }
    }
    pub fn root(&mut self, a: usize) -> usize {
        if a != self.par[a] {
            self.par[a] = self.root(self.par[a]);
        }
        self.par[a]
    }
    pub fn join(&mut self, mut a: usize, mut b: usize) -> bool {
        a = self.root(a);
        b = self.root(b);
        if a == b {
            return false;
        }
        if self.size[a] < self.size[b] {
            std::mem::swap(&mut a, &mut b);
        }
        self.size[a] += self.size[b];
        self.par[b] = a;
        self.comps -= 1;
        true
    }
    pub fn comps(&self) -> usize {
        self.comps
    }
    #[allow(clippy::len_without_is_empty)]
    pub fn len(&self) -> usize {
        self.par.len()
    }
}
