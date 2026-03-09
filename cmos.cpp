#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <iomanip>

using namespace std;

struct KGramHash {
    unsigned long long hash;
    int position;
};

struct Fingerprint {
    unsigned long long minimum_hash;
    int position;
};

struct FileData {
    int fileID;
    vector<int> tokens;
    vector<KGramHash> k_mers_hashes;
    vector<Fingerprint> fingerprints;
};

struct Score {
    int fileID;
    double similarityScore;
    int comparisonFile;
};

/// @brief This function will read in all of the tokens for a text file and place them in a vector for future use
/// @param fileTokens This string is all of the tokens from a file on one line from tokens.txt
/// @param fileID Currently this is an int that represents the file from the directory since file names may repeat
/// @return A vector of the tokens from a file
vector<int> readInTokens(string fileTokens)
{
    vector<int> tokens;
    stringstream fileLine(fileTokens);
    int token;
    string filename;

    ///consume file name
    fileLine >> filename;

    while(fileLine >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}

/// @brief Using a vector of tokens, it will hash all of the tokens into one k-mer hash 
/// @param tokens Vector of int file tokens
/// @param k Size of the overlapping k-mers, currently 4
/// @return A vector of hashes for the k-mers
vector<KGramHash> hashKMers(const vector<int>& tokens, int k)
{
    vector<KGramHash> hashes;
    const unsigned long long hashBase = 31;

    // ///quick sanity check to make sure tokens and k are appropiate sizes. 
    // if ((int)tokens.size() < k || k <= 0) {
    //     return hashes;
    // }

    for(int i = 0; i <= (int)tokens.size() - k; i++)
    {
        unsigned long long hash = 0;
        ///This will hash the k-mers
        for(int j = 0; j < k; ++j)
        {
            hash = hashBase * hash + tokens[i+j];
        }
        hashes.push_back({hash, i});
    }

    return hashes;
}

/// @brief This function will perform the winnowing algorithm on the k-mer hashes
/// @param hashes This contains all of the hashes for the tokens of a certain file
/// @param w This is the set window size, currently it is set at 4
/// @return Unsing the winnowing algorthim it will return a vector of the unique fingerprints for the overlapping windows of a file
vector<Fingerprint> winnowingAlgorithm(const vector<KGramHash>& hashes, int w)
{
    vector<Fingerprint> fingerprints;

    ///quick sanity check to make sure hashes and w are appropiate sizes for winnowing 
    // if ((int)hashes.size() < w || w <= 0) {
    //     return fingerprints;
    // }

    /// This loop will find all the unique fingerprint among the overlapping windows
    int last_index = -1;
    for(int i = 0; i <= (int)hashes.size() - w; i++)
    {
        ///Track the smallest hash found and its position
        unsigned long long smallest = hashes[i].hash;
        int smallest_index = i;
        ///this will find the smallest has in a window
        for(int j = 0; j < w; ++j)
        {
            ///This will check to see if a hash is smaller or equal to the current smallest which may be the starting index
            if(hashes[i + j].hash < smallest || (hashes[i + j].hash == smallest && (i + j) > smallest_index))
            {
                smallest = hashes[i+j].hash;
                smallest_index = i + j;
            }
        }
        ///if the new fingerprint is not the same as the last add it
        if(smallest_index != last_index)
        {
            fingerprints.push_back({smallest, hashes[smallest_index].position});
            last_index = smallest_index;
        }
    }

    return fingerprints;
}

/// @brief This function will find the similarity score between two file fingerprints
/// @param a File a's vectors of fingerprints
/// @param b File b's vectors of fingerprints
/// @return A double representing the percent of similarity 
double findSimilarity(const vector<Fingerprint>& a, const vector<Fingerprint>& b)
{
    unordered_set<unsigned long long> setA;
    unordered_set<unsigned long long> setB;
    int shared_count = 0;

    for(int i = 0; i < a.size(); ++i)
    {
        setA.insert(a[i].minimum_hash);
    }

    for(int i = 0; i < b.size(); ++i)
    {
        setB.insert(b[i].minimum_hash);
    }

    ///Find the shared fingerprints - Intersection of the two sets
    for(const auto& hash : setA){
        if(setB.count(hash))
        {
            shared_count++;
        }
    }

    ///find total unique fingerprints - Union of the two sets
    int union_count = setA.size() + setB.size() - shared_count;
    if(union_count == 0)
    {
        return 0.0;
    }
    double similarity = (static_cast<double>(shared_count) / union_count) * 100.0;

    return similarity;
}

vector<vector<Score>> similarityTable(const vector<FileData>& files){
    int n = files.size();
    vector<vector<Score>> scores(n, vector<Score>(n));

    ///initialize score values
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scores[i][j] = {i, -1.0, j};
        }
    }
    for(int i = 0; i < n; i++)
    {
        for(int j = i + 1; j < n; j++)
        {
            double similarityScore = findSimilarity(files[i].fingerprints, files[j].fingerprints);
            scores[i][j] = {i, similarityScore, j};
            scores[j][i] = {j, similarityScore, i};
        }
    }
    return scores;
}

void printReport(const vector<FileData>& files, const vector<vector<Score>>& similarityScores)
{
    int n = files.size();

    ///print out all ranked scores for a file in a table format into an output file
    cout << setw(300)<< setfill('-') << "" << endl;
    cout << setfill(' ');

    ///table header
    cout << setw(10) << "";
    for(int i = 0; i < n; i++)
    {
        cout << setw(10) << files[i].fileID;
    }
    cout << endl;

    ///print out each Similarity scores for the files
    for(int i = 0; i < n; i++)
    {
        cout << setw(10) << files[i].fileID;
        for (int j = 0; j < n; j++) {

            if( i == j )
            {
                cout << setw(10) << "--";
            }
            else {
                cout << setw(10) << fixed << setprecision(2) << similarityScores[i][j].similarityScore;
            }
        }
        cout << endl;
    }
    cout << setw(300)<< setfill('-') << "" << endl;

}


int main() {
    /// Open tokens file
    ifstream infileTokens("tokens.txt");
    if (!infileTokens) {
        cerr << "Failed to open tokens.txt\n";
        return 1;
    }
    cout << "Tokens.txt opening...." << endl;

    ///Choose k and w values
    int k = 4; 
    int w = 4;

    ///Store all file data
    vector<FileData> files;
    string fileTokens;
    int fileID = 1;

    cout << "Reading in File Data......." << endl;
    while(getline(infileTokens, fileTokens))
    {
        if(fileTokens.empty()) continue;

        ///Read and store all data associated with a file
        FileData file;

        ///Set file ID
        file.fileID = fileID;

        ///Store all tokens for the file 
        file.tokens = readInTokens(fileTokens);

        ///Create and store hashes for all k-mers of a file
        file.k_mers_hashes = hashKMers(file.tokens, k);

        ///Find and store all fingerprints for a file
        file.fingerprints = winnowingAlgorithm(file.k_mers_hashes, w);

        cout << "File " << file.fileID
     << " tokens: " << file.tokens.size()
     << ", k-gram hashes: " << file.k_mers_hashes.size()
     << ", fingerprints: " << file.fingerprints.size()
     << endl;

        ///Add file data to our tracking vector
        files.push_back(file);
        fileID++;
    }

    ///Create N * N table, with N being the number of files
    /// calculate similarity score for files a and b and store in table
    vector<vector<Score>> similarityScores = similarityTable(files);

    ///Print out report for each file
    printReport(files, similarityScores);

    infileTokens.close();
    return 0;
}
