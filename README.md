
# Santa Claus Problem
Let us indicate the project root directory with `/`.

## Petri net models
The folder `/Petri_net_models` includes the Petri net models (actually, GSPN and DTPN models) reported in the paper <i>Exposing the Concurrency of the Santa Claus Problem</i>, submitted for publication by F. Brandini, A. Mazzocchi and S. Caselli.

All models in format .PNPRO can be open and analyzed with the tool GreatSPN3.1, available at [this repo](https://github.com/greatspn/SOURCES).

Last compatibility check: July 6, 2025

## Timed activities
All timed activities occurring in the Santa Claus Problem  are implemented as random durations with values drawn from negative exponential distributions. The samples come from the <i>exp_dis(avg)</i> function described at `/src/lib/utilities.hpp`; this function samples a negative exponential distribution with average <i>avg</i> and returns the sampled value.

## Run
First, navigate to the project root directory and compile the project by running the following command:

```shell
make
```

Once compiled, you can select which version to run from the `/build/bin` directory.

### v1

In this version, Santa Claus and the reindeer deliver toys together as a single entity, modeled using a Petri net. To run the program from the `/build/bin` directory, use the following command:

```shell
./main1
```

The program runs indefinitely, but you can stop it with `Ctrl+C`. You can modify the number of elves and reindeer by adjusting the corresponding macro constants in `/src/main1.cpp` and recompiling the project. You can also display some additional information as indicated below:
- To display **delivery times** (<i>ms</i>), uncomment the `#define V1_DELIVERY_TIME_VERBOSE` directive in `/src/main1.cpp`.
- To display **delivery delays** (<i>ms</i>), uncomment the `#define V1_DELIVERY_DELAY_VERBOSE` directive in `/src/main1.cpp`.
- To display **Santa's state** when all reindeer arrive, uncomment the `#define V1_SANTA_STATE_FOR_DELIVERY` directive in `/src/monitor/v1/santa_v1.hpp`.

### v2

In this version, Santa Claus and the reindeer deliver toys independently, with downstream synchronization. As with the previous version, you can run the program from the `/build/bin` directory using the following command:

```shell
./main2
```

The program runs indefinitely, but you can stop it with `Ctrl+C`. You can modify the number of elves and reindeer by adjusting the corresponding macro constants in `/src/main2.cpp` and recompiling the project. If you want to display **delivery times** (<i>ms</i>), uncomment the `#define V2_DELIVERY_TIME_VERBOSE` directive in the same file.

### v3

This version generalizes version 1 by allowing an arbitrary number of Santa Claus entities. You can specify the number of elves, reindeer, and Santa Claus instances as command-line parameters. Run the following command from the `/build/bin` directory:

```shell
./main3 n_reindeer n_elves n_santa
```

The program runs indefinitely, but you can stop it with `Ctrl+C`. You can also display some additional information as indicated below:
- To display **delivery delays** (<i>ms</i>), uncomment the `#define V3_DELIVERY_DELAY_VERBOSE` directive in `/src/monitor/v3/santa_v3.hpp`.
- To display **Santa's state** when all reindeer arrive, uncomment the `#define V3_SANTA_STATE_FOR_DELIVERY` directive in `/src/monitor/v3/santa_v3.hpp`.
- To display **elves' waiting times** (<i>ms</i>), uncomment `#define V3_ELVES_WAIT_TIME_VERBOSE` in `/src/monitor/v3/santa_v3.hpp`.

### Docs
The complete statistics are reported in `/stats/stats.xlsx`, where you can also find the documentation about the system used to test the solutions in `Info` section.
You can find the documentation related to the C++ <i>high_resolution_clock</i> function at [this link](https://en.cppreference.com/w/cpp/chrono/high_resolution_clock).
