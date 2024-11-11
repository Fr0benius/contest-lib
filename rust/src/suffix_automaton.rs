#[derive(Clone, Debug)]
struct SuffixAutomaton<T> {
    next: Vec<HashMap<T, usize>>,
    len: Vec<usize>,
    link: Vec<usize>,
    last: usize,
}

impl<T> SuffixAutomaton<T> 
where T: Copy + Eq + std::hash::Hash {
    fn new(s: &[T]) -> Self {
        let mut sa = Self {
            next: vec![Default::default()],
            len: vec![0],
            link: vec![0],
            last: 0,
        };
        for &c in s {
            sa.add(c);
        }
        sa
    }
    fn new_node(&mut self) -> usize {
        self.next.push(Default::default());
        self.len.push(0);
        self.link.push(0);
        self.next.len() - 1
    }
    pub fn add(&mut self, c: T) {
        let cur = self.new_node();
        let mut p = self.last;
        self.len[cur] = self.len[p] + 1;
        while !self.next[p].contains_key(&c) {
            self.next[p].insert(c, cur);
            if p == 0 {
                self.last = cur;
                return;
            }
            p = self.link[p];
        }
        let q = self.next[p][&c];
        if self.len[q] == self.len[p] + 1 {
            self.link[cur] = q;
        } else {
            let clone = self.new_node();
            self.next[clone] = self.next[q].clone();
            self.link[clone] = self.link[q];
            self.len[clone] = self.len[p] + 1;
            while self.next[p][&c] == q {
                self.next[p].insert(c, clone);
                if p == 0 {
                    break;
                }
                p = self.link[p];
            }
            self.link[q] = clone;
            self.link[cur] = clone;
        }
        self.last = cur;
    }
}
