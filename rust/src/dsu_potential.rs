pub trait Group {
    fn id() -> Self;
    fn op(&self, rhs: &Self) -> Self;
    fn inv(&self) -> Self;
}

/// https://judge.yosupo.jp/problem/unionfind_with_potential_non_commutative_group
/// DSU that also keeps track of relationships between elements`
/// rel[x] is a function f such that x = f(par[x])
/// Multiplication is composition (f*g)(x) = f(g(x))
pub struct DSUPotential<T> {
    par: Vec<usize>,
    size: Vec<i32>,
    rel: Vec<T>,
    comps: usize,
}

impl<T> DSUPotential<T>
where
    T: Group + Clone + std::cmp::PartialEq,
{
    pub fn new(n: usize) -> Self {
        Self {
            par: (0..n).collect(),
            size: vec![1; n],
            rel: vec![T::id(); n],
            comps: n,
        }
    }
    pub fn root(&mut self, a: usize) -> (usize, T) {
        if a != self.par[a] {
            let (root, f) = self.root(self.par[a]);
            self.rel[a] = self.rel[a].op(&f);
            self.par[a] = root;
        }
        (self.par[a], self.rel[a].clone())
    }
    // Try joining a, b with the constraint a = f(b)
    // Return true if this is consistent with known relations
    pub fn join(&mut self, a: usize, b: usize, f: &T) -> bool {
        let (mut a, mut fa) = self.root(a);
        let (mut b, mut fb) = self.root(b);
        if a == b {
            return fa == f.op(&fb);
        }
        let mut root_f = fb.inv().op(&f.inv()).op(&fa);
        if self.size[a] < self.size[b] {
            std::mem::swap(&mut a, &mut b);
            std::mem::swap(&mut fa, &mut fb);
            root_f = root_f.inv();
        }
        self.size[a] += self.size[b];
        self.par[b] = a;
        self.rel[b] = root_f;
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
