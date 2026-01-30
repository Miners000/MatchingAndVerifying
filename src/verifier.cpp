#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cerr << "Usage: " << argv[0] << " <inputFile> <matchingFile>" << endl;
		return 1;
	}

	ifstream inputFile(argv[1]);
	if (!inputFile)
	{
		cerr << "Error: Cannot open input file " << argv[1] << endl;
		return 1;
	}

	ifstream matchFile(argv[2]);
	if (!matchFile)
	{
		cerr << "Error: Cannot open matching file " << argv[2] << endl;
		return 1;
	}

	int n;
	if (!(inputFile >> n) || n <= 0)
	{
		cout << "Invalid: Invalid or non-positive n" << endl;
		return 1;
	}

	auto start = chrono::high_resolution_clock::now();

	vector<vector<int>> hospitalPrefs(n, vector<int>(n));
	vector<vector<int>> studentPrefs(n, vector<int>(n));

    // hospital preference lists
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (!(inputFile >> hospitalPrefs[i][j]))
			{
				cout << "Invalid: malformed hospital preference list" << endl;
				return 1;
			}
			hospitalPrefs[i][j]--;
			if (hospitalPrefs[i][j] < 0 || hospitalPrefs[i][j] >= n)
			{
				cout << "Invalid: hospital preference index out of range" << endl;
				return 1;
			}
		}
	}

	// student preference lists
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (!(inputFile >> studentPrefs[i][j]))
			{
				cout << "Invalid: malformed student preference list" << endl;
				return 1;
			}
			studentPrefs[i][j]--;
			if (studentPrefs[i][j] < 0 || studentPrefs[i][j] >= n)
			{
				cout << "Invalid: student preference index out of range" << endl;
				return 1;
			}
		}
	}

	inputFile.close();

	// ranking arrays for O(1) comparison.
	vector<vector<int>> hospitalRank(n, vector<int>(n));
	vector<vector<int>> studentRank(n, vector<int>(n));
	for (int h = 0; h < n; h++)
	{
		for (int rank = 0; rank < n; rank++)
		{
			int s = hospitalPrefs[h][rank];
			hospitalRank[h][s] = rank;
		}
	}
	for (int s = 0; s < n; s++)
	{
		for (int rank = 0; rank < n; rank++)
		{
			int h = studentPrefs[s][rank];
			studentRank[s][h] = rank;
		}
	}

	// proposed matching
	vector<int> hospitalMatch(n, -1); // hospital -> student
	vector<int> studentMatch(n, -1);  // student -> hospital

	int h, s;
	int pairCount = 0;
	while (matchFile >> h >> s)
	{
		pairCount++;
		if (h < 1 || h > n || s < 1 || s > n)
		{
			cout << "Invalid: hospital or student index out of range in matching" << endl;
			return 1;
		}

		int hospitalIndex = h - 1;
		int studentIndex = s - 1;

		if (hospitalMatch[hospitalIndex] != -1)
		{
			cout << "Invalid: hospital " << h << " appears more than once in matching" << endl;
			return 1;
		}
		if (studentMatch[studentIndex] != -1)
		{
			cout << "Invalid: student " << s << " appears more than once in matching" << endl;
			return 1;
		}

		hospitalMatch[hospitalIndex] = studentIndex;
		studentMatch[studentIndex] = hospitalIndex;
	}

	matchFile.close();

	if (pairCount != n)
	{
		cout << "Invalid: matching must contain exactly " << n << " pairs" << endl;
		return 1;
	}

	for (int i = 0; i < n; i++)
	{
		if (hospitalMatch[i] == -1)
		{
			cout << "Invalid: hospital " << (i + 1) << " is unmatched" << endl;
			return 1;
		}
		if (studentMatch[i] == -1)
		{
			cout << "Invalid: student " << (i + 1) << " is unmatched" << endl;
			return 1;
		}
	}

	// check for no blocking pairs
	for (int hospitalIndex = 0; hospitalIndex < n; hospitalIndex++)
	{
		int sCurrent = hospitalMatch[hospitalIndex];
		for (int studentIndex = 0; studentIndex < n; studentIndex++)
		{
			if (studentIndex == sCurrent)
			{
				continue; // already matched pair
			}

			int hCurrent = studentMatch[studentIndex];

			// does hospital h prefer this student over its current match?
			bool hospitalPrefers = hospitalRank[hospitalIndex][studentIndex] < hospitalRank[hospitalIndex][sCurrent];
			// does student s prefer this hospital over its current match?
			bool studentPrefers = studentRank[studentIndex][hospitalIndex] < studentRank[studentIndex][hCurrent];

			if (hospitalPrefers && studentPrefers)
			{
				cout << "Unstable: blocking pair (hospital " << (hospitalIndex + 1)
					 << ", student " << (studentIndex + 1) << ")" << endl;
				return 1;
			}
		}
	}

	cout << "VALID STABLE" << endl;
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double, micro> duration = end - start;
	cout << "Verifier time = " << duration.count() << " microseconds" << endl;
	return 0;
}

