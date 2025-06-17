pub struct SuffixArray {
    pub suffix: Vec<usize>,
    pub lcp: Vec<usize>,
}

impl SuffixArray {
    pub fn new<T: Ord>(s: &[T]) -> Self {
        assert!(!s.is_empty());
        let (suffix, inv) = Self::generate_sa(s);
        let lcp = Self::generate_lcp(s, &suffix, &inv);
        Self { suffix, lcp }
    }

    fn generate_sa<T: Ord>(s: &[T]) -> (Vec<usize>, Vec<usize>) {
        let n = s.len();
        let mut sa: Vec<usize> = (0..n).collect();
        let mut inv = {
            let mut ord: Vec<usize> = (0..n).collect();
            ord.sort_unstable_by_key(|&i| &s[i]);
            let mut inv = vec![0usize; n];
            for w in ord.windows(2) {
                let i0 = w[0];
                let i1 = w[1];
                inv[i1] = inv[i0];
                if s[i0] < s[i1] {
                    inv[i1] += 1;
                }
            }
            inv
        };

        let comp = |i: usize, j: usize, gap: usize, inv: &[usize]| -> std::cmp::Ordering {
            match inv[i].cmp(&inv[j]) {
                std::cmp::Ordering::Equal => {
                    if i + gap >= n || j + gap >= n {
                        j.cmp(&i)
                    } else {
                        inv[i + gap].cmp(&inv[j + gap])
                    }
                }
                order => order,
            }
        };
        let mut gap = 1;
        let mut temp = vec![0; n];
        loop {
            sa.sort_unstable_by(|&i, &j| comp(i, j, gap, &inv));
            for i in 1..n {
                temp[i] = temp[i - 1]
                    + if comp(sa[i - 1], sa[i], gap, &inv) == std::cmp::Ordering::Less {
                        1
                    } else {
                        0
                    };
            }
            for i in 0..n {
                inv[sa[i]] = temp[i];
            }
            if temp[n - 1] == n - 1 {
                break;
            }
            gap *= 2;
        }
        (sa, inv)
    }

    fn generate_lcp<T: std::cmp::PartialEq>(
        s: &[T],
        suffix: &[usize],
        inv: &[usize],
    ) -> Vec<usize> {
        let n = suffix.len();
        let mut lcp = vec![0; n - 1];
        let mut k = 0;
        for i in 0..n {
            if inv[i] == n - 1 {
                continue;
            }
            let j = suffix[inv[i] + 1];
            while i + k < n && j + k < n && s[i + k] == s[j + k] {
                k += 1;
            }
            lcp[inv[i]] = k;
            if k > 0 {
                k -= 1;
            }
        }
        lcp
    }
}
