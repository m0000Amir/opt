# program to solve following problem:

Tractors ride only by asphalt undirected roads on the planet Barsoom. While moving, tractors destroy roadbed under themselves. A tractor Harrison made a trip from the Capital to Zodanga city.

Denote by `rb` and `ra` the minimal road width on the Barsoom before and after this trip correspondingly. We know that `rb = ra`.
What values can Harrison‘s width take?

The width of every road is known. We expect that you provide a C++ code for the solution of this problem. This code should be written based on C++14.

All the code should be contained in a single file. Also, we expect algorithm explanation (with prove of correctness and complexity estimation).

You should take into account that Barsoom planet can have a rich number of roads.
The format of data file with roadmap is up to you.
You don’t need to use any libraries except for input/output, random and sorting.

Roads widths are positive integer numbers;
- number of cities <= 10^6;
- number of cities <= number of roads <= 10^6.

Provide generation of a sample input with any number of cities and roads in your code (roads’ placement and their capacities (width) should be generated randomly). The code should also contain a test function, which reads the data file, calls your solution method, and returns the result.


The result of compilation should be console application (e.g. opt.exe).

## How to run
### Build
```bash
clang++ -std=c++14 -o opt.exe main.cpp
```
### Run

1) `command` -b - Generate graph with `numCities` nodes and `numRoads` roads and write to `filename`

```shell
./opt.exe -b <numCities> <numRoads> <fileName>
```

2) `command` -t - Test graph from `filename` with ID of Capital equal to `capitalId` and ID of Zodanga equal to `ZodangaID`

```shell
./opt.exe -b <capitalId> <ZodangaID> <fileName>
```

## Notes:

```cpp
void reconstructWidestPath(Graph& graph, Path& path)
```
Function reconstructs widest path from capital to Zodanga with widths of roads. To call the function `bool isReconstruct` must be `true` (default false). Only small size problems are preferred.

Example - Capital Id is 3, Zodanga Id is 4. Widest path is
```
(3) -45- (0) -83- (2) -49- (4) 
```



 
