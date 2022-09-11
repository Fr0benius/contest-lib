use std::ops::{Index, IndexMut};
#[derive(Clone, PartialEq, Eq, PartialOrd, Ord, Hash, Debug)]
pub struct Arr2<T> {
    n: usize,
    m: usize,
    v: Vec<T>,
}

impl<T> Arr2<T> {
    pub fn from_fn<F: FnMut(usize, usize) -> T>(n: usize, m: usize, mut f: F) -> Self {
        let mut v = Vec::with_capacity(n * m);
        for i in 0..n {
            for j in 0..m {
                v.push(f(i, j));
            }
        }
        Self { n, m, v }
    }
    pub fn from_raw(n: usize, m: usize, v: Vec<T>) -> Self {
        assert_eq!(n * m, v.len());
        Self { n, m, v }
    }

    pub fn dims(&self) -> (usize, usize) {
        (self.n, self.m)
    }

    pub fn iter(&self) -> impl Iterator<Item = &T> {
        self.v.iter()
    }
    pub fn row(&self, i: usize) -> impl Iterator<Item = &T> {
        (0..self.m).map(move |j| &self[i][j])
    }
    pub fn col(&self, j: usize) -> impl Iterator<Item = &T> {
        (0..self.n).map(move |i| &self[i][j])
    }

    pub fn map<U, F: FnMut(T) -> U>(self, mut f: F) -> Arr2<U> {
        let (n, m) = self.dims();
        let mut iter = self.into_iter();
        Arr2::from_fn(n, m, move |_, _| f(iter.next().unwrap()))
    }

    pub fn enumerate(&self) -> impl Iterator<Item = ((usize, usize), &T)> {
        let m = self.dims().1;
        self.iter()
            .enumerate()
            .map(move |(k, x)| ((k / m, k % m), x))
    }
    pub fn swap(&mut self, (i0, j0): (usize, usize), (i1, j1): (usize, usize)) {
        self.v.swap(i0 * self.m + j0, i1 * self.m + j1);
    }
}

impl<T: Clone> Arr2<T> {
    pub fn new(n: usize, m: usize, x: T) -> Self {
        Self {
            n,
            m,
            v: vec![x; n * m],
        }
    }
}

impl<T> Index<usize> for Arr2<T> {
    type Output = [T];

    fn index(&self, i: usize) -> &Self::Output {
        &self.v[i * self.m..(i + 1) * self.m]
    }
}

impl<T> IndexMut<usize> for Arr2<T> {
    fn index_mut(&mut self, i: usize) -> &mut Self::Output {
        &mut self.v[i * self.m..(i + 1) * self.m]
    }
}

impl<T> Index<(usize, usize)> for Arr2<T> {
    type Output = T;

    fn index(&self, (i, j): (usize, usize)) -> &Self::Output {
        &self.v[i * self.m + j]
    }
}

impl<T> IndexMut<(usize, usize)> for Arr2<T> {
    fn index_mut(&mut self, (i, j): (usize, usize)) -> &mut Self::Output {
        &mut self.v[i * self.m + j]
    }
}

impl<T> IntoIterator for Arr2<T> {
    type Item = T;

    type IntoIter = std::vec::IntoIter<T>;

    fn into_iter(self) -> Self::IntoIter {
        self.v.into_iter()
    }
}

impl<T: Clone> From<Vec<Vec<T>>> for Arr2<T> {
    fn from(v: Vec<Vec<T>>) -> Self {
        let n = v.len();
        assert!(n > 0, "Vector cannot be empty");
        let m = v[0].len();
        Self::from_raw(n, m, v.concat())
    }
}
