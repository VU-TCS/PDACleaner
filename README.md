# PDACleaner

PDACleaner detects and removes useless transitions (i.e. transitions that are not used in any terminating run)
from a Pushdown Automaton (PDA). It does so by using the algorithm presented in the following paper.

> D. Grune, W. Fokkink, E. Chatzikalymnios, B. Hond, and P. Rutgers. Detecting Useless Transitions n Pushdown Automata. Language and Automata Theory and Applications, 2017.

Note that PDACleaner is not the prototype implementation mentioned in the paper.

## Usage

The `samples` directory contains some examples of PDAs that can be used as input for PDACleaner. Follow the same template when creating your own input. The directory `samples/reference` contains the expected output for each of the provided PDAs.
Note that when using the `samples/C_Grammar.txt` input file, the `-i` option needs to be passed to PDACleaner.

In order to use the tool, clone this reposiroty and run the following.
```sh
$ cd PDACleaner
$ make                                                  # compile the source code
$ bin/pdacleaner [options] input_file [output_file]     # run the tool with your desired input
```

##### Options
The available options are:

Option  |   Description                                         |
--------|-------------------------------------------------------|
-h      |   print usage message                                 |
-c      |   print a cleaned version of the input PDA            |
-i      |   use multi-character identifiers as stack symbols    |

## Requirements

You can adjust the `Makefile` to use your preferred compiler and flags. PDACleaner has been tested against:
- `g++ (GCC) 6.3.1`, and
- `clang++ (clang) version 3.9.1`

If you want to alter the code, you will need the [Google Test](https://github.com/google/googletest) utility to run unit tests. Tests can be run as follows.
```sh
$ cd PDACleaner
$ make                                                  # compile the source code
$ make test                                             # compile the tests
$ test/run                                              # run all unit tests
```

