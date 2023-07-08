#ifndef DMS_H
#define DMS_H

#include <iostream>
#include <sstream>

// Latitude and Longitude can be expressed in DMS format (degrees/minutes/seconds, e.g. 0820830W).
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

        // Validate the components
        if (degrees < 0 || degrees > 180 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59 ||
            (direction != 'N' && direction != 'S' && direction != 'E' && direction != 'W')) {
            throw std::invalid_argument("Invalid coordinate: " + coordinate);
        }
    }

    // This constructor takes coordinate in DMS format as separate components.
    DMS(double degrees, double minutes, double seconds, char direction) : degrees{degrees}, minutes{minutes},
                                                                          seconds{seconds}, direction{direction} {}

    // To decimal format, e.g. 38.5
    [[nodiscard]] double toDecimal() const {
        double value = degrees + minutes / 60 + seconds / 3600;
        return (direction == 'N' || direction == 'E') ? value : -value;
    }

    // To decimal format, e.g. 38.5
    [[nodiscard]] std::string toDecimalString() const {
        std::ostringstream os;
        double value = degrees + minutes / 60 + seconds / 3600;
        os << ((direction == 'N' || direction == 'E') ? value : -value) << "°";
        return os.str();
    }

    // To human-readable DMS String, e.g. 38°30'0"N
    [[nodiscard]] std::string toDmsString() const {
        std::ostringstream os;
        os << degrees << "°" << minutes << "'" << seconds << "\"" << direction;
        return os.str();
    }

    [[nodiscard]] int toSeconds() const {
        int dmsToSeconds = degrees * 3600 + minutes * 60 + seconds;
        return dmsToSeconds;
    }
};

#endif //DMS_H
