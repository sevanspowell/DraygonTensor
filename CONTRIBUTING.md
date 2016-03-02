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

<!-- markdown-toc end -->


## General Contribution Guidelines

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
pull request must conform to the [Codestyle Guidelines](https://github.com/Izabia/DrunkenSailorEngine/blob/master/docs/CODESTYLE.md) and should at least list
``Izabia`` and ``samdelion`` as reviewers.

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
