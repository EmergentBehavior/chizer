![](https://travis-ci.org/storj-jp/chi.svg)
[![Coverage Status](https://coveralls.io/repos/storj-jp/chi/badge.svg?branch=master)](https://coveralls.io/r/storj-jp/chi?branch=master)

#Encrypted File Detector by Chi Square Test

##Theory Outline
This is a program that determines specified file is encrypted or not. 
Encrypted files have more random bytes than raw/compressed files. And randomness is measured by 
http://en.wikipedia.org/wiki/Chi-square_test. Putting it simply, if a file has completely randome bytes,
the expectation rate of occurence of data 0x00 is (total bytes)/256, because bytes are between 0 and 255.
if this file will becomes less random, the rate will be far from this value. so the value of

![](http://upload.wikimedia.org/math/1/9/a/19a61a6c2844c76004d17666674c31df.png)

will be bigger.(∑ means to sum up for all rate of occurence of data 0x00,0x01,,,0xff)
So if χ^2 is bigger than certain value, file can be judged as not random,i.e. not encrpyted.

Distribution of χ^2 is called http://en.wikipedia.org/wiki/Chi-squared_distribution, and the probability density 
function is like a figure below.

![](http://www.philender.com/courses/intro/notes3/xdist.gif)

The probability that x^2 value is bigger than (x^2)* in figure above equals to blue area. This probability is called significance level.
When you consider that x^2 is not distributed like figure above(i.e. data is not random) when x^2 value is bigger
than  (x^2)*, the probability that in fact data is random equals blue area.

Normally people uses significance level as 5%. It means files may be misjudged as not encrypted
at 5% rate.

Above figure varies depend of the number of summation. This number is called degrees of freedom(k).
In our case, k=256-1=255. When k=255 and significant level is 5%, (x^2)* =293.247835. 
This was calulated by https://www.sist.ac.jp/~suganuma/cpp/2-bu/7-sho/C++/chi.txt .

##What This Program Does
This program checkes two stages. One calculates chi square deviation for whole file and judge specified file is encrypted for not.For whole file judgement I use 5% significance level mentioned above.

procedure

1. calculate histogram for data 0x00~0xff. i.e. count the number of byte=0x00,0x01...0xff.
2. calculate ![](http://upload.wikimedia.org/math/1/9/a/19a61a6c2844c76004d17666674c31df.png), where observed is the number counted procedure 1, and expected=(all bytes count)/256
3. if x^2<293.247835, file is judged as encrypted.

Another caculates deviation for each chunks whose size is 32 bytes . and I used deviation<512 for detecting suspecious chunks.
And number of suspecious chunks are over 5, this file is judged as not encrpyted.
These parametes are used in article I refered. (It seemds these are introduced heuristically.)

procedure

1. pointer=start of data
2. calculate x^2 for 32 bytes from pointer.
3. if x^2>512, number of suspecious chunks++
4. set pointer to next 32 bytes, and go procedure 1 until pointer<end of data.
5. if number ofsuspecious chunks>=5, file is not encrypted.

## Evaluation
1.First I used http://www.fourmilab.ch/random/ to files that created by https://github.com/Storj/RandomIO, 
and checked the chi square distribution were same as the one of this program. All values were matched.
```
$ ./ent data/1f567965f3b034d819d035cbfa68f4b1 
Entropy = 7.999658 bits per byte.

Optimum compression would reduce the size
of this 500000 byte file by 0 percent.

Chi square distribution for 500000 samples is 237.05, and randomly
would exceed this value 78.36 percent of the times.

Arithmetic mean value of data bytes is 127.5081 (127.5 = random).
Monte Carlo value for Pi is 3.148812595 (error 0.23 percent).
Serial correlation coefficient is -0.001882 (totally uncorrelated = 0.0).
```

* data/1f567965f3b034d819d035cbfa68f4b1(500,000bytes):237.053952
* data/d0d5aadd2e49c38f52261d9b5a3e6d9a(1,00,000bytes):223.445504
* data/5167585a6f04f84378734a59249fc741 (3,000,000bytes):267.375104
* data/bbd39238d1e5368c95ade4554dae9712(5,000,000bytes):275.085312
* data/578419af524bc10e35a4aa27fee31b1b(10,000,000bytes):232.634829

2.I ran this programs to above files, and all files were judged as encrypted.
```
$ ./chi data/1f567965f3b034d819d035cbfa68f4b1 
        Chi square distribution=237.053952
;-)     Whole File is encrypted.
        suspecious blocks:0
;-)     chunks are encrypted.
```

3.I ran this program to https://www.youtube.com/watch?v=vl3bUzfn2lg (Storj Promotion Video), and judged as
not encrypted.
```
$../chi data/Storj\ -\ Decentralizing\ Cloud\ Storage-vl3bUzfn2lg.mp4
        Chi square distribution=231780.126855
:-(     Whole File is NOT encrypted.
        suspecious blocks:3594
:-(     chunks are NOT encrypted.

```

4.I gzipped above mp4, and ran this program, and judged as not encrpyted.

```
$ ./chi data/Storj\ -\ Decentralizing\ Cloud\ Storage-vl3bUzfn2lg.mp4.gz 
        Chi square distribution=3035.437162
:-(     Whole File is NOT encrypted.
        suspecious blocks:183
:-(     chunks are NOT encrypted.
```

5.I bzipped above mp4, and ran this program, and judged as not encrpyted.
```
$ ./chi data/Storj\ -\ Decentralizing\ Cloud\ Storage-vl3bUzfn2lg.mp4.bz2 
        Chi square distribution=1809.816191
:-(     Whole File is NOT encrypted.
        suspecious blocks:134
:-(     chunks are NOT encrypted.
```

6.I made 30MB encrpyted data by RandomIO, and append gzipped mp3 extracted from mp4 above to it, and I generated
mixed data. This mixed data was judged as not encrpyted(fake was NOT succeeded...).
```
$  ./chi data/ac59ab5a282afd3de22062c7d62b5367 
        Chi square distribution=236.723524
;-)     Whole File is encrypted.
        suspecious blocks:0
;-)     chunks are encrypted.

$ ./chi data/Storj\ -\ Decentralizing\ Cloud\ Storage-vl3bUzfn2lg.mp3.gz 
        Chi square distribution=2018.514713
:-(     Whole File is NOT encrypted.
        suspecious blocks:6
:-(     chunks are NOT encrypted.


$ cat RandomIO/ac59ab5a282afd3de22062c7d62b5367 data/Storj\ -\ Decentralizing\ Cloud\ Storage-vl3bUzfn2lg.mp3.gz >> mixed2.dat

$ ./chi  mixed2.dat 
        Chi square distribution=261.202954
;-)     Whole File is encrypted.
        suspecious blocks:6
:-(     chunks are NOT encrypted.
```

graph of chi square distribution is:

![](http://i.imgur.com/G7hWfKu.png)

(x:chuck no from beginng of file, y:distribution)

7.I gzipped the encrypted file, and I ran this program. The result is "encrypted".
```
$./chi data.gz/1f567965f3b034d819d035cbfa68f4b1.gz 
        Chi square distribution=235.491691
;-)     Whole File is encrypted.
        suspecious blocks:0
;-)     chunks are encrypted.

```

##Performance

result of 30MB file.
```
 time ./chi data/ac59ab5a282afd3de22062c7d62b5367 
;-)     Whole File is encrypted.
;-)     chunks are encrypted.

real    0m1.404s
user    0m1.337s
sys     0m0.050s
```
at
* compile :  gcc -o chi chi.c -O3
* kernel: Linux  3.18.4-1-ARCH #1 SMP PREEMPT Tue Jan 27 20:45:02 CET 2015 x86_64 GNU/Linux
* CPU: AMD Athlon(tm) 64 X2 Dual Core Processor 3800+
* memory:3GB

# Contribution
Feel free to make any improvements, and to pull requests. 

#License
BSD license

##Reference
1. http://www.devttys0.com/2013/06/differentiate-encryption-from-compression-using-math/
2. http://www.devttys0.com/2013/06/encryption-vs-compression-part-2/
