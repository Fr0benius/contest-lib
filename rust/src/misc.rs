use std::collections::HashSet;

pub fn render_2d<I: Iterator<Item = (i64, i64)>>(iter: I) -> String {
    let set: HashSet<_> = iter.collect();
    let mnr = set.iter().map(|&(r, _)| r).min().unwrap();
    let mxr = set.iter().map(|&(r, _)| r).max().unwrap();
    let mnc = set.iter().map(|&(_, c)| c).min().unwrap();
    let mxc = set.iter().map(|&(_, c)| c).max().unwrap();
    let mut out = vec![];
    for r in mnr..=mxr {
        for c in mnc..=mxc {
            if set.contains(&(r, c)) {
                out.push(b'0');
            } else {
                out.push(b' ');
            }
        }
        out.push(b'\n');
    }
    String::from_utf8(out).unwrap()
}

