We are doing test driven development. For each task, do the following:
1. Add a catch2 test to the file with a failing example. Add any stub implementation necessary to get the test to compile.
2. Run the tests (e.g., `ctest -R rot13`) and ensure it fails. Fix any compile issue to get the test to run.If the test does not fail, stop.
3. Add the minimal implementation to get the test pass.
4. Run the tests and ensure it passes. If it doesn't attempt to fix it by changing only the implementation.
5. Stop, please let the user review your changes before committing.
6. Commit it, starting the commit message with 🤖 do denote the commit was done by you.

After every step above, stop and let the user review your changes.

If the task involves refactoring, take small steps figure out a sequence of small moves that minimize the amount of time the code is broken. Treat each small step as a task, run the tests, and commit after each one.

If the user doesn't specify test and implementation files, stop and ask the user before continuing. DO NOT USE TOOLS TO TRY TO FIND THE FILES.
