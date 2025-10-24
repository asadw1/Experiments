# TopoCrypt

Bare-metal C prototype for TopoCrypt–KnotHomology, a research-oriented cryptosystem that encodes keys and messages into layered topological objects: triangulated surfaces with embedded link gadgets and two-parameter filtrations. This repository is a scaffold and development platform for implementing, experimenting with, and benchmarking the system in a minimal-dependency C environment.

## Goals
- Provide a deterministic, minimal-dependency C implementation to explore topology-based encoding and decoding primitives.  
- Implement modular components: mesh handling, combinatorial knot gadgets, two-parameter filtration engine, invariant calculators, persistence samplers, classifiers, and a small crypto wrapper for final-key use.  
- Enable reproducible experiments and benchmarks to evaluate practical hardness of the composed problems (knot recognition, multi-parameter persistence, label obfuscation).  
- Maintain clear, canonical serialization of public artifacts so experiments are comparable across runs.

---

## Architecture Overview
- Mesh core  
  - Deterministic triangulated surface representation (triangle lists, half-edge utilities).  
  - Vertex label permutation and inversion.  
- Gadget builder  
  - Localized combinatorial tangles for unknot and nontrivial knots; gadget insertion and obfuscation.  
- Filtration engine  
  - Primary ordering of simplices and sparse secondary events to produce a two-parameter filtered complex; slice extraction for single-parameter projections.  
- Invariants and homology  
  - Bounded-complexity polynomial invariants, Betti calculators, and slicewise persistence sampler to derive features for decoding.  
- Classifier and ECC  
  - Compact rule-based and small-model classifiers; error-correcting encoding for embedded final keys.  
- Crypto wrapper  
  - Minimal Ed25519 signing for public outputs; stubbed interface for post-quantum primitives (Kyber, Dilithium, SPHINCS+) to import/export final keys.  
- Serialization  
  - Canonical, deterministic binary format for public artifacts and private key files with versioning and signatures.  
- Bench and tools  
  - Synthetic instance generator, visualization helpers, and benchmark harnesses.

---

## Build and Run

### Windows CMD quick notes
- This project is portable but assumes a working C toolchain. For Windows, using MSYS2, MinGW, or WSL is recommended for the smoothest build with the provided Makefile. If you prefer native Windows tooling, ensure gcc and make are available in PATH (MinGW/MSYS recommended).

### Common commands (run from repository root)
- Build (POSIX or Windows with make)
  - `make`
- Clean
  - `make clean`
  - or run `build.bat` to invoke the Makefile on Windows if `make` is present

### Expected CLI layout (placeholders implemented in src/main.c)
- `topo keygen` — Generate a private key file and a public artifact template  
- `topo encode` — Encode messages and K_final into layered topology; output signed public artifact  
- `topo decode` — Given private key and public artifact, run decryption pipeline and recover K_final

### Examples
- Generate a deterministic test keypair:
  - `topo keygen --seed 0xDEADBEEF --out private.key --pub public.art`
- Encode a small test payload:
  - `topo encode --priv private.key --m1-file m1.bin --m2-file m2.bin --kfinal kfinal.bin --out public.art`
- Decode and recover final key:
  - `topo decode --priv private.key --pub public.art --out recovered_kfinal.bin`

Notes:
- The scaffold provides command stubs and file-format placeholders. Implementations for heavy modules (invariants, homology, full decryption) must be completed before the pipeline produces usable cryptographic keys.  
- For development and debugging, build in debug mode by editing `CFLAGS` in the Makefile to include `-g -O0` and additional assertions.

---

## Serialization and File Formats
- Public artifact canonical layout  
  - Header: magic, version, canonicalization flags  
  - Face list: ordered triangle list describing scrambled triangulation F_out  
  - Filtration metadata: primary order array and secondary-event sparse list  
  - Signature: Ed25519 signature over canonical bytes
- Private key file  
  - Permutation π, gadget map entries, μ bits, deterministic PRNG seed, signing private key, optional K_final material (when embedding symmetric final keys)
- Tests and fixtures  
  - `tests/fixtures` contains canonical small meshes and gadget templates for unit testing and consistency checks

Refer to `docs/file-formats.md` for byte-level offsets, endianness, and canonicalization rules.

---

## Development Conventions
- Language: C99  
- Single translation unit per logical module for clarity and deterministic linking  
- Minimal third-party code stored under `third_party`; prefer small, vetted, permissively-licensed implementations (Ed25519 stub provided)  
- Deterministic behavior under fixed seeds; tests use fixed seeds in `tests/fixtures`  
- Strict error codes and explicit resource ownership; avoid implicit globals that break determinism

---

## Testing and Benchmarks
- Unit tests: implement under `tests/unit` for mesh, gadget, filtration, invariants.  
- Integration tests: an end-to-end test in `tests/integration` that encodes and decodes small instances deterministically.  
- Bench harness: use `src/bench` to measure per-gadget runtime, invariant computation, and classifier accuracy.  
- Reproducibility: seed the deterministic PRNG from fixed test seeds in fixtures to ensure identical outputs across runs.

---

## Ethical and Security Considerations
- This repository is research-oriented. The theoretical TopoCrypt design intentionally composes hard mathematical problems for study. Practical cryptographic security is not established by this scaffold.  
- When experimenting with systems intended to resist lawful access or surveillance, follow applicable laws and institutional policies. Use responsibly and document legitimate research goals.  
- Do not use preliminary or experimental outputs from this scaffold as drop-in replacements for vetted cryptographic systems until formal security proofs, peer review, and rigorous testing have been completed.

---

## Contributing and License
- Contributing  
  - Fork, implement incremental modules, include regression tests under `tests/unit`, and add experiment scripts under `bench/`. Keep new third-party code minimal and include license files inside `third_party`.
- License  
  - MIT License. See `LICENSE` file.


