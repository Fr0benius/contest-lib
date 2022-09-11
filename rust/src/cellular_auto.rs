use std::{cmp::Eq, collections::HashSet, hash::Hash};

pub trait CellAutoSpec {
    type T;
    fn neighbors(x: &Self::T) -> Vec<Self::T>;
    fn rule(alive: bool, alive_neighbors: usize) -> bool;
}

pub struct CellAuto<Spec: CellAutoSpec> {
    pub cells: HashSet<Spec::T>,
}

impl<Spec: CellAutoSpec> CellAuto<Spec> {
    #[must_use]
    pub fn next(&self) -> Self
    where
        Spec::T: Eq + Hash + Clone,
    {
        let candidates: HashSet<Spec::T> = self.cells.iter().flat_map(Spec::neighbors).collect();
        let cells = candidates
            .iter()
            .filter(|&cell| {
                let alive = self.cells.contains(cell);
                let alive_neighbors = Spec::neighbors(cell)
                    .iter()
                    .filter(|&other| self.cells.contains(other))
                    .count();
                Spec::rule(alive, alive_neighbors)
            })
            .cloned()
            .collect();
        Self { cells }
    }
}

