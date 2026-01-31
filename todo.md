# C++ Port Plan Organized by Testing Without Mocks Patterns

> The spec mandates a modern C++ port while preserving the Testing Without Mocks ethos. Every actionable item below maps directly to one of the repository’s documented patterns so we can work in focused slices. Follow the TDD micro-cycle for **every** code-producing bullet:
> 1. Extend the specified Catch2 test file with a failing example tied to the pattern.
> 2. Run the narrow test target (e.g., `ctest -R rot13`) and confirm the failure mode.
> 3. Touch only the named implementation file(s) to provide the production behavior.
> 4. Re-run the same test until it passes.
> 5. Commit with a `🤖`-prefixed message that describes the pattern slice you just completed.
> 6. Stop and await the next instruction before beginning another slice.

---

## Pattern Focus Queue
1. **Narrow Tests** — *ACTIVE*: currently in progress; keep other pattern tasks parked until this one is complete.
2. **State-Based Tests** — *Next Up*: prepare assertions to remain state-focused once Narrow Tests land.
3. **Overlapping Sociable Tests** — *Backlog*: will resume after the above patterns go green.
4. **Smoke Tests** — *Backlog*: to be scheduled once sociable coverage stabilizes.

## Foundational Patterns

### 1. Narrow Tests *(ACTIVE)*
- [x] **Repository Skeleton** — Create the `cpp/` layout (`src/{app,infrastructure,logic}`, `tests/{app,infrastructure,logic}`, and infrastructure helper runner folders).  
  - *Why:* Enables per-layer narrow test targets.  
  - *Verify:* Manual `ls cpp`.  
- [x] **Build Driver** — Add top-level `CMakeLists.txt` delegating to `cpp/`, plus `cpp/CMakeLists.txt` that enforces C++20, warnings, and FetchContent for Catch2.  
  - *Why:* Lets each narrow suite build independently.  
  - *Verify:* `cmake -S cpp -B build` succeeds.  
- [ ] **Rot13 Logic Tests** — Begin `cpp/tests/logic/Rot13Tests.cpp` with the minimal failing empty-string case, then expand for lowercase, uppercase, and symbol preservation.  
  - *Implementation:* `cpp/src/logic/Rot13.{hpp,cpp}`.  
  - *Pattern Link:* Ensures every behavior is covered by a targeted test.  
- [ ] **CommandLine Narrow Suite** — Scaffold `cpp/tests/infrastructure/CommandLineTests.cpp` to compile-instantiate headers, then drive each capability (args parsing, writing, listener fan-out, null handles) one case at a time.  
  - *Implementation:* `cpp/src/infrastructure/CommandLine.{hpp,cpp}`.  
- [ ] **OutputListener Narrow Suite** — Add `cpp/tests/infrastructure/OutputListenerTests.cpp` with failing cases for tracking, clearing, and stopping output capture.  
  - *Implementation:* `cpp/src/infrastructure/OutputListener.{hpp,cpp}`.  
- [ ] **App Narrow Suite** — Author `cpp/tests/app/AppTests.cpp` to cover no-arg usage, too many args, and happy path output via `CommandLine::createNull()`.  
  - *Implementation:* `cpp/src/app/App.{hpp,cpp}`.

### 2. State-Based Tests
- [ ] Ensure every new test asserts return values, captured output strings, or tracker state rather than call counts. Refine assertions in each suite above to check exact strings (including trailing newlines) or vector contents.  
  - *Verify:* Review each test file for state assertions before committing.

### 3. Overlapping Sociable Tests
- [ ] Keep `AppTests` sociable by using the real `rot13::transform()` and `CommandLine` null handles. Add helper builders that call real collaborators and compare outputs to expectations derived from actual logic.  
  - *Verify:* `ctest -R app_tests` after each sociable scenario.

### 4. Smoke Tests
- [ ] Once the CLI links, add `cpp/tests/app/MainSmokeTests.cpp` (or extend the existing suite) to spawn the built `rot13_cli` with a sample arg and assert the stdout text.  
  - *Implementation:* `cpp/src/main.cpp`.  
  - *Verify:* `ctest -R main_smoke` plus manual `build/rot13_cli "Hello"`.

### 5. Zero-Impact & Parameterless Instantiation
- [ ] Keep constructors for `App`, `CommandLine`, and `OutputListener` side-effect free. Add compile-time checks (tests that instantiate the classes without invoking behavior).  
  - *Verify:* `ctest -R command_line` & `ctest -R app_tests`.

### 6. Signature Shielding
- [ ] Introduce helper functions/structs in tests (e.g., `AppScenario` builder inside `AppTests.cpp`) that return multiple expected strings. Ensure production signatures remain stable while tests adapt through the helpers.  
  - *Verify:* Each helper is exercised by at least two tests before moving on.

### 7. Collaborator-Based Isolation
- [ ] In the happy-path `App` test, compute the expected ROT13 output by calling `rot13::transform()` directly inside the test, mirroring the JavaScript version.  
  - *Verify:* The test compares `CommandLine` output to the collaborator-derived value.

---

## Architectural Patterns

### 8. A-Frame Architecture
- [ ] Maintain strict include boundaries: `App` sees both infrastructure and logic; infrastructure/logic stay unaware of each other. Add CI check or script (e.g., `scripts/lint_includes.sh`) if necessary.  
  - *Verify:* Grep includes before committing each slice.

### 9. Logic Sandwich
- [ ] Implement `App::run()` as: read args via `CommandLine`, validate counts, call `rot13::transform()`, then write via `CommandLine`. Drive this with sequential failing tests (no args, too many, happy path).  
  - *Verify:* `ctest -R app_tests`.

### 10. Smoke Tests (Application top)
- *Handled above under Foundational Smoke Tests but track progress here as well.*

---

## Logic Patterns

### 11. Easily-Visible Behavior
- [ ] Keep `rot13::transform()` pure (no I/O). Enforce via tests that supply the same input twice and compare outputs, ensuring no hidden state.  
  - *Verify:* Additional Catch2 case checking determinism.

---

## Infrastructure Patterns

### 12. Infrastructure Wrappers
- [ ] `CommandLine::create()` should accept `ProcessHandles` wrapping `argc`, `argv`, and an `std::ostream`. Add a failing test that supplies fake handles and verifies args stripping/program-name skipping.  
  - *Implementation:* `CommandLine.{hpp,cpp}`.

### 13. Narrow Integration Tests
- [ ] Port the TypeScript runner concept: create helper binaries under `cpp/tests/infrastructure/runners/` (`ArgsRunner.cpp`, `OutputRunner.cpp`, `NulledOutputRunner.cpp`). Add Catch2 tests that spawn these helpers to verify real argv handling and stdout writes.  
  - *Verify:* `ctest -R infrastructure_runner`.

### 14. Output Tracking
- [ ] Provide `CommandLine::trackOutput()` returning an RAII tracker with `.data()`, `.clear()`, `.stop()`. Drive each method with a failing test before implementing.  
  - *Implementation:* `OutputListener.{hpp,cpp}` plus `CommandLine.cpp`.

### 15. Nullables & Embedded Stub / Configurable Responses
- [ ] Implement `CommandLine::createNull()` that accepts a vector of strings and a sink `std::ostringstream`. Tests must confirm:  
  1. Provided args are returned without touching real stdout.  
  2. Multiple null instances do not interfere.  
  - *Implementation:* `CommandLine.{hpp,cpp}`.  
  - *Verify:* `ctest -R command_line`.

---

## Developer Experience & Evolutionary Seeds

### 16. Scripts & CI (supporting pattern: Grow Evolutionary Seeds)
- [ ] Add `scripts/build.sh`, `scripts/test.sh`, `scripts/run.sh` that wrap CMake configure/build/test flows, keeping iterations fast.  
  - *Verify:* Running each script from repo root succeeds.  
- [ ] Introduce CI workflow (`.github/workflows/cpp.yml`) executing the scripts on Linux.  
  - *Verify:* Workflow passes after push or via local runner.  
- [ ] Update `README.md` with a “C++ Port” section describing how each Testing Without Mocks pattern manifests in the new codebase, including commands for build/test/run.  
  - *Verify:* Manually follow the documented steps on a clean checkout.

---

## Final Pattern Review

- [ ] Once all pattern-specific tasks are complete, perform an end-to-end manual acceptance pass: `scripts/build.sh && scripts/test.sh && build/rot13_cli "Testing"`. Capture any regressions as new pattern-aligned TODO entries rather than ad-hoc fixes.
- [ ] Audit layering boundaries, include guards, and formatter/lint outputs. Record any remaining gaps per pattern (e.g., missing smoke test) directly beneath the relevant heading above for future work.

> Work **one pattern slice at a time**: pick an unchecked bullet, execute the full TDD + commit workflow, then pause for review before starting the next slice.