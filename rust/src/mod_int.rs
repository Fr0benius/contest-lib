const MOD: i32 = 998_244_353;

#[derive(Debug, Clone, Copy, PartialEq)]
pub struct ModInt(i32);

impl ModInt {
    pub fn new(x: i64) -> Self {
        let mut x = (x % MOD as i64) as i32;
        if x < 0 {
            x += MOD;
        }
        Self(x)
    }

    pub fn pow(self, mut b: i64) -> Self {
        let mut a = self;
        let mut res = Self(1);
        while b > 0 {
            if b % 2 == 1 {
                res = res * a;
            }
            a = a * a;
            b /= 2;
        }
        res
    }

    /// Modular inverse
    /// Assumes MOD is prime and number is nonzero
    pub fn inv(self) -> Self {
        self.pow((MOD - 2) as i64)
    }
}

impl std::ops::Add for ModInt {
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        let mut res = self.0 + rhs.0;
        if res >= MOD {
            res -= MOD
        }
        Self(res)
    }

}

impl std::ops::Mul for ModInt {
    type Output = Self;

    fn mul(self, rhs: Self) -> Self::Output {
        Self(((self.0 as i64 * rhs.0 as i64) % MOD as i64) as i32)
    }
}

impl std::ops::Neg for ModInt {
    type Output = Self;

    fn neg(self) -> Self::Output {
        let mut res = MOD - self.0;
        if res >= MOD {
            res -= MOD
        }
        Self(res)
    }
}
