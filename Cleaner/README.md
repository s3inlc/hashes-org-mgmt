# Hashes.org Cleaner

This program is used to find hashes in the Hashes.org left list which are partially from a simple MD5 hash.


## Usage

Command line input:
./Cleaner wordlist.txt output.txt left.txt

Important:
* The wordlist should not be too big, the recommended size is ~10 million lines maximum. (It was possible to run 23mil on my server)
* You need to provide all three file paths.

## Compiling

### Linux

You need to have the Qt libraries installed. Then just run
qmake Cleaner.pro && make
and you're done

### Windows

The easiest way is to install Qt Creator and then open the Cleaner.pro project file. Then just build and run.
