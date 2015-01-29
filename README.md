#Determination of Encrypted Files by Chi Square Test

##Outline
This is a program that determins specified file is encrypted or not. 
Encrypted files have more random bytes than raw/compressed files. And randomness is measured by 
http://en.wikipedia.org/wiki/Chi-square_test. Putting it simply, if a file has completely randome bytes,
the expectation rate of occurence of data 0x00 is (total bytes)/256, because bytes are between 0 and 255.
if this file will becomes less random, the rate will be far from this value. i.e. the value of
```math
χ2=(observed-expected)2/expected
```
will be bigger. So if χ2 is bigger than certain value, file can be judged as not random,i.e. not encrpyted.
This threshold is defined by significance level, which is usually 5%. It means files may be misjudged as not encrypted
at 5% rate.

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
        file size=500000 bytes
        Chi square distribution=237.053952
        significance level:0.050000
;-)     encrypted.

```

3.I ran this program to https://www.youtube.com/watch?v=vl3bUzfn2lg (Storj Promotion Video), and judged as
not encrypted.
```
$./chi data/Storj\ -\ Decentralizing\ Cloud\ Storage-vl3bUzfn2lg.mp4
        file size=8328756 bytes
        Chi square distribution=231780.126855
        significance level:0.000000
:-(     NOT encrypted.
```

4.I gzipped above mp4, and ran this program, and judged as not encrpyted.

```
$ ./chi data/Storj\ -\ Decentralizing\ Cloud\ Storage-vl3bUzfn2lg.mp4.gz 
data/Storj - Decentralizing Cloud Storage-vl3bUzfn2lg.mp4.gz
        Chi square distribution=3035.437162
        significance level:0.000000
:-(     NOT encrypted.
```

5.I bzipped above mp4, and ran this program, and judged as not encrpyted.
```
$ ./chi data/Storj\ -\ Decentralizing\ Cloud\ Storage-vl3bUzfn2lg.mp4.bz2 
        file size=8282494 bytes
        Chi square distribution=1809.816191
        significance level:0.000000
:-(     NOT encrypted.
```

6.I made 30MB encrpyted data by RandomIO, and append gzipped mp3 extracted from mp4 above to it, and I generated
mixed data. This mixed data was judged as encrpyted(unfortunately fake was succeeded...).
```
$ ./chi RandomIO/ac59ab5a282afd3de22062c7d62b5367 
        file size=30000000 bytes
        Chi square distribution=236.723524
        significance level:0.050000
;-)     encrypted.

$ ./chi data/Storj\ -\ Decentralizing\ Cloud\ Storage-vl3bUzfn2lg.mp3.gz 
        file size=1086301 bytes
        Chi square distribution=2018.514713
        significance level:0.000000
:-(     NOT encrypted.

$ cat RandomIO/ac59ab5a282afd3de22062c7d62b5367 data/Storj\ -\ Decentralizing\ Cloud\ Storage-vl3bUzfn2lg.mp3.gz >> mixed2.dat

$ ./chi mixed2.dat 
        file size=31086301 bytes
        Chi square distribution=261.202954
        significance level:0.050000
;-)     encrypted.

```



##Reference
1. http://www.devttys0.com/2013/06/differentiate-encryption-from-compression-using-math/
2. http://www.devttys0.com/2013/06/encryption-vs-compression-part-2/
