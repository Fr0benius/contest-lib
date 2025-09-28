use std::ops::{Index, IndexMut};
#[derive(Clone, PartialEq, Eq, PartialOrd, Ord, Hash, Debug)]
pub struct Arr3<T> {
    n1: usize,
    n2: usize,
    n3: usize,
    v: Vec<T>,
}

impl<T> Arr3<T> {
    pub fn from_fn<F: FnMut(usize, usize, usize) -> T>(n1: usize, n2: usize, n3: usize, mut f: F) -> Self {
        let mut v = Vec::with_capacity(n1 * n2 * n3);
        for i in 0..n1 {
            for j in 0..n2 {
                for k in 0..n3 {
                    v.push(f(i, j, k));
                }
            }
        }
        Self { n1, n2, n3, v }
    }
    pub fn from_raw(n1: usize, n2: usize, n3: usize, v: Vec<T>) -> Self {
        assert_eq!(n1 * n2 * n3, v.len());
        Self { n1, n2, n3, v }
    }
    pub fn dims(&self) -> (usize, usize, usize) {
        (self.n1, self.n2, self.n3)
    }
    pub fn iter(&self) -> impl Iterator<Item = &T> {
        self.v.iter()
    }
    pub fn map<U, F: FnMut(T) -> U>(self, mut f: F) -> Arr3<U> {
        let (n1, n2, n3) = self.dims();
        let mut iter = self.into_iter();
        Arr3::from_fn(n1, n2, n3, move |_, _, _| f(iter.next().unwrap()))
    }
    pub fn swap(&mut self, (i0, j0, k0): (usize, usize, usize), (i1, j1, k1): (usize, usize, usize)) {
        self.v.swap(i0 * self.n2 * self.n3 + j0 * self.n3 + k0, i1 * self.n2 * self.n3 + j1 * self.n3 + k1);
    }
}

impl<T: Clone> Arr3<T> {
    pub fn new(n1: usize, n2: usize, n3: usize, x: T) -> Self {
        Self {
            n1,
            n2,
            n3,
            v: vec![x; n1 * n2 * n3],
        }
    }
}

impl<T> Index<(usize, usize, usize)> for Arr3<T> {
    type Output = T;

    fn index(&self, (i, j, k): (usize, usize, usize)) -> &Self::Output {
        &self.v[i * self.n2 * self.n3 + j * self.n3 + k]
    }
}

impl<T> IndexMut<(usize, usize, usize)> for Arr3<T> {
    fn index_mut(&mut self, (i, j, k): (usize, usize, usize)) -> &mut Self::Output {
        &mut self.v[i * self.n2 * self.n3 + j * self.n3 + k]
    }
}

impl<T> IntoIterator for Arr3<T> {
    type Item = T;

    type IntoIter = std::vec::IntoIter<T>;

    fn into_iter(self) -> Self::IntoIter {
        self.v.into_iter()
    }
}

impl<T: Copy> From<Vec<Vec<Vec<T>>>> for Arr3<T> {
    fn from(v: Vec<Vec<Vec<T>>>) -> Self {
        let n1 = v.len();
        assert!(n1 > 0, "Vector cannot be empty");
        let n2 = v[0].len();
        assert!(n2 > 0, "Vector cannot be empty");
        let n3 = v[0].len();
        Self::from_fn(n1, n2, n3, |i, j, k| v[i][j][k])
    }
}
