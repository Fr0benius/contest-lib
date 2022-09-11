pub fn kmp_prefix_func<T: std::cmp::PartialEq>(v: &[T]) -> Vec<usize> {
    let n = v.len();
    let mut pref = vec![0; n];
    for i in 1..n {
        pref[i] = pref[i - 1];
        while pref[i] > 0 && v[pref[i]] != v[i] {
            pref[i] = pref[pref[i] - 1];
        }
        if v[pref[i]] == v[i] {
            pref[i] += 1;
        }
    }
    pref
}

