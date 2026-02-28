#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXLOC = 150;

class KmerHash {
private:
    //table size
    int size;
    string* hashTable;
    // locations 
    int** locs;
    // number of locations
    int* locNum;
    // number of unique kmers
    int unique;
public:
    KmerHash(int s) {
        size = s;
        unique = 0;
        hashTable = new string[size];
        locs = new int*[size];
        for (int i = 0; i < size; i++) locs[i] = new int[MAXLOC];
        locNum = new int[size];        
    }
    /**
     * @brief hashes a string and returns index
     * 
     * @param key - string to hash
     * @return int - index
     */
    int hash(const string& key) {
        int h = 0;
        for (size_t i = 0; i < key.length(); i++) h += key[i];
        int index = h % size;
        return index;        
    }
    /**
     * @brief insert new k-mer
     * 
     * @param kmer - kmer to insert
     * @param pos - position in string
     */
    void insert(const string& kmer, int pos) {
        int index = hash(kmer);
        while (!hashTable[index].empty() && hashTable[index] != kmer) {
            index = (index + 1) % size;
        }
        if (hashTable[index] == kmer) {
            // put kmer into hashtable, is max locations hasnt been reached
            // MAY HAVE TO ADJUST MAXLOC
            if (locNum[index] < MAXLOC) {
                int x = locNum[index];
                locs[index][x] = pos;
                locNum[index]++;
            } else {
                printf("Max Locations reached for K-Mer %s\n", kmer);
            }
        } else {
            hashTable[index] = kmer;
            locs[index][0] = pos;
            locNum[index] = 1;
            unique++;
        }
        // check for occupied percent, rehash if over 50%
        float occupied = 1.0 * unique / size;
        if (occupied > 0.5) {
            rehash();
        }   
    }
    /**
     * @brief rehashes table
     * 
     */
    void rehash() {
        int oldSize = size;
        string* oldHashTable = hashTable;
        int** oldLocs = locs;
        int* oldlocNum = locNum;

        size *= 2;
        unique = 0;
        hashTable = new string[size];
        locs = new int*[size];
        for (int i = 0; i < size; i++) locs[i] = new int[MAXLOC];
        locNum = new int[size];

        for (int i = 0; i < oldSize; i++) {
            if (!oldHashTable[i].empty()) {
                for (int j = 0; j < oldlocNum[i]; j++) {
                    insert(oldHashTable[i], oldLocs[i][j]);
                }                
            }            
        }
        for (int i = 0; i < oldSize; i++) delete[] oldLocs[i];
        delete[] oldLocs;
        delete[] oldHashTable;
        delete[] oldlocNum;                
        
    }
    ~KmerHash() {
        for (int i = 0; i < size; i++) delete[] locs[i];
        delete[] locs;
        delete[] hashTable;
        delete[] locNum;
    }
    vector<int> locOfKmer(const string& kmer) {
        vector<int> v_locs;
        int index = hash(kmer);
        while (!hashTable[index].empty() && hashTable[index] != kmer) {
            index = (index + 1) % size;
        }
        if(hashTable[index] == kmer) {
            for (int i = 0; i < locNum[index]; i++) {
                v_locs.push_back(locs[index][i]);
            }   
        }
        return v_locs;        
    }
    void remove(const string& kmer, string& tokens, int k) {
        int index = hash(kmer);
        bool found = false;

        while (!hashTable[index].empty() && hashTable[index] != kmer) {
            index = (index + 1) % size;
        }
        if (hashTable[index] == kmer) {
            found = true;
            int count = locNum[index];
            vector<int> positions(locs[index], locs[index] + count);
            sort(positions.rbegin(), positions.rend());

            for (int pos : positions) {
                if (pos + kmer.length() <= tokens.length()) {
                    tokens.erase(pos, kmer.length());
                }                
            }            
        }    
    }
};

KmerHash::KmerHash(/* args */)
{
}

KmerHash::~KmerHash()
{
}


int main(int argc, char const *argv[]) {
    /// Open file
    ifstream infile;
    ///read in file info and remove white space so it is one line of characters
    if(argc > 1) {
        infile.open(argv[1]);
    } else {
        string tmp;
        printf("Please input filename: ");
        getline(cin, tmp);
        infile.open(tmp);
    }
    if (infile.fail()) {
        cerr << "Failed to open file\n";
        return 1;
    }
    
    // get tokenized version
    string tokens;
    getline(infile, tokens);
    
    ///Choose k value
    int k = 4; /* kylie: global, input, or hard-code? */

    // make hash table
    KmerHash table

    /// make all of the overlapping k-mers
    for (size_t i = 0; i < tokens.length() - k; i++) {
        /* code */
    }
    
    /// Hash the k-mers
    /// store sequence of hashed
    /// choose window size
    /// choose minimum hash value from each window

    /// calculate similarity score

    infile.close();
    cout << "CMOS Project" << endl;
    return 0;
    return 0;
}
