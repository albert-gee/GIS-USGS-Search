#ifndef GIS_INDEXSTATS_H
#define GIS_INDEXSTATS_H

// This struct describes the statistics of the name index after indexing a file.
struct IndexStats {

    unsigned int numOfIndexedLines{0}; // The number of lines indexed

    unsigned int totalNameLength{0}; // The total length of all the feature names

    unsigned int longestProbeSeq{0}; // The longest probe sequence

    // The average length of the feature name
    [[nodiscard]] unsigned int getAvgNameLength() const {
        return (numOfIndexedLines != 0) ? totalNameLength / numOfIndexedLines : 0;
    }
};

#endif //GIS_INDEXSTATS_H
