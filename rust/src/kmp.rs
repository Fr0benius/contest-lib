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

pub fn z_func<T: std::cmp::PartialEq>(v: &[T]) -> Vec<usize> {
    let n = v.len();
    let mut z = vec![0; n];
    z[0] = n;
    let mut l = 0;
    let mut r = 0;
    for i in 1..n {
        if i < r {
            z[i] = z[i - l].min(r - i);
        }
        while i + z[i] < n && v[z[i]] == v[i + z[i]] {
            z[i] += 1;
        }
        if i + z[i] > r {
            l = i;
            r = i + z[i];
        }
    }
    z
}
