# C++ Port Spec for `testing-without-mocks-example`

## Goals

1. Reproduce the TypeScript ROT‑13 CLI using idiomatic modern C++.
2. Preserve the A‑Frame architecture: Application/UI → Infrastructure + Logic.
3. Keep Infrastructure and Logic layers independent; only the Application knows about both.
4. Uphold the “Testing Without Mocks” philosophy: narrow, state-based, sociable tests with minimal test doubles.
5. Offer a clean developer experience (single build tool, fast tests, easy entry point).

## Target Environment

- **Language:** C++20 (gives `std::string_view`, ranges-like utilities, and designated initializers while remaining broadly supported).
- **Build & test driver:** CMake ≥ 3.24 generating Ninja/Makefiles. Top-level `CMakeLists.txt` drives both library and executable builds plus tests.
- **Unit test framework:** Catch2 v3 (header-only, good BDD-style descriptions, easy to embed). Tests compiled into dedicated binaries per layer.

## Repository Layout (proposed)

testing-without-mocks-example/  
├── CMakeLists.txt  
├── spec.md                 ← this file  
├── cpp/  
│   ├── CMakeLists.txt  
│   ├── src/  
│   │   ├── app/  
│   │   │   ├── App.hpp  
│   │   │   └── App.cpp  
│   │   ├── infrastructure/  
│   │   │   ├── CommandLine.hpp  
│   │   │   ├── CommandLine.cpp  
│   │   │   ├── OutputListener.hpp  
│   │   │   └── OutputListener.cpp  
│   │   ├── logic/  
│   │   │   ├── Rot13.hpp  
│   │   │   └── Rot13.cpp  
│   │   └── main.cpp        ← replaces TypeScript `run.js`  
│   └── tests/  
│       ├── app/  
│       │   └── AppTests.cpp  
│       ├── infrastructure/  
│       │   ├── CommandLineTests.cpp  
│       │   └── OutputListenerTests.cpp  
│       └── logic/  
│           └── Rot13Tests.cpp  
└── scripts/ (optional helpers: build, test, run)

*`main.cpp` is deliberately thin: instantiate `App::create()` and call `run()`.*

## Layer Responsibilities

### Application/UI (`App`)
- Owns the “logic sandwich”:
  1. Retrieve command-line args from `CommandLine`.
  2. Validate count (0 → usage, >1 → “too many arguments”).
  3. On exactly one arg, delegate to `rot13::transform()` and write result via `CommandLine`.
- **Construction**: `App::create()` assembles concrete dependencies; unit tests inject doubles via constructor overloads.
- **Testing strategy**: sociable tests using `CommandLine::createNull()` (null process, tracked output). Expectations match strings exactly, including trailing newline.

### Infrastructure (`CommandLine`, `OutputListener`)
- `CommandLine` wraps:
  - Reading `argv` (skip program name).
  - Writing to `std::ostream` (defaults to `std::cout`), while notifying listeners.
- `CommandLine::create()` uses a thin wrapper struct `ProcessHandles` (pointers/references to `int argc`, `char* argv[]`, `std::ostream&`).
- `CommandLine::createNull()` builds stub handles: configurable args vector, `std::ostringstream` sink.
- `trackOutput()` returns an `OutputTracker` (captures every emitted chunk; supports `.data()`, `.clear()`, `.stop()`).
- `OutputListener` uses `std::function` callbacks stored in a vector; `trackOutput()` registers a RAII tracker object.

**Testing approach:**
- State-based tests verifying:
  - `writeOutput()` sends text to provided stream and tracker.
  - `args()` trimming program metadata.
- “Narrow integration” tests spawn helper executables (similar to TypeScript runners) via `std::system`/`std::process`? Instead of forking, we can build tiny Catch2 helpers run with custom args. Each helper binary is executed by the test to assert real CLI behavior.

### Logic (`rot13`)
- Pure function `std::string rot13::transform(std::string_view input)`.
- Validates input (throw `std::invalid_argument` if not UTF-8 string? In C++ we accept string_view; no runtime type info). Instead, overloads will accept `std::string_view`, so compile-time ensures string usage. We still guard against non-ASCII by only rotating `[A-Za-z]`.
- Implementation: iterate characters; if `A-Z` or `a-z`, shift by 13 using arithmetic on char codes.
- Tests mirror TypeScript suite (empty string, lower/upper-case, symbols/numbers/emojis untouched).

## Testing Matrix

| Component | Test Binary | Key Assertions |
|-----------|-------------|----------------|
| Logic     | `rot13_tests` | Alphabet transformations, unchanged symbols, exception on invalid usage (if any). |
| Infrastructure | `command_line_tests`, `output_listener_tests` | Streams writes, trackers capture sequences, null variant avoids stdout, real-run helpers prove `argv` pass-through. |
| Application | `app_tests` | `CommandLine.createNull()` supplies args; verify outputs match expected lines. Tests remain sociable (real rot13). |

**Runner helpers** (C++ analogs of `_command_line_test_*_runner.js`) live under `cpp/tests/infrastructure/runners/`:
- `ArgsRunner.cpp`: prints JSON-like vector to stdout to assert actual process argv handling.
- `OutputRunner.cpp`: writes to stdout to ensure `writeOutput` hits console.
- `NulledOutputRunner.cpp`: ensures null stream suppresses terminal output.

Catch2 tests can spawn these binaries using `std::filesystem` paths resolved relative to test executable (e.g., via `std::filesystem::path(__FILE__)` helpers). Equivalent of `pathToFile()` to ensure portability.

## Build & Execution Flow

1. `cmake -S cpp -B build` (configure).
2. `cmake --build build` (builds `rot13_cli`, `*_tests`, helper runners).
3. `ctest --output-on-failure` or direct invocation of each test binary.
4. `build/rot13_cli "Hello World"` prints encoded result.

Provide shell scripts mirroring TS repo (`build.sh`, `run.sh`, `watch.sh` if desired) that forward to CMake/Ninja.

## Edge Considerations

- **UTF-8 handling**: `std::string` assumed to hold UTF-8; ROT13 operates per byte, leaving multibyte characters untouched (matching TS behavior).
- **Process abstraction**: Instead of global `argc/argv`, application entry point (`main.cpp`) populates a `ProcessHandles` struct and passes it down, allowing future extension (e.g., injecting file streams).
- **Testing Without Mocks alignment**:
  - Use real logic in App tests (collaborator-based isolation by computing expected output via `rot13::transform()`).
  - Null command line avoids actual stdout, representing “Nullables” + “Embedded Stub”.
  - Output tracking uses RAII to stop tracking, paralleling TypeScript `OutputTracker`.

## Next Steps

1. Scaffold `cpp/` tree with placeholder headers/source files and CMake targets.
2. Implement Logic layer first (fast feedback via Catch2).
3. Build Infrastructure with trackers and null process; add helper runner executables for integration-style tests.
4. Implement Application using logic sandwich; write tests verifying usage/argument rules.
5. Wire `main.cpp` to `App`.
6. Document usage in README section for C++ port; update scripts.
