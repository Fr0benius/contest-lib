// Taken from https://cp-algorithms.com/string/manacher.html
// The sentinel values should be distinct.
// Can be made more efficient by getting rid of sentinels, but would make the code slightly more
// complicated.

fn manacher<T: PartialEq + Copy>(s: &[T], sentinels: [T; 3]) -> Vec<usize> {
    let mut t = vec![sentinels[0]];

    for &c in s {
        t.push(sentinels[2]);
        t.push(c);
    }
    t.extend([sentinels[2], sentinels[1]]);
    let n = t.len();
    let mut p = vec![0; n];
    let mut l = 1;
    let mut r = 1;
    for i in 1..=n - 2 {
        p[i] = max(0, min(r - i, p[l + (r - i)]));
        while t[i - p[i]] == t[i + p[i]] {
            p[i] += 1;
        }
        if i + p[i] > r {
            l = i - p[i];
            r = i + p[i];
        }
        p[i] -= 1;
    }
    p[2..n - 2].to_owned()
}

