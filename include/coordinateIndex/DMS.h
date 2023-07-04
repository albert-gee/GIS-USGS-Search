#ifndef DMS_H
#define DMS_H

#include <iostream>

// This struct describes a geographic coordinate in the DMS format.
struct DMS {
    double degrees;   // Degrees component
    double minutes;   // Minutes component
    double seconds;   // Seconds component
    char direction; // Direction (N, S, E, W)

    DMS(double degrees, double minutes, double seconds, char direction) : degrees{degrees}, minutes{minutes}, seconds{seconds}, direction{direction} {}

    // Print the DMS coordinate
    void print() const {
        std::cout << degrees << "°" << minutes << "'" << seconds << "\"" << direction;
    }
};

#endif //DMS_H
