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
        validate();
    }

    // This constructor takes coordinate in DMS format as separate components.
    DMS(double degrees, double minutes, double seconds, char direction)
            : degrees{degrees}, minutes{minutes}, seconds{seconds}, direction{direction} {
        // Validate the components
        validate();
    }

    // To decimal format, e.g. 38.5
    [[nodiscard]] double toDecimal() const {
        double value = degrees + minutes / 60 + seconds / 3600;
        return (direction == 'N' || direction == 'E') ? value : -value;
    }

    // To human-readable decimal string, e.g. 38.5°
    [[nodiscard]] std::string toDecimalString() const {
        std::ostringstream os;
        double value = degrees + minutes / 60 + seconds / 3600;
        os << ((direction == 'N' || direction == 'E') ? value : -value) << "°";
        return os.str();
    }

    // To human-readable DMS string, e.g. 38°30'0"N
    [[nodiscard]] std::string toDmsString() const {
        std::ostringstream os;
        os << degrees << "°" << minutes << "'" << seconds << "\"" << direction;
        return os.str();
    }

    // Validate the components: degrees, minutes, seconds, and direction.
    // Throw an exception if any of the components is invalid.
    void validate() const {
        // Validate degrees
        if (degrees < 0 || degrees > 180) {
            throw std::invalid_argument("Degrees value out of range.");
        }

        // Validate minutes
        if (minutes < 0 || minutes >= 60) {
            throw std::invalid_argument("Minutes value out of range.");
        }

        // Validate seconds
        if (seconds < 0 || seconds >= 60) {
            throw std::invalid_argument("Seconds value out of range.");
        }

        // Validate direction
        if (direction != 'N' && direction != 'S' && direction != 'E' && direction != 'W') {
            throw std::invalid_argument("Invalid direction.");
        }
    }

    // Find the center between this DMS and another DMS, e.g. between to latitudes.
    [[nodiscard]] DMS findCenterBetweenThisAndAnotherDMS(DMS anotherDMS) const {
        double avgDegrees = (degrees + anotherDMS.degrees) / 2.0;
        double avgMinutes = (minutes + anotherDMS.minutes) / 2.0;
        double avgSeconds = (seconds + anotherDMS.seconds) / 2.0;

        // Normalize the components
        while (avgSeconds >= 60.0) {
            avgMinutes += 1.0;
            avgSeconds -= 60.0;
        }
        while (avgMinutes >= 60.0) {
            avgDegrees += 1.0;
            avgMinutes -= 60.0;
        }
        while (avgDegrees >= 360.0) {
            avgDegrees -= 360.0;
        }

        // Determine the direction
        char newDirection;
        if (direction == anotherDMS.direction) {
            newDirection = direction;
        } else {
            double diffDegrees = std::abs(degrees - anotherDMS.degrees);
            double diffMinutes = std::abs(minutes - anotherDMS.minutes);
            double diffSeconds = std::abs(seconds - anotherDMS.seconds);

            if (diffDegrees < 180.0 ||
                (diffDegrees == 180.0 && diffMinutes < 180.0) ||
                (diffDegrees == 180.0 && diffMinutes == 180.0 && diffSeconds < 180.0)) {
                newDirection = (direction == 'N' || direction == 'E') ? direction : anotherDMS.direction;
            } else {
                newDirection = (direction == 'S' || direction == 'W') ? direction : anotherDMS.direction;
            }
        }

        return DMS{avgDegrees, avgMinutes, avgSeconds, newDirection};
    }

    // Compare this DMS with another DMS.
    [[nodiscard]] bool equals(const DMS& other) const {
        return (degrees == other.degrees) &&
               (minutes == other.minutes) &&
               (seconds == other.seconds) &&
               (direction == other.direction);
    }
};

#endif //DMS_H
