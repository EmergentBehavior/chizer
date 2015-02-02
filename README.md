[![Build Status](https://travis-ci.org/storj-jp/chizer.svg?branch=master)](https://travis-ci.org/storj-jp/chizer)
[![Coverage Status](https://coveralls.io/repos/storj-jp/chizer/badge.svg?branch=master)](https://coveralls.io/r/storj-jp/chi?branch=master)

# ![](https://raw.githubusercontent.com/storj-jp/chizer/master/image/chizer.png)  Chizer
Chizer is a tool which uses a chi-square test to determine if a file is encrypted or not. Chizer is available as a C program or Python module.

## Theory & Evaluation
Information relating to the theory and efficacy of this module are available [here](https://github.com/storj-jp/chizer/blob/master/EVALUATION.md). 

## Requirements
Chizer only requires `gcc`. However, the python modules require that either Python 2.x or 3.x is installed.

## Installation

To compile the C program:

    $ make chizer

To build the python module:

    $ make python
    
To run the associated tests:

    $ make test
    $ ./test

## Usage
Once installed, you can use `chizer` to analyze a file of indeterminate encryption:

    $ ./chizer <filename_of_shard>

If you are using Python, you can use `isFileEncrypted()` to test for encryption (by testing the entire file at once), or you can use `isChunksEncrypted()` to do a deeper analysis by analyzing discrete chunks of the file:

    import chizer
    file_path = 'data/faked.dat'
    isFileEncrypted = chizer.isFileEncrypted(file_path)
    areChunksEncrypted = chizer.isChunksEncrypted(file_path)
  
# Contribution
Improvements to the codebase and pull requests are encouraged.
