#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "../../include/nameIndex/NameIndex.h"

using namespace std;

// Structure of an index in the name index
struct NameIndex::Index{
    string key;
    int lineNum;
    Index(string key, int lineNum){
        Index::key = key;
        Index::lineNum = lineNum;
    }
};

void indexLine(const string &featureName, const string &stateAbrv, int lineNum);

void indexLine(const string &featureName, const string &stateAbrv, int lineNum);

// Constru ctor
NameIndex::NameIndex() {
    currentIndexSize = INITIAL_SIZE; // Holds the current index size
    indexesFilled = 0; // Keeps track of how many indexes are filled
    indexPtrs = new Index*[currentIndexSize]{nullptr}; // The array to hold th name index
}

// Generate hash
unsigned int NameIndex::hashAlgorithm(const string &key, int indexSize){
    unsigned int hash = 0;
    unsigned int x = 0;

    for(unsigned int i =0 ; i < key.length(); ++i){
        hash = (hash << 4) + (key[i]);
        if((x = hash & 0xF0000000) != 0){
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }
    return hash % indexSize;
}

// Quadratic equation
unsigned int NameIndex::quadraticProbing(unsigned int i){
    return(i * i + i) /2;
}

// Rehash a current hash
unsigned int NameIndex::rehash(unsigned int hash, unsigned int i){
    unsigned int q = quadraticProbing(++i);
    hash += q;
    hash %= currentIndexSize;
    return hash;
}

// Check if index size has grown past the limit
bool NameIndex::isIndexOverFilled() {
    return indexesFilled >= currentIndexSize * MAX_FILLED_PERCENTAGE;
}

// Create a bigger index and insert all currently indexed items it
void NameIndex::resizeIndex() {
    int oldIndexSize = currentIndexSize;
    currentIndexSize *= INDEX_GROWTH_RATE;
    indexesFilled = 0;
    Index **oldIndexPtrs = indexPtrs;
    indexPtrs = new Index *[currentIndexSize]{nullptr};
    for (int i = 0; i < oldIndexSize; ++i) {
        if (oldIndexPtrs[i] != nullptr) {
            string key = oldIndexPtrs[i]->key;
            int lineNum = oldIndexPtrs[i]->lineNum;
            indexLine(key, lineNum);
        }
    }
    delete[] oldIndexPtrs;
}

// Insert the key and line into the index
unsigned int NameIndex::indexLine(const string &key, int lineNum) {
    unsigned int hash = hashAlgorithm(key, currentIndexSize);
    unsigned int i = 0;
    //unsigned int probes = 0;
    // Loop until an index with nullptr is found
    while (indexPtrs[hash] != nullptr) {
        hash = rehash(hash, ++i);
    }

    indexPtrs[hash] = new Index(key, lineNum);
    ++indexesFilled;

    if(isIndexOverFilled()){
        resizeIndex();
    }
    return i;
}

// Insert the key and line into the index
/*unsigned int NameIndex::indexLine(const string &featureName, const string &stateAbrv, int lineNum) {
    ostringstream os;
    os << featureName << " " << stateAbrv;
    string key = os.str();
    unsigned int hash = hashAlgorithm(key, currentIndexSize);
    unsigned int i = 0;

    // Loop until an index with nullptr is found
    while (indexPtrs[hash] != nullptr) {
        hash = rehash(hash, ++i);
    }

    indexPtrs[hash] = new Index(key, lineNum);
    ++indexesFilled;

    if(isIndexOverFilled()){
        resizeIndex();
    }
}*/

// Print contents of index
void NameIndex::printIndex() {
    for(int i = 0; i < currentIndexSize; i++) {
        if(indexPtrs[i] != nullptr){
            cout << "indexDatabaseByName: " << i << ", key: " << indexPtrs[i]->key << ", line: " << indexPtrs[i]->lineNum << endl;
        }
    }
}

const string NameIndex::str(){
    ostringstream os;
    os << "Format of display is\n";
    os << "Slot number: data record\n";
    os << "Current table size is " << currentIndexSize << endl;
    os << "Number of elements in table is " << indexesFilled << endl << endl;
    unsigned int indexesRead = 0;
    unsigned int index = 0;
    while(index < currentIndexSize && indexesRead <= indexesFilled){

        if(indexPtrs[index] != nullptr){

            string featureName = indexPtrs[index]->key.substr(0, indexPtrs[index]->key.length() - 3);
            string stateAbrv = indexPtrs[index]->key.substr(indexPtrs[index]->key.length() - 2, 2);
            os.width(4);
            os << "" << index << ": [" << featureName << ":" << stateAbrv << ", [" << indexPtrs[index]->lineNum << "]]";
            if(++indexesRead < indexesFilled){
                os << endl;
            }
        }
        ++index;

    }
/*
    for(int i = 0; i < currentIndexSize; i++) {
        if(indexPtrs[i] != nullptr){
            string featureName = indexPtrs[i]->key.substr(0, indexPtrs[i]->key.length() - 3);
            string stateAbrv = indexPtrs[i]->key.substr(indexPtrs[i]->key.length() - 2, 2);
            os.width(4);
            os << "" << i << ": [" << featureName << ":" << stateAbrv << ", [" << indexPtrs[i]->lineNum << "]]" << endl;
        }
    }

*/
    //cout << stats;
    return os.str();
}

// Search nameIndex for entries that match the key and return the lines
list<int> NameIndex::getLineNumsByKey(string key) {
    unsigned int i = 0;
    unsigned int hash = hashAlgorithm(key, currentIndexSize);
    list<int> lines;

    // Go through name index until a nullptr is found
    // If index contains key that match, add line to the list
    while(indexPtrs[hash] != nullptr){
        if(indexPtrs[hash]->key == key){
            lines.push_front(indexPtrs[hash]->lineNum);
        }
        hash = rehash(hash, ++i);
    }
    return lines;
}