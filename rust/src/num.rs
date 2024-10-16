pub fn mod_exp(mut a: i64, mut b: i64, m: i64) -> i64 {
    a %= m;
    if a < 0 {
        a += m;
    }
    let mut res = 1;
    while b > 0 {
        if b % 2 == 1 {
            res = res * a % m;
        }
        a = a * a % m;
        b /= 2;
    }
    res
}

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

/// Factorials mod m. Allows computing binomial coefficients.
pub struct Factorials {
    m: i64,
    f: Vec<i64>,
}

impl Factorials {
    pub fn new(m: i64, max: usize) -> Self {
        let mut f = vec![0; max + 1];
        f[0] = 1;
        for k in 1..=max {
            f[k] = f[k - 1] * k as i64 % m;
        }
        Self { m, f }
    }

    pub fn fac(&self, k: i64) -> i64 {
        if k < 0 || k >= self.f.len() as i64 {
            return 0;
        }
        self.f[k as usize]
    }

    pub fn choose(&self, n: i64, k: i64) -> i64 {
        if k < 0 || k > n {
            return 0;
        }
        let mut res = self.fac(n);
        res = res * mod_inv(self.fac(k), self.m) % self.m;
        res = res * mod_inv(self.fac(n - k), self.m) % self.m;
        res
    }
}

pub fn gcd(mut a: i64, mut b: i64) -> i64 {
    a = a.abs();
    b = b.abs();
    while b > 0 {
        a %= b;
        std::mem::swap(&mut a, &mut b);
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
    fn mod_exp_test() {
        assert_eq!(mod_exp(2, 20, 12341234), 1 << 20);
        assert_eq!(mod_exp(2, 20, 1234), (1 << 20) % 1234);
    }
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
