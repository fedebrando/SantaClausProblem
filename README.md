# Santa Claus Problem

First, navigate to the project root directory and compile the project by running the following command:

```shell
make
```

Once compiled, you can select which version to run from the `build` directory.

## v1

In this version, Santa Claus and the reindeer deliver toys together as a single entity, modeled using a Petri net. To run the program from the `build` directory, use the following command.

```shell
./main1
```

The program runs indefinitely, but you can stop it with `Ctrl+C`. You can modify the number of elves and reindeer by adjusting the corresponding macro constant in `src/main1.cpp` and recompiling the project. If you want to display delivery times, uncomment the `#define V1_DELIVERY_TIME_VERBOSE` directive in the same file.

## v2

In this version, Santa Claus and the reindeer deliver toys independently, with downstream synchronization. As with the previous version, you can run the program from the `build` directory using the following command.

```shell
./main2
```

The program runs indefinitely, but you can stop it with `Ctrl+C`. You can modify the number of elves and reindeer by adjusting the corresponding macro constant in `src/main2.cpp` and recompiling the project. If you want to display delivery times, uncomment the `#define V2_DELIVERY_TIME_VERBOSE` directive in the same file.

## v3

This version generalizes version 1 by allowing an arbitrary number of Santa Claus entities. You can specify the number of elves, reindeer, and Santa Claus instances as command-line parameters. Run the following command from the `build` directory.

```shell
./main3 n_reindeer n_elves n_santa
```

The program runs indefinitely, but you can stop it with `Ctrl+C`. If you want to display elves' waiting times, uncomment `#define V3_ELVES_WAIT_TIME_VERBOSE` in `monitor/v3/santa_v3.hpp`. Finally, if you want to display last arrived reindeer's waiting time, uncomment `#define V3_DELIVERY_DELAY_VERBOSE` in the same file.

### Multiple Executions

To compute the average and standard deviation of elves' waiting times when consulting with Santa for various `<n_elves, n_santa>` pairs, use the following command:

```shell
./main3multi
```

**Important:** Before running this program, you must:
- uncomment `#define V3_ELVES_WAIT_TIME_VERBOSE` in `monitor/v3/santa_v3.hpp`;
- set the `VERBOSE` macro constant in `lib/utilities.hpp` to `false`;
- recompile the project to ensure only time values are printed during `main3` iterative executions.

If you want to set a different number of reindeer, modify the `N_REINDEER` macro constant in `src/main3multi.cpp`. In the same file, you can also define the minimum and maximum values for `n_elves`.
