use std::collections::HashMap;
use std::hash::Hash;
use std::cmp::Eq;

pub trait Counter<T>: Iterator<Item = T> {
    fn counter(self) -> HashMap<T, usize>;
}

impl<T: Hash + Eq, I: Iterator<Item = T>> Counter<T> for I {
    fn counter(self) -> HashMap<T, usize> {
        let mut res = HashMap::new();
        for t in self {
            res.entry(t).and_modify(|c| *c += 1).or_insert(1);
        }
        res
    }
}
