# DraygonTensor 

[![Built with Spacemacs](https://cdn.rawgit.com/syl20bnr/spacemacs/442d025779da2f62fc86c2082703697714db6514/assets/spacemacs-badge.svg)](http://github.com/syl20bnr/spacemacs)

![Draygon](http://vignette4.wikia.nocookie.net/metroid/images/2/25/Draygon.jpg/revision/latest?cb=20131117053231)

*The above image file is copyrighted by Nintendo or another organization. The individual who uploaded this work and first used it in an article, and later persons who place it into articles, assert that this qualifies as fair use of the material under United States copyright law.*

## Prerequisites 

### Windows

- CMake 2.8 or greater
- Git

The included build scripts assume that `cmake.exe` can be found in the path, edit them to reflect the location of your cmake binary.

### OSX

- CMake 2.8 or greater
- Git
- SDL2-2.0.4 or greater

### Linux

- CMake 2.8 or greater
- Git
- SDL2-2.0.4 or greater

## Getting the repo 

- Make a fork of this repository

- Clone your fork

```
git clone <your_fork_url>
cd <your_fork_dir>
```

- Setup local branches to track remote branches

```
git branch develop origin/develop
```

- Repeat for each branch you want to track locally

- OR grab all remote branches for local tracking:

```
for b in `git branch -r | grep -v -- '->'`; do git branch --track ${b##origin/} $b; done
```

## Building


## Contributing

Before contributing, consult the [contribution guidelines](https://github.com/samdelion/DraygonTensor/blob/master/CONTRIBUTING.md).
    
