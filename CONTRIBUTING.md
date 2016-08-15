# Contribution Guidelines

The previous sailors were drunkards, we're running a tight ship here, so please
read the following guidelines.

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-generate-toc again -->
**Table of Contents**

  - [General Contribution Guidelines](#general-contribution-guidelines)
      - [Git Workflow](#git-workflow)
      - [Branches](#branches)
      - [Pull-request](#pull-request)
      - [Commit messages](#commit-messages)
  - [Project Structure](#project-structure)
      - [Contributing to the engine](#contributing-to-the-engine)
      - [Creating an engine application](#creating-an-engine-application)
      - [Writing unit tests](#writing-unit-tests)

<!-- markdown-toc end -->


## General Contribution Guidelines

### Coding standards

From now on please read and abide by the [coding standards document](https://github.com/samdelion/DraygonTensor/blob/master/docs/CodingStandards.md). When working with old code, prefer to leave it in it's own style, rather than changing it to abide by the new coding standards (unless previously agreed upon by at least one team member).

### Git Workflow

Use the [Feature Branch](https://www.atlassian.com/git/tutorials/comparing-workflows/feature-branch-workflow) Git workflow.

### Branches

New developments of any kind should be developed in a new branch. Branches should
follow the naming convention: [first letter of first name][last name]_[feature name]. 
For example, ```tanderson_becomeTheOne``` or ```asmith_destroyAnomaly```.

All development occurs on the ```develop``` branch and as such, all contributions
should be submitted against the ```develop``` branch. The ```master``` branch is
reserved for submission-ready code and should be considered read-only.

### Pull-request

All contributions should be submitted as a [pull request](https://yangsu.github.io/pull-request-tutorial/). To be considered, the
pull request must conform to the [Codestyle Guidelines](https://github.com/samdelion/DraygonTensor/blob/master/docs/CodingStyleGuide.md) and should at least list
two other group members as reviewers.

### Commit messages

(Taken from https://github.com/syl20bnr/spacemacs/blob/master/CONTRIBUTING.org)

Write commit messages according to [Tim Pope’s](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html) guidelines (slightly modified):

* In the commit message, use present tense and write in the imperative: “Fix bug”, not “fixed bug” or 
“fixes bug”. Imagine saying "When applied, this commit will \<commit message\>"
* Start with a capitalized, short (72 characters or less) summary, followed by a blank line.
  Do not end the summary with a period.
* If necessary, add one or more paragraphs with details, wrapped at 72 characters.
* The present tense requirement may be relaxed at this point.
* Separate paragraphs by blank lines.

This is a model commit message:

```
Capitalized, short (72 chars or less) summary

More detailed explanatory text, if necessary.  Wrap it to about 72
characters or so.  In some contexts, the first line is treated as the
subject of an email and the rest of the text as the body.  The blank
line separating the summary from the body is critical (unless you omit
the body entirely); tools like rebase can get confused if you run the
two together.

Write your commit message in the imperative: "Fix bug" and not "Fixed bug"
or "Fixes bug."  This convention matches up with commit messages generated
by commands like git merge and git revert.

Further paragraphs come after blank lines.

- Bullet points are okay, too

    - Typically a hyphen or asterisk is used for the bullet, followed by a
      single space, with blank lines in between, but conventions vary here

    - Use a hanging indent 
```

Use the summary to briefly describe what changed, use the more detailed explanation to explain **why** the change was made.

## Project Structure 

The project structure is as follows:
```
 .
 +-- bin      // Output of build, i.e. the engine library file and header files.
 +-- build    // Out of source build directory, filled with CMake and compiler-specific files generated at compile time. 
              // May be deleted at any time to trigger a fresh build. This is where your project files will appear.
 +-- cmake    // Contains all the configuration files for the CMake build
 +-- docs     // Documentation
 +-- external // External dependencies downloaded by CMake, may be deleted at any time to trigger redownload of dependencies.
 +-- project  // Applications that make use of the engine, we would use this space mainly for developing little 
              // prototype applications to check some part of our engine. 
              // For example, a script application to test the scripting capabilities of our engine.
 +-- src      // Engine source and header files
 +-- test     // Test suite source and header files
```

### Contributing to the engine
 
To contribute code to the engine:
 
- Author the relevant header and/or source files under `src/engine/`:
    - `touch engine/lua/MyLuaApi.h && touch engine/lua/MyLuaApi.cpp`
- Add the created files to `src/engine/CMakeLists.txt` under ENGINE_INCLUDE_FILES and ENGINE_SRC_FILES respectively, relative to the src/engine directory:
```
set(ENGINE_INCLUDE_FILES lua/MyLuaApi.h) 
set(ENGINE_SRC_FILES lua/MyLuaApi.cpp) 
``` 
- Build or rebuild the project.

### Creating an engine application

- Create a new directory under `project/`:
  - `mkdir -p project/myLuaTest`
- Add a new directory to `project/CMakeLists.txt`:
```
subdirs(
sound
myLuaTest
)
```
- `cd` into your new directory:
  - `cd myLuaTest`
- Copy the contents of `project/example`` into your new directory.
- Modify the top-level `CMakeLists.txt` to reflect your project:
```
project(myLuaTest)

include(Common)

subdirs(src)
```
- Place your source/header files in the `src/`` directory.
- Place your assets in the `assets/` directory.
- Modify the CMakelists.txt in the src/ directory to include your project's
source and header files:
```
...
set(GAME_INCLUDE_FILES
)

set(GAME_SRC_FILES
  main.cpp
)
...
```

- Nothing else should need modifying.
- Build/rebuild the project.
- Find your executable in `project/${YOUR_PROJECT_NAME}/bin`:
```
cd project/myLuaTest/bin
./myLuaTest
```

### Writing unit tests

All tests can be found in the test directory.

```
.
+-- test
|      +-- assets // Place any test assets here
|      +-- bin    // Where test executable is placed
|      +-- src    // Where test framework file and all test suites are found
```

To create a new unit test:

- Write the unit test file. Preferably, the test suite file location should mirror
that of the engine. For example, if testing `engine/lua/LuaAPI.h` create a file
in the `src` directory called `engine/lua/LuaAPITestSuite.h`.
- Add your test suite to the `src/main.cpp` file as an include directive:

```
...

#include "engine/lua/LuaAPITestSuite.h"

...
```

- Add your test suite to the `src/CMakeLists.txt` file:

```
set(TEST_SUITE_INCLUDE_FILES
  engine/lua/LuaAPITestSuite.h
)
```

- Congratulations! Your test suite is now added to the list of test suites to be run.
- To learn how to write unit tests, either look at unit tests already written or refer to the [Google Test Primer](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md).
