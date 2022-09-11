/// Returns x, y such that ax + by = gcd(a, b);
/// Assumes a, b are >= 0
pub fn extended_euclid(a: i64, b: i64) -> (i64, i64, i64) {
    let (mut x1, mut y1, mut z1) = (1i64, 0i64, a);
    let (mut x2, mut y2, mut z2) = (0i64, 1i64, b);
    while z2 > 0 {
        let k = z1 / z2;
        let (x3, y3, z3) = (x1 - k * x2, y1 - k * y2, z1 - k * z2);
        (x1, y1, z1) = (x2, y2, z2);
        (x2, y2, z2) = (x3, y3, z3);
    }
    (x1, y1, z1)
}

/// Finds the inverse of a mod m.
/// Assumes they are relatively prime and m > 0
pub fn mod_inv(mut a: i64, m: i64) -> i64 {
    a %= m;
    if a < 0 {
        a += m;
    }
    let res = extended_euclid(a, m).0 % m;
    if res < 0 {
        res + m
    } else {
        res
    }
}

pub fn gcd(mut a: i64, mut b: i64) -> i64 {
    a = a.abs();
    b = b.abs();
    while b > 0 {
        a %= b;
        (a, b) = (b, a);
    }
    a
}

pub fn lcm(a: i64, b: i64) -> i64 {
    if (a, b) == (0, 0) {
        0
    } else {
        a / gcd(a, b) * b
    }
}

/// Find a number that is a mod n and b mod m.
/// Requires n, m are relatively prime and positive.
pub fn cr_theorem(mut a: i64, n: i64, mut b: i64, m: i64) -> i64 {
    a %= n;
    b %= m;
    let res = (a + (b - a) * mod_inv(n, m) % m * n) % (n * m);
    res + if res < 0 { n * m } else { 0 }
}

#[cfg(test)]
pub mod tests {
    use super::*;

    #[test]
    fn gcd_test() {
        assert_eq!(gcd(666, -148), 74);
    }
    #[test]
    fn extended_euclid_test() {
        assert_eq!(extended_euclid(5, 8), (-3, 2, 1));
        assert_eq!(extended_euclid(666, 148), (1, -4, 74));
    }
    #[test]
    fn mod_inv_test() {
        assert_eq!(mod_inv(-7, 123), 35);
    }
    #[test]
    fn cr_test() {
        assert_eq!(cr_theorem(-1, 5, 20, 6), 14);
    }
}
