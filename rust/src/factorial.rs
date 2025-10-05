pub struct Factorials<const MOD: i32> {
    fac: Vec<ModInt<MOD>>,
    ifac: Vec<ModInt<MOD>>,
}

impl<const MOD: i32> Factorials<MOD> {
    pub fn new(max: usize) -> Self {
        let mut fac = vec![ModInt(0); max + 1];
        fac[0].0 = 1;
        for k in 1..=max {
            fac[k] = fac[k - 1] * ModInt(k as i32);
        }
        let mut ifac = vec![ModInt(0); max + 1];
        ifac[max] = fac[max].inv();
        for k in (1..=max).rev() {
            ifac[k - 1] = ifac[k] * ModInt(k as i32);
        }
        Self { fac, ifac }
    }
    pub fn fac(&self, k: i64) -> ModInt<MOD> {
        if k < 0 || k >= self.fac.len() as i64 {
            return ModInt(0);
        }
        self.fac[k as usize]
    }
    pub fn choose(&self, n: i64, k: i64) -> ModInt<MOD> {
        if k < 0 || k > n {
            return ModInt(0);
        }
        self.fac[n as usize] * self.ifac[k as usize] * self.ifac[(n - k) as usize]
    }
}
