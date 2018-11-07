# Time Series Indexing - LABD
This project has been made for educational purpose during lecture of [Laboratory on Algorithms for Big Data] ( University of Pisa ) 2016/17.

## Libraries
* [Cereal] - Serialization library
* [SDSL] - Succinct Data Structures Library
* [G-Test] - Google Unit Test

## Tech
* [CMake] - Family of tools designed to build, test and package software

## Usage
Generate a makefile and build project:
```sh
$ mkdir _build
$ cd _build
$ cmake ..
$ make
```
The executables will be placed in `_build/bin/`.  

### Indices
There are two different implementations, each of them is represented by an index:
* **0** : **Baseline** implementation
* **1** : Implementation with **succinct data structures**  

### Data serialization
```sh
$ cd _build/bin/
$ ./build_index id path/to/dataset
```
The object is saved in the same folder where the input dataset is located, and named like `datases+index`.

### Build query sets
```sh
$ ./build_query_sets path/to/dataset num_of_query min_date_interval max_k
```
This one builds `(min_date_interval\100)*max_k` different set of queries each having a different size of `Range` and `K`.
If there is a serialized data structure with `id=0`, the creation will be faster because it won't need to populate any structure from file.  

### Run queries
```sh
$ ./run_queries id path/to/dataset path/to/query_set
```
## Test
Test index=0 implementation:
```sh
$ ./test_baseline
```
Test index=1 implementation:
```sh
$ ./test_index1
```  
## Author
Alessandro Romano

[Laboratory on Algorithms for Big Data]: <http://pages.di.unipi.it/rossano/221-2/laboratory-on-algorithms-for-big-data-a-a-201617/>
[Cereal]: <http://uscilab.github.io/cereal/index.html>
[CMake]: <https://cmake.org/>
[SDSL]: <https://github.com/simongog/sdsl-lite>
[G-Test]: <https://github.com/google/googletest>
