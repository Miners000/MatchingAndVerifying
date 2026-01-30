#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " <n> <outputFile>" << endl;
        return 1;
    }

    int n = stoi(argv[1]);
    if (n <= 0)
    {
        cerr << "Error: n must be positive" << endl;
        return 1;
    }

    ofstream out(argv[2]);
    if (!out)
    {
        cerr << "Error: Cannot open output file " << argv[2] << endl;
        return 1;
    }

    out << n << "\n";

    // random setup
    random_device rd;
    mt19937 gen(rd());

    vector<int> base(n);
    for (int i = 0; i < n; ++i)
    {
        base[i] = i + 1; // 1..n
    }

    // hospitals' preference lists
    for (int h = 0; h < n; ++h)
    {
        vector<int> prefs = base;
        shuffle(prefs.begin(), prefs.end(), gen);
        for (int j = 0; j < n; ++j)
        {
            out << prefs[j] << (j + 1 == n ? '\n' : ' ');
        }
    }

    // students' preference lists
    for (int s = 0; s < n; ++s)
    {
        vector<int> prefs = base;
        shuffle(prefs.begin(), prefs.end(), gen);
        for (int j = 0; j < n; ++j)
        {
            out << prefs[j] << (j + 1 == n ? '\n' : ' ');
        }
    }

    return 0;
}
