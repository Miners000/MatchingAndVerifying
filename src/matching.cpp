#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cstdlib>

using namespace std;

class StableMatching 
{
    private:
        int n;
        vector<vector<int>> hospitalPrefs;
        vector<vector<int>> studentPrefs;
        vector<int> hospitalMatch;
        vector<int> studentMatch;
        vector<vector<int>> studentRanking;
        vector<int> nextProposal;
        int proposalCount;

    public:
        StableMatching(int size)
        {
            n = size;
            proposalCount = 0;
            hospitalPrefs.resize(n, vector<int>(n));
            studentPrefs.resize(n, vector<int>(n));
            hospitalMatch.resize(n, -1);
            studentMatch.resize(n, -1);
            studentRanking.resize(n, vector<int>(n));
            nextProposal.resize(n, 0);
        }

        void readInput(istream& input)
        {
            for(int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    input >> hospitalPrefs[i][j];
                    hospitalPrefs[i][j]--;
                }
            }

            for(int i = 0; i < n; i++)
            {
                for(int j = 0; j < n; j++)
                {
                    input >> studentPrefs[i][j];
                    studentPrefs[i][j]--;
                }
            }

            for(int i = 0; i < n; i++)
            {
                for(int rank = 0; rank < n; rank++)
                {
                    int hospital = studentPrefs[i][rank];
                    studentRanking[i][hospital] = rank;
                }
            }
        }

        void runAlgorithm()
        {
            while (true) 
            {
                int freeHospital = -1;
                for(int i = 0; i < n; i++)
                {
                    if (hospitalMatch[i] == -1 && nextProposal[i] < n)
                    {
                        freeHospital = i;
                        break;
                    }
                }

                if (freeHospital == -1)
                {
                    break;
                }

                int student = hospitalPrefs[freeHospital][nextProposal[freeHospital]];
                nextProposal[freeHospital]++;
                proposalCount++;

                if(studentMatch[student] == -1)
                {
                    hospitalMatch[freeHospital] = student;
                    studentMatch[student] = freeHospital;
                }
                else
                {
                    int currentHospital = studentMatch[student];
                    
                    if (studentRanking[student][freeHospital] < studentRanking[student][currentHospital])
                    {
                        hospitalMatch[currentHospital] = -1;
                        hospitalMatch[freeHospital] = student;
                        studentMatch[student] = freeHospital;
                    }
                }
            }
        }

        void writeOutput(ostream& output)
        {
            for (int i = 0; i < n; i++)
            {
                output << (i + 1) << " " << (hospitalMatch[i] + 1) << "\n";
            }
        }

        void writeStats(ostream& output)
        {
            output << "Number of proposals: " << proposalCount << "\n";
        }

        int getProposalCount() const
        {
            return proposalCount;
        }

        const vector<int>& getHospitalMatch() const
        {
            return hospitalMatch;
        }

        const vector<int>& getStudentMatch() const
        {
            return studentMatch;
        }

        const vector<vector<int>>& getHospitalPrefs() const
        {
            return hospitalPrefs;
        }

        const vector<vector<int>>& getStudentPrefs() const
        {
            return studentPrefs;
        }

        const vector<vector<int>>& getStudentRanking() const
        {
            return studentRanking;
        }

        int getN() const 
        {
            return n;
        }
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <inputFile> [outputFile]" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile)
    {
        cerr << "Error: Cannot open input file " << argv[1] << endl;
        return 1;
    }

    int n;
    inputFile >> n;

    StableMatching matching(n);
    matching.readInput(inputFile);
    inputFile.close();
    auto start = chrono::high_resolution_clock::now();
    matching.runAlgorithm();
    auto end = chrono::high_resolution_clock::now();
    
    chrono::duration<double, micro> duration = end - start;

    if (argc >= 3)
    {
        ofstream output_file(argv[2]);
        matching.writeOutput(output_file);
        output_file.close();
    }
    else
    {
        matching.writeOutput(cout);
    }

    cout << "n = " << n << ", ";
    cout << "Proposals = " << matching.getProposalCount() << ", ";
    cout << "Time = " << duration.count() << " microseconds" << endl;

    return 0;
}