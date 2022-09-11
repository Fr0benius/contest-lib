set -e
cargo init --bin --name contest
rm ./src/main.rs
mkdir src/bin
for problem in "$@"; do
  cp "$HOME"/Coding/contest-lib/rust/template.rs src/bin/"$problem".rs
done
