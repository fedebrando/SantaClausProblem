# Santa Claus Problem
First of all, go to project root directory and compile project launching this command.

```shell
make
```
Now, you can select which version to run in `build` directory.

## v1
In this version, Santa Claus and reindeer deliver toys together as a single entity in a Petri net view. For running you can launch program in this simple way from `build` directory.
```shell
./main1
```
Once launched, the program doesn't stop. However, you can stop it by `Ctrl+C` command. If you want, you can change number of elves and reindeer by setting relative macroconstant in `src/main1.cpp` and compiling project again. Finally, if you want show delivery timing values, you must uncomment `#define STATS_VERBOSE_V1` in the same file.

## v2
In this version, Santa Claus and reindeer deliver toys independently with downstream synchronization. As you can see before, you can launch program in this way from `build` directory.
```shell
./main2
```
Once launched, the program doesn't stop. However, you can stop it by `Ctrl+C` command. As in the previous version, you can change number of elves and reindeer by setting relative macroconstant in `src/main2.cpp` and compiling project again. Finally, if you want show delivery timing values, you must uncomment `#define STATS_VERBOSE_V2` constant in the same file.

## v3
This is a generalization of version 1 with an arbitrary number of Santa Claus. You can also set the number of elves and reindeer by command-line parameters. From `build` directory again, lauch this command.
```shell
./main3 n_reindeer n_elves n_santa
```
Once launched, the program doesn't stop. However, you can stop it by `Ctrl+C` command. If you want show elves wait timing values, you must uncomment `#define WAIT_VERBOSE` in `monitor/v3/santa_v3.hpp`.

### Several execution
For computing average and standard deviation of elves waiting times for consulting with Santa for several couples <i>(n_elves, n_santa)</i>, you can launch this program. <b>Make sure that for the correct execution of it, you must uncomment `#define WAIT_VERBOSE` in `monitor/v3/santa_v3.hpp`, set to `false` the `VERBOSE` macroconstant in `lib/utilities.hpp` and compile project again to print time values only during the `main3` iterative executions.</b>
```shell
./main3multi
```
If you want fix another value for reindeer number, you must set `N_REINDEER` macroconstant in `src/main3multi.cpp` source file. In the same file, you can also choose the minimum and the maximum values for n_elves varying.
