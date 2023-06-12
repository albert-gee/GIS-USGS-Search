#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <array>
#include <vector>
#include <sstream>
#include "../include/NameIndex.h"
#include "../include/LineUtility.h"

using namespace std;

struct NameIndex::Index{
    string key;
    list<int> lineNums;
    Index(string key){
        Index::key = key;
    }
};

NameIndex::NameIndex() {
    currentIndexSize = INITIAL_SIZE;
    indexesFilled = 0;
    indexPtrs = new Index*[currentIndexSize]{nullptr};
}

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

unsigned int NameIndex::quadraticProbing(unsigned int i){
    return(i * i + i) /2;
}

string NameIndex::extractKey(string line) {
    cout << line;
    const vector<string> *parameters = LineUtility::extractParametersFromLine(line);
    string featureName = (*parameters)[FEATURE_NAME_COL];
    string stateAlpha = (*parameters)[STATE_ALPHA_COL];
    return featureName + " " + stateAlpha;
}

bool NameIndex::isIndexOverFilled() {
    return indexesFilled >= currentIndexSize * MAX_FILLED_PERCENTAGE;
}

void NameIndex::resizeIndex() {
    int oldIndexSize = currentIndexSize;
    currentIndexSize *= INDEX_GROWTH_RATE;
    indexesFilled = 0;
    Index **newIndexPtrs = new Index *[currentIndexSize]{nullptr};
    for (int i = 0; i < oldIndexSize; ++i) {
        if (indexPtrs[i] != nullptr) {
            string key = indexPtrs[i]->key;
            unsigned int hash = hashAlgorithm(key, currentIndexSize);
            unsigned int j = 0;
            bool keyFound = false;
            while (newIndexPtrs[hash] != nullptr && !keyFound) {
                if (newIndexPtrs[hash]->key == key) {
                    keyFound = true;
                } else {
                    unsigned int q = quadraticProbing(++j);
                    hash += q;
                    hash %= currentIndexSize;
                }
            }

            if (newIndexPtrs[hash] == nullptr) {
                newIndexPtrs[hash] = new Index(key);
                ++indexesFilled;
            }
            newIndexPtrs[hash]->lineNums = indexPtrs[i]->lineNums;
        }
    }

    delete[] indexPtrs;
    indexPtrs = newIndexPtrs;
}

void NameIndex::indexLine(const string &key, int lineNum) {
    unsigned int hash = hashAlgorithm(key, currentIndexSize);
    unsigned int i = 0;
    bool keyFound = false;
    while (indexPtrs[hash] != nullptr && !keyFound) {
        if (indexPtrs[hash]->key == key) {
            keyFound = true;
        } else {
            unsigned int q = quadraticProbing(++i);
            hash += q;
            hash %= currentIndexSize;
        }
    }

    if(indexPtrs[hash] == nullptr) {
        indexPtrs[hash] = new Index(key);
        ++indexesFilled;
    }

    indexPtrs[hash]->lineNums.push_back(lineNum);

    if(isIndexOverFilled()){
        resizeIndex();
    }
}

void NameIndex::printIndex() {
    for(int i = 0; i < currentIndexSize; i++) {
        if(indexPtrs[i] != nullptr){
            ostringstream os;
            for(int j : indexPtrs[i]->lineNums){
                os<< j << " ";
            }
            cout << "index: " << i << ", key: " << indexPtrs[i]->key << ", line: " << os.str() << endl;
        }
    }
}

list<int> NameIndex::getLineNumsByKey(string key) {
    unsigned int i = 0;

    unsigned int hash = hashAlgorithm(key, currentIndexSize);
    list<int> lines;
    bool keyFound = false;
    while(indexPtrs[hash] != nullptr  && !keyFound){
        if(indexPtrs[hash]->key == key){
            keyFound = true;
            lines = indexPtrs[hash]->lineNums;
        } else {
            unsigned int q = quadraticProbing(++i);
            hash += q;
            hash %= currentIndexSize;
        }
    }
    lines.sort();
    return lines;
}
