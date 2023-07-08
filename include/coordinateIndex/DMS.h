#ifndef DMS_H
#define DMS_H

#include <iostream>
#include <sstream>

// This struct describes a geographic coordinate in the DMS format.
struct DMS {
    double degrees;   // Degrees component
    double minutes;   // Minutes component
    double seconds;   // Seconds component
    char direction; // Direction (N, S, E, W)

    // This constructor takes coordinate in DMS format as a string, e.g. 0794530W, 0792630W, 381000N, 383000N.
    explicit DMS(const std::string &coordinate) {
        std::string coordinateWithoutDirection = coordinate.substr(0, coordinate.length() - 1);
        std::string directionString = coordinate.substr(coordinate.length() - 1);

        // Determine the number of digits for each component
        unsigned int minutesDigits = 2;
        unsigned int secondsDigits = 2;
        unsigned long degreesDigits = coordinateWithoutDirection.length() - minutesDigits - secondsDigits;


        // Extract the components from the string
        degrees = std::stod(coordinateWithoutDirection.substr(0, degreesDigits));
        minutes = std::stod(coordinateWithoutDirection.substr(degreesDigits, minutesDigits));
        seconds = std::stod(coordinateWithoutDirection.substr(degreesDigits + minutesDigits, secondsDigits));
        direction = directionString[0];
    }

    // This constructor takes coordinate in DMS format as separate components.
    DMS(double degrees, double minutes, double seconds, char direction) : degrees{degrees}, minutes{minutes},
                                                                          seconds{seconds}, direction{direction} {}

    [[nodiscard]] double toDecimal() const {
        double decimal = degrees + minutes / 60 + seconds / 3600;
        if (direction == 'S' || direction == 'W') {
            decimal *= -1;
        }
        return decimal;
    }

    // Print the DMS coordinate
    void print() const {
        std::cout << degrees << "°" << minutes << "'" << seconds << "\"" << direction;
    }

    int toSeconds() const {
        int dmsToSeconds = degrees * 3600 + minutes * 60 + seconds;
        return dmsToSeconds;
    }

    std::string str() const {
        std::ostringstream os;
        os << degrees << minutes << seconds << direction;
        return os.str();
    }
};

#endif //DMS_H
