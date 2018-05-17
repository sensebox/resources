# Contributing

We are generally open for contributions! If you feel you are able to provide improvements or bug fixes by yourself, please do so by forking the repository and creating a [pull request]. Otherwise feel free to create an [issue].

## A note on organization of the repository

First of all init the include submodule by running:
```
git submodule --init --recursive
```

Before you start developing please create the following `prepare-commit-msg git-hook` inside your `.git/hooks` folder:
```bash
#!/bin/sh

# a list of all files that are changing with this commit
FILES_CHANGING=$(git diff --cached --name-only -- libraries)

# if there is only one file changing
if [ -z "$FILES_CHANGING" ]; then

    # append it to $1 - the file containing the commit message
    echo " [ci skip]" >> $1

fi
```

This will prefix all commit messages with `[ci skip]` for travis to only release new versions if files inside the `libraries` folder are changed. Changes to the rest of the repository are not important for releasing a new verison.