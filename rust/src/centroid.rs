/// Finds the centroid of the component of tree g containing v0.
/// "marked" contains previously found centroids, and "sub" is used as a buffer for computing
/// subtree sizes.
pub fn find_centroid(g: &[Vec<usize>], v0: usize, marked: &mut [bool], sub: &mut [i64]) -> usize {
    let mut st = vec![(v0, v0, true)];
    while let Some((v, p, down)) = st.pop() {
        if down {
            sub[v] = 1;
            st.push((v, p, false));
            for &w in &g[v] {
                if w != p && !marked[w] {
                    st.push((w, v, true));
                }
            }
        } else {
            if p != v {
                sub[p] += sub[v];
            }
        }
    }
    let bound = sub[v0] / 2;
    let mut c = v0;
    'outer: loop {
        for &w in &g[c] {
            if !marked[w] && sub[w] < sub[c] && sub[w] > bound {
                c = w;
                continue 'outer;
            }
        }
        marked[c] = true;
        return c;
    }
}
