#[derive(Debug, Clone, Copy, PartialEq, Eq, Default)]
pub struct RollingHash<const X: i32, const P: i32> {
    value: ModInt<P>,
    size: i32,
}

impl<const X: i32, const P: i32> RollingHash<X, P> {
    pub fn push(mut self, c: u8) -> Self {
        self.value = self.value * ModInt(X) + ModInt(c as i32);
        self.size += 1;
        self
    }

    pub fn push_left(mut self, c: u8) -> Self {
        self.value = ModInt(c as i32).pow(self.size as _) + self.value;
        self.size += 1;
        self
    }

    pub fn combine(mut self, other: Self) -> Self {
        self.value = self.value * ModInt(X).pow(other.size as _) + other.value;
        self.size += other.size;
        self
    }

    pub fn singleton(c: u8) -> Self {
        Self {
            value: ModInt(c as _),
            size: 1,
        }
    }
}
