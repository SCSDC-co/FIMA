default: check lint run test

# Run FIMA without arguments
run:
    cargo run -p fima

docs workspace="--workspace":
    cargo doc {{ workspace }} --no-deps

lint:
    cargo clippy --no-deps

check workspace="--workspace":
    cargo check {{ workspace }}

test workspace="--all-targets":
    cargo test {{ workspace }}
    cargo test --doc
