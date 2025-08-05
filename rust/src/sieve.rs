pub struct Sieve {
    pub primes: Vec<i64>,
    lowest: Vec<i64>,
}

impl Sieve {
    /// Generate all primes smaller than m, as well as a smallest-prime-divisor array.
    pub fn new(m: usize) -> Self {
        let mut lowest = vec![0; m];
        let mut primes = vec![];
        for k in 2..m {
            if lowest[k] == 0 {
                lowest[k] = k as _;
                primes.push(k as _);
            }
            for &p in &primes {
                let n = k * p as usize;
                if n >= m || p > lowest[k] {
                    break;
                }
                lowest[n] = p;
            }
        }
        Self { primes, lowest }
    }

    pub fn lowest(&self, k: i64) -> i64 {
        let k = k as usize;
        if (1..self.lowest.len()).contains(&k) {
            self.lowest[k]
        } else {
            0
        }
    }
}
