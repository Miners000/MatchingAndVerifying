# MatchingAndVerifying

## Authors

- Hemanshu Boppana (UFID: 74149423)
- Trent Ford (UFID: 80027867)

## Files

- Matcher source: src/matching.cpp
- Verifier source: src/verifier.cpp
- Generator source (for Task C): src/generator.cpp
- Example input: data/example.in
- Example matcher output: results/example.out (generated)

## Build Instructions

Requirements:
- C++17 compiler (e.g., g++).

From the repository root, on any system with g++:

```bash
g++ -std=c++17 -O2 -Wall src/matching.cpp -o bin/matching_engine
g++ -std=c++17 -O2 -Wall src/verifier.cpp -o bin/verifier
g++ -std=c++17 -O2 -Wall src/generator.cpp -o bin/generator
```

On Windows PowerShell, use `\` instead of `/` in paths.

## Input Format

Preference file (used by matcher and verifier):
- Line 1: integer `n` (number of hospitals and students).
- Next `n` lines: hospital preference lists, each a permutation of `1..n`.
- Next `n` lines: student preference lists, each a permutation of `1..n`.

## Running the Matcher

Command:

```bash
./bin/matching_engine <inputFile> <outputFile>
```

Example (reproduce example output):

```bash
./bin/matching_engine data/example.in results/example.out
```

Matcher output:
- `n` lines, each: `i j` meaning hospital `i` is matched to student `j`.
- One extra line on stdout with `n`, number of proposals, and runtime in microseconds.

## Running the Verifier

Command:

```bash
./bin/verifier <inputFile> <matchingFile>
```

Example (check example output):

```bash
./bin/verifier data/example.in results/example.out
```

Verifier output:
- On success: `VALID STABLE` and a timing line.
- On error: `Invalid: ...` or `Unstable: ...` with a reason.

## Assumptions / Dependencies

- One-to-one market: exactly `n` hospitals and `n` students.
- Preference lists are complete and strict (each line is a permutation of `1..n`).
- No external libraries; only standard C++17.

## Task C: Scalability

1. Generate random instances for selected `n` values (e.g., 1, 2, 4, 8, 16, 32, 64, 128, 256, 512):

Example generation:

	 ```bash
	 ./bin/generator 32 data/n32.in
	 ```

2. For each `n`:
	 - Run the matcher:

		 ```bash
		 ./bin/matching_engine data/n32.in results/n32.out
		 ```

		 Record the `Time = ... microseconds` value.

	 - Run the verifier:

		 ```bash
		 ./bin/verifier data/n32.in results/n32.out
		 ```

		 Record `Verifier time = ... microseconds`.

## Graph of Results
Recorded results and plotted on a graph. Should show a graph of O(n^2).

(Graph HERE)
