struct BitTrie {
    max_bits: usize,
    next: [Vec<usize>; 2],
    size: Vec<usize>,
}

impl BitTrie {
    pub fn new(max_bits: usize) -> Self {
        Self {
            max_bits,
            next: [vec![0], vec![0]],
            size: vec![0],
        }
    }
    fn new_node(&mut self) -> usize {
        self.next[0].push(0);
        self.next[1].push(0);
        self.size.push(0);
        self.size.len() - 1
    }
    pub fn contains(&self, x: u64) -> bool {
        let mut cur = 0;
        for i in (0..self.max_bits).rev() {
            let b = ((x >> i) & 1) as usize;
            cur = self.next[b][cur];
            if cur == 0 {
                return false;
            }
        }
        assert!(self.size[cur] <= 1);
        self.size[cur] == 1
    }
    /// Insert value, returning true if it wasn't already in the trie
    pub fn insert(&mut self, x: u64) -> bool {
        if self.contains(x) {
            return false;
        }
        self.size[0] += 1;
        let mut cur = 0;
        for i in (0..self.max_bits).rev() {
            let b = ((x >> i) & 1) as usize;
            if self.next[b][cur] == 0 {
                let node = self.new_node();
                self.next[b][cur] = node;
            }
            cur = self.next[b][cur];
            self.size[cur] += 1;
        }
        assert!(self.contains(x));
        true
    }
    /// Remove value, returning true if it was in the trie
    pub fn remove(&mut self, x: u64) -> bool {
        if !self.contains(x) {
            return false;
        }
        self.size[0] -= 1;
        let mut cur = 0;
        for i in (0..self.max_bits).rev() {
            let b = ((x >> i) & 1) as usize;
            cur = self.next[b][cur];
            self.size[cur] -= 1;
        }
        assert!(!self.contains(x));
        true
    }

    /// Assuming trie is nonempty, finds minimal x^y for y in the trie
    pub fn min_xor(&self, x: u64) -> u64 {
        if self.size[0] == 0 {
            unreachable!()
        }
        let mut cur = 0;
        let mut y = 0;
        for i in (0..self.max_bits).rev() {
            let mut b = ((x >> i) & 1) as usize;
            if self.next[b][cur] == 0 || self.size[self.next[b][cur]] == 0 {
                b ^= 1;
            }
            if b != 0 {
                y ^= 1 << i;
            }
            cur = self.next[b][cur];
            assert!(cur != 0);
            assert!(self.size[cur] > 0);
        }
        y ^ x
    }
}
