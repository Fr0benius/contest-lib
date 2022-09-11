pub trait BinSearch<T> {
    fn lower_bound(&self, val: &T) -> usize;
    fn upper_bound(&self, val: &T) -> usize;
}

impl<T: std::cmp::PartialOrd> BinSearch<T> for [T] {
    fn lower_bound(&self, val: &T) -> usize {
        let mut l = 0;
        let mut r = self.len();
        while l < r {
            let m = (l + r) / 2;
            if &self[m] < val {
                l = m + 1;
            } else {
                r = m;
            }
        }
        l
    }

    fn upper_bound(&self, val: &T) -> usize {
        let mut l = 0;
        let mut r = self.len();
        while l < r {
            let m = (l + r) / 2;
            if &self[m] <= val {
                l = m + 1;
            } else {
                r = m;
            }
        }
        l
    }
}

