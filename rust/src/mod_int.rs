#[derive(Debug, Clone, Copy, PartialEq)]
pub struct ModInt<const M: i32>(i32);

impl<const M: i32> ModInt<M> {
    pub fn new(x: i64) -> Self {
        let mut x = (x % M as i64) as i32;
        if x < 0 {
            x += M;
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
    /// Assumes M is prime and number is nonzero
    pub fn inv(self) -> Self {
        self.pow((M - 2) as i64)
    }
}

impl<const M: i32> std::ops::AddAssign for ModInt<M> {
    fn add_assign(&mut self, rhs: Self) {
        self.0 += rhs.0;
        if self.0 >= M {
            self.0 -= M;
        }
    }
}

impl<const M: i32> std::ops::Add for ModInt<M> {
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        let mut res = self;
        res += rhs;
        res
    }
}

impl<const M: i32> std::ops::MulAssign for ModInt<M> {
    fn mul_assign(&mut self, rhs: Self) {
        self.0 = ((self.0 as i64 * rhs.0 as i64) % M as i64) as i32
    }
}
impl<const M: i32> std::ops::Mul for ModInt<M> {
    type Output = Self;

    fn mul(self, rhs: Self) -> Self::Output {
        let mut res = self;
        res *= rhs;
        res
    }
}

impl<const M: i32> std::ops::Neg for ModInt<M> {
    type Output = Self;

    fn neg(self) -> Self::Output {
        let mut res = M - self.0;
        if res >= M {
            res -= M
        }
        Self(res)
    }
}
