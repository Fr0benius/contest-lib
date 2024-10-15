pub struct DSURollback {
    par: Vec<usize>,
    size: Vec<i32>,
    comps: usize,
    history: Vec<(usize, usize)>,
}

impl DSURollback {
    pub fn new(n: usize) -> Self {
        Self {
            par: (0..n).collect(),
            size: vec![1; n],
            comps: n,
            history: vec![],
        }
    }
    pub fn root(&mut self, mut a: usize) -> usize {
        while self.par[a] != a {
            a = self.par[a];
        }
        a
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
        self.history.push((b, a));
        true
    }
    pub fn comps(&self) -> usize {
        self.comps
    }
    /// Rolls back the last SUCCESSFUL join
    pub fn rollback(&mut self) {
        if let Some((v, p)) = self.history.pop() {
            self.size[p] -= self.size[v];
            self.comps += 1;
            self.par[v] = v;
            self.par[p] = p;
        }
    }
    #[allow(clippy::len_without_is_empty)]
    pub fn len(&self) -> usize {
        self.par.len()
    }
}
