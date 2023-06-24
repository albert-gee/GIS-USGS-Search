#ifndef FINAL_NAMEINDEX_H
#define FINAL_NAMEINDEX_H
#include <string>
#include <list>

using namespace std;



class NameIndex {
public:
    unsigned int indexLine(const string &key, int lineNum);
    //unsigned int indexLine(const string &featureName, const string &stateAbrv, int lineNum);
    list<int> getLineNumsByKey(string);
    NameIndex();
    void printIndex();
private:
    static unsigned int const INITIAL_SIZE = 1024;
    static double constexpr MAX_FILLED_PERCENTAGE = 0.7;
    static int const INDEX_GROWTH_RATE = 2;
    static const int FEATURE_NAME_COL = 1;
    static const int STATE_ALPHA_COL = 3;
    unsigned int currentIndexSize;

    struct Index;
    Index** indexPtrs;
    unsigned int indexesFilled;

    unsigned int hashAlgorithm(const string&, int);
    unsigned int rehash(unsigned int hash, unsigned int i);
    unsigned int quadraticProbing(unsigned int);
    string extractKey(const string&, char delimiter);
    bool isIndexOverFilled();

    void resizeIndex();

};
#endif //FINAL_NAMEINDEX_H
