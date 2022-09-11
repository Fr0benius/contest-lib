pub struct BipartiteMatcher {
    n: usize,
    m: usize,
    g: Vec<Vec<usize>>,
}

impl BipartiteMatcher {
    pub fn new(n: usize, m: usize, g: Vec<Vec<usize>>) -> Self {
        Self { n, m, g }
    }
    fn dfs(
        &self,
        v: usize,
        seen: &mut [bool],
        mat: &mut [Option<usize>],
        rmat: &mut [Option<usize>],
    ) -> bool {
        if seen[v] {
            return false;
        }
        seen[v] = true;
        for &w in &self.g[v] {
            if rmat[w].is_none() {
                mat[v] = Some(w);
                rmat[w] = Some(v);
                return true;
            }
        }
        for &w in &self.g[v] {
            if self.dfs(rmat[w].unwrap(), seen, mat, rmat) {
                mat[v] = Some(w);
                rmat[w] = Some(v);
                return true;
            }
        }
        false
    }
    pub fn matching(&self) -> (Vec<Option<usize>>, Vec<Option<usize>>, usize) {
        let mut mat = vec![None; self.n];
        let mut rmat = vec![None; self.m];
        let mut seen;
        let mut done = false;
        while !done {
            done = true;
            seen = vec![false; self.n];
            for v in 0..self.n {
                if mat[v].is_none() && self.dfs(v, &mut seen, &mut mat, &mut rmat) {
                    done = false;
                }
            }
        }
        let count = mat.iter().filter(|x| x.is_some()).count();
        (mat, rmat, count)
    }
}
