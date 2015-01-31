[![Build Status](https://travis-ci.org/storj-jp/chizer.svg?branch=master)](https://travis-ci.org/storj-jp/chizer)
[![Coverage Status](https://coveralls.io/repos/storj-jp/chizer/badge.svg?branch=master)](https://coveralls.io/r/storj-jp/chi?branch=master)

# ![](https://raw.githubusercontent.com/storj-jp/chizer/master/image/chizer.png)  chizer
Encrypted File Detector by Chi Square Test. You can also use this program as Python module.

# Theory outline & evaluation results
Please refer https://github.com/storj-jp/chizer/blob/master/evaluation.md

## Requirement
* gcc
* python3 for using python module

## Install
* for C program
  -  make chizer 
* for python module
  - make python 
* to test
  - make test 

## Usage
* for C program
  - ./chizer "filename" 
* for python module:
  - chizer.isFileEncrypted("filename") returns true if "filename" is judged as encrypted.
  - chizer.isChunksEncrypted("filename") returns true if chunks of "filename" is judged as encrypted.
* to test
  - ./test
  
# Contribution
Feel free to make any improvements, and to pull requests. 

#License
BSD license
