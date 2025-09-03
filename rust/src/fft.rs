#[derive(Clone, Copy, Debug, PartialEq, Default)]
pub struct Complex(f64, f64);

impl std::ops::Add for Complex {
    type Output = Self;
    fn add(self, rhs: Self) -> Self::Output {
        Self(self.0 + rhs.0, self.1 + rhs.1)
    }
}
impl std::ops::AddAssign for Complex {
    fn add_assign(&mut self, rhs: Self) {
        *self = *self + rhs;
    }
}

impl std::ops::Sub for Complex {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self::Output {
        Self(self.0 - rhs.0, self.1 - rhs.1)
    }
}
impl std::ops::SubAssign for Complex {
    fn sub_assign(&mut self, rhs: Self) {
        *self = *self - rhs;
    }
}

impl std::ops::Mul for Complex {
    type Output = Self;
    fn mul(self, rhs: Self) -> Self::Output {
        Self(
            self.0 * rhs.0 - self.1 * rhs.1,
            self.0 * rhs.1 + self.1 * rhs.0,
        )
    }
}

impl std::ops::MulAssign for Complex {
    fn mul_assign(&mut self, rhs: Self) {
        *self = *self * rhs;
    }
}

impl Into<Complex> for f64 {
    fn into(self) -> Complex {
        Complex(self, 0.0)
    }
}

impl Into<Complex> for i64 {
    fn into(self) -> Complex {
        (self as f64).into()
    }
}

/// Takes a polynomial with complex coefficients and computes its values at the nth roots of unity.
/// Requires that the length is a power of 2.
pub fn classic_fft_inplace(a: &mut [Complex], invert: bool) {
    let n = a.len();
    debug_assert!(n.is_power_of_two());
    // Rearrange in reverse bit order
    // At each step, i and j are reverses of each other
    {
        let mut j = 0;
        for i in 1..n {
            let mut b = n >> 1;
            while j & b != 0 {
                j ^= b;
                b >>= 1;
            }
            j ^= b;
            if i < j {
                a.swap(i, j);
            }
        }
    }
    let mut len = 2;
    let pi = f64::acos(-1.0);
    let sign = if invert { -1.0 } else { 1.0 };
    while len <= n {
        for i in 0..n / len {
            for j in 0..len / 2 {
                let arg = 2.0 * j as f64 * sign * pi / len as f64;
                let z = Complex(arg.cos(), arg.sin());
                let i1 = i * len + j;
                let i2 = i * len + j + len / 2;
                (a[i1], a[i2]) = (a[i1] + a[i2] * z, a[i1] - a[i2] * z);
            }
        }
        len <<= 1;
    }
    if invert {
        for x in a {
            x.0 /= n as f64;
            x.1 /= n as f64;
        }
    }
}

/// Multiplies the polynomials a and b using classic fft.
pub fn convolution(a: &[i64], b: &[i64]) -> Vec<i64> {
    let n = (a.len() + b.len() - 1).next_power_of_two();
    let complexify = |v: &[i64]| {
        let mut cv: Vec<Complex> = Vec::with_capacity(n);
        cv.extend(v.into_iter().map(|&k| -> Complex { k.into() }));
        cv.resize(n, Default::default());
        cv
    };
    let mut ca = complexify(a);
    let mut cb = complexify(b);
    classic_fft_inplace(&mut ca, false);
    classic_fft_inplace(&mut cb, false);
    for i in 0..n {
        ca[i] *= cb[i];
    }
    classic_fft_inplace(&mut ca, true);
    // let mut error = 0.0f64;
    let mut res = vec![0; a.len() + b.len() - 1];
    for i in 0..res.len() {
        res[i] = ca[i].0.round() as i64;
        // error = error.max((ca[i].0 - ca[i].0.round()).abs());
    }
    // dbg!(error);
    res
}
