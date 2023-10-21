// Taken from https://cp-algorithms.com/string/manacher.html
// The sentinel values should be distinct.
// Can be made more efficient by getting rid of sentinels, but would make the code slightly more
// complicated.

fn manacher<T: PartialEq + Copy>(s: &[T], sentinels: [T; 3]) -> Vec<usize> {
    let mut t = vec![];
    for &c in s {
        t.push(sentinels[2]);
        t.push(c);
    }
    t.push(sentinels[2]);
    let res = manacher_odd(&t, [sentinels[0], sentinels[1]]);
    res[1..res.len() - 1].to_owned()
}

fn manacher_odd<T: PartialEq + Copy>(t: &[T], sentinels: [T; 2]) -> Vec<usize> {
    let n = t.len();
    let mut s = vec![sentinels[0]];
    s.extend_from_slice(t);
    s.push(sentinels[1]);

    let mut p = vec![0; n + 2];
    let mut l = 1;
    let mut r = 1;
    for i in 1..=n {
        p[i] = max(0, min(r - i, p[l + (r - i)]));
        while s[i - p[i]] == s[i + p[i]] {
            p[i] += 1;
        }
        if i + p[i] > r {
            l = i - p[i];
            r = i + p[i];
        }
    }
    p[1..n + 1].to_owned()
}
