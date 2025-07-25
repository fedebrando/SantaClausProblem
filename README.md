
# Santa Claus Problem
Let us indicate the project root directory with `/`.

## Petri net models
The folder `/Petri_net_models` includes the Petri net models (actually, GSPN and DTPN models) reported in the paper <i>Exposing the Concurrency of the Santa Claus Problem</i>, submitted for publication by [F. Brandini](https://github.com/fedebrando), [A. Mazzocchi](https://github.com/mattoszky) and [S. Caselli](https://github.com/SCaselli).

All models in format .PNPRO can be open and analyzed with the tool GreatSPN3.1, available [here](https://github.com/greatspn/SOURCES).

Last compatibility check: July 6, 2025

## Timed activities
All timed activities occurring in the Santa Claus Problem  are implemented as random durations with values drawn from negative exponential distributions. The samples come from the **<i>exp_dis(avg)</i>** function described at `/src/lib/utilities.hpp`; this function samples a negative exponential distribution with average <i>avg</i> and returns the sampled value.

In the code, this function is never called directly, but through the **<i>log(str, avg)</i>** function. It represents an activity executed by an entity (e.g., an elf): it prints the string <i>str</i> on the console and waits for <i>exp_dis(avg)</i>. For more details, consult `/src/lib/utilities.hpp`. **To disable log prints** go to the same file and set to `false` the value of the constant `VERBOSE` (this setting does not act on stochastic times).

## Run
First, navigate to the project root directory and compile the project by running the following command:

```shell
make
```

Once compiled, you can select which version to run from the `/build/bin` directory.

### v1

In this version, **Santa Claus and the reindeer deliver toys together** as a single entity, modeled using a Petri net. To run the program from the `/build/bin` directory, use the following command:

```shell
./main1
```

The program runs indefinitely, but you can stop it with `Ctrl+C`. You can modify the number of elves and reindeer by adjusting the corresponding macro constants in `/src/main1.cpp` and recompiling the project. You can also display some additional information as indicated below.
- To display **delivery times** (<i>ms</i>), uncomment the `#define V1_DELIVERY_TIME_VERBOSE` directive in `/src/main1.cpp`
- To display **delivery delays** (<i>ms</i>), uncomment the `#define V1_DELIVERY_DELAY_VERBOSE` directive in `/src/main1.cpp`
- To display **Santa's state** when all reindeer arrive, uncomment the `#define V1_SANTA_STATE_FOR_DELIVERY` directive in `/src/monitor/v1/santa_v1.hpp`

For a **single-core run**, uncomment the `#define SINGLE_CORE` directive in `/src/main1.cpp`.

To view the pseudocode for this monitor version, see the file `/pseudo/pseudo1.md`.

### v2

In this version, **Santa Claus and the reindeer deliver toys independently**, with downstream synchronization. As with the previous version, you can run the program from the `/build/bin` directory using the following command:

```shell
./main2
```

The program runs indefinitely, but you can stop it with `Ctrl+C`. You can modify the number of elves and reindeer by adjusting the corresponding macro constants in `/src/main2.cpp` and recompiling the project. If you want to display **delivery times** (<i>ms</i>), uncomment the `#define V2_DELIVERY_TIME_VERBOSE` directive in the same file.

For a **single-core run**, uncomment the `#define SINGLE_CORE` directive in `/src/main2.cpp`.

To view the pseudocode for this monitor version, see the file `/pseudo/pseudo2.md`.

### v3

This version **generalizes v1 by allowing an arbitrary number of Santa Claus entities**. You can specify the number of elves, reindeer, and Santa Claus instances as command-line parameters. Run the following command from the `/build/bin` directory:

```shell
./main3 n_reindeer n_elves n_santa
```

The program runs indefinitely, but you can stop it with `Ctrl+C`. You can also display some additional information as indicated below.
- To display **delivery delays** (<i>ms</i>), uncomment the `#define V3_DELIVERY_DELAY_VERBOSE` directive in `/src/monitor/v3/santa_v3.hpp`
- To display **Santa's state** when all reindeer arrive, uncomment the `#define V3_SANTA_STATE_FOR_DELIVERY` directive in `/src/monitor/v3/santa_v3.hpp`
- To display **elves' waiting times** (<i>ms</i>), uncomment `#define V3_ELVES_WAIT_TIME_VERBOSE` in `/src/monitor/v3/santa_v3.hpp`

For a **single-core run**, uncomment the `#define SINGLE_CORE` directive in `/src/main3.cpp`.

To view the pseudocode for this monitor version, see the file `/pseudo/pseudo3.md`.

## Condition variables
In all three versions, Santa Claus waits for a service request on the condition variable `await_someone`. Once he is woken by a group of clients (elves or reindeer), he waits for the elves to be accommodated or the reindeer to be harnessed to the sleigh on `wait_all_passed`, and then proceeds to execute the service (consultation or toy delivery). Once the service is completed, Santa waits for greetings from the clients on the condition variable `wait_greetings`. In version **v1**, each Santa Claus will have these three condition variables, while, in version **v2**, during the execution of the service, Santa has to wait for the clients to finish their operations on `wait_clients_finish` before proceeding to greetings.

From the client’s perspective, after sending a service request to Santa Claus, an elf or a reindeer waits for the service to begin on its condition variable `wait_service`. Waiting for the end of the service takes place on `wait_end_of_service`.

## Statistics and documentation
The complete statistics are reported in `/stats/stats.xlsx`, where you can also find the documentation about the system used to test the solutions in `Info` section.
The documentation related to the C++ <i>high_resolution_clock()</i> function, which is used for all time measurments in the code, is available [here](https://en.cppreference.com/w/cpp/chrono/high_resolution_clock).
