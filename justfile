default: check lint run test

# Run FIMA without arguments
run:
    cargo run -p fima

build:
    cargo build

docs workspace="--workspace":
    cargo doc {{ workspace }} --no-deps --all-features

lint:
    cargo clippy --no-deps

check workspace="--workspace":
    cargo check {{ workspace }}

test workspace="--all-targets":
    cargo test {{ workspace }}
    cargo test --doc

# Publish a new version of libfima in crates.io
publish:
    cargo publish -p libfima
