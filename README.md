# Lab 01 — Safe primes and Diffie–Hellman

Course: Nhập môn mã hóa mật mã. Group repo for the two-person split.

The program is C++14. It uses the C++ standard library and the SHAKE128 PRNG shipped in `lib/fips202`. GMP and NTL are not used. The graded code has to build without them.

## Build and run

Open **MSYS2 UCRT64** in the clone. On this PC the tree is already at `/c/Users/HP/Documents/24127017_MMMH`.

```bash
make test
make run
```

`make test` checks the arithmetic and `generate_private_key`. It does not search for a 512-bit prime.

`make run` prints a small exchange on `p = 23`, then searches for a 512-bit safe prime. That search can take a long time. After it finds one, Alice and Bob each get a private key and the program checks that their shared secrets match.

## What is already implemented

| File | Role |
|---|---|
| `include/uint512.h`, `src/uint512.cpp` | 512-bit unsigned integer. `digits[0]` is the least significant limb. Operators `+`, `-`, and the six comparisons. |
| `include/modular.h`, `src/modular.cpp` | `mod`, `AddMod`, `MulMod`, and `modular_exponentiation`, following `docs/pseudocode-modular-operation.pdf`. |
| `src/prime.cpp` | `generate_safe_prime` uses Miller–Rabin. `generate_private_key` draws a key in `[2, p - 2]` by rejection sampling. |
| `src/main.cpp` | Small exchange on `p = 23`, then the 512-bit exchange. |
| `lib/fips202/` | SHAKE128 PRNG from the course sample. On Windows, `randombytes` uses `std::random_device` because `<sys/random.h>` is not available. The lab seed is the fixed string `"seed"`. |
| `tests/test_modular.cpp` | Arithmetic checks, including `3^5 mod 13 = 9` and the `p = 23` exchange (shared secret `2`). |
| `tests/test_private_key.cpp` | Range, boundary, 512-bit sampling, and same-seed checks for the private key. |

`mod` reads bits from the most significant bit down to bit 0. `MulMod` and `ExpMod` read `y_i` in `y = Σ y_i * 2^i`, so index 0 is the least significant bit. A single carry bit keeps every stored intermediate inside 512 bits when the shift drops the top bit.

## Diffie–Hellman

`make run` computes:

- `A = g^a mod p` and `B = g^b mod p` with `g = 2`
- Alice's secret `B^a mod p` and Bob's secret `A^b mod p`
- It prints whether the two secrets are equal

`g = 2` is only a placeholder. In the report, state whether 2 generates the subgroup you use. For a safe prime `p = 2q + 1`, check that `g^q mod p` and `g^2 mod p` are not `1`. `uint512_t` holds at most 512 bits, so `bitsize` must be from 3 to 512. The course asks for at least 512 bits in the submitted run.

## Report

Write the PDF after the program is finished. It is not in this repo. The lab asks for these sections:

1. The big-integer type and how `+`, `-`, comparison, `mod`, `AddMod`, `MulMod`, and `ExpMod` work. That description can follow the code already in this repo.
2. The primality test you implemented.
3. Randomness in cryptography, and how the PRNG is seeded. This section is optional extra credit.
4. One real-world use of Diffie–Hellman.
5. How to build and run the program, and how to change `bitsize` (the argument of `generate_safe_prime` in `src/main.cpp`).
6. Strengths, weaknesses, and what you would improve.

## Files the course expects in the zip

The names below are the submission layout. This repo is the working tree.

```text
<MSSV1-MSSV2>.zip
  project_01_source/
    *.cpp
  project_01_report/
    *.pdf
```

Sort the student IDs in class-list order. A part that does not compile scores zero for that part. Questions from the lab sheet go to dientran0901@gmail.com.

## Course documents

- `docs/Lab01.pdf`
- `docs/pseudocode-modular-operation.pdf`
