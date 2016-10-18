# Time Series Indexing - LABD

This is a project made for educational purpose for the course of [Laboratory on Algorithms for Big Data] ( University of Pisa ) 2016/17.

### Libraries
* [Cereal] - serialization library
* [CMake] - family of tools designed to build, test and package software

### Usage
Generate a makefile and build project:
```sh
$ cmake CMakeLists.txt
$ make
```
The executables will be placed in `bin/`.  

Data serialization:
```sh
$ test_serialize path/to/dataset output.bin
```
Data querying:
```sh
$ test_querying path/to/output.bin
```
Random queries maker:
```sh
$ queries_maker numberOfQueries K_min K_max path/to/dataset out_queries
```
Where **K_min** and **K_max** reppresent the range of K values, and **out_queries** the result file.

### Autors
Alessandro Romano  
Paolo Grossi

[Laboratory on Algorithms for Big Data]:<http://pages.di.unipi.it/rossano/221-2/laboratory-on-algorithms-for-big-data-a-a-201617/>
[Cereal]: <http://uscilab.github.io/cereal/index.html>
[CMake]: <https://cmake.org/>
