use std::iter::successors;

pub fn next_permutation<T: Ord>(p: &mut [T]) -> bool {
    let n = p.len();
    if let Some(i) = (0..n - 1).rev().find(|&i| p[i] < p[i + 1]) {
        let j = (i + 1..n).rev().find(|&j| p[i] < p[j]).unwrap();
        p.swap(i, j);
        p[i + 1..].reverse();
        true
    } else {
        p.reverse();
        false
    }
}

pub fn all_permutations<T: Ord + Clone>(p: &[T]) -> impl Iterator<Item = Vec<T>> {
    successors(Some(p.to_vec()), |op| {
        let mut nxt = op.clone();
        if next_permutation(&mut nxt) {
            Some(nxt)
        } else {
            None
        }
    })
}
