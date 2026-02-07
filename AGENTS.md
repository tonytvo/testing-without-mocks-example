We are doing test driven development. For each task, do the following:
1. Add a test to the test file specified by the user.
2. Run the tests and ensure it fails. If it doesn't stop.
3. Add the implementation to the implementation file specified by the user.
4. Run the tests and ensure it passes. If it doesn't attempt to fix it by changing only the implementation.
5. Commit it, starting the commit message with 🤖 do denote the commit was done by you.
6. Stop and wait for the next task.

If the task involves refactoring, take small steps figure out a sequence of small moves that minimize the amount of time the code is broken. Treat each small step as a task, run the tests, and commit after each one.

If the user doesn't specify test and implementation files, stop and ask the user before continuing. DO NOT USE TOOLS TO TRY TO FIND THE FILES.
