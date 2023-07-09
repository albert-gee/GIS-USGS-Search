#ifndef DMS_H
#define DMS_H

#include <iostream>
#include <sstream>
#include <iomanip>

// Latitude and Longitude can be expressed in DMS format (degrees/minutes/seconds, e.g. 0820830W).
// This struct describes a geographic coordinate in the DMS format.
struct DMS {
    unsigned int degrees;   // Degrees component
    unsigned int minutes;   // Minutes component
    unsigned int seconds;   // Seconds component
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
        degrees = std::stoi(coordinateWithoutDirection.substr(0, degreesDigits));
        minutes = std::stoi(coordinateWithoutDirection.substr(degreesDigits, minutesDigits));
        seconds = std::stoi(coordinateWithoutDirection.substr(degreesDigits + minutesDigits, secondsDigits));
        direction = directionString[0];

        // Validate the components
        validate();
    }

    // This constructor takes coordinate in DMS format as separate components.
    DMS(unsigned int degrees, unsigned int minutes, unsigned int seconds, char direction)
            : degrees{degrees}, minutes{minutes}, seconds{seconds}, direction{direction} {
        // Validate the components
        validate();
    }

    // To decimal format, e.g. 38.5
    [[nodiscard]] double toDecimal() const {
        double value = degrees + (double) minutes / 60 + (double) seconds / 3600;
        return (direction == 'N' || direction == 'E') ? value : -value;
    }

    // To human-readable decimal string, e.g. 38.5°
    [[nodiscard]] std::string toDecimalString() const {
        double value = degrees + (double) minutes / 60 + (double) seconds / 3600;

        std::ostringstream os;
        os << std::fixed << std::setprecision(7);

        // Apply direction to the value
        os << ((direction == 'N' || direction == 'E') ? value : -value);

        // Convert to string
        std::string formattedValue = os.str();

        // Truncate trailing zeros
        size_t pos = formattedValue.find_last_not_of('0');
        if (pos != std::string::npos && pos != formattedValue.size() - 1) {
            if (formattedValue[pos] == '.')
                formattedValue.erase(pos);
            else
                formattedValue.erase(pos + 1);
        }

        // Append the direction
        formattedValue += "°";

        return formattedValue;
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
        int totalSeconds1 = (direction == 'S' || direction == 'W') ? -1 : 1;
        totalSeconds1 *= (int) (degrees * 3600 + minutes * 60 + seconds);

        int totalSeconds2 = (anotherDMS.direction == 'S' || anotherDMS.direction == 'W') ? -1 : 1;
        totalSeconds2 *= (int) (anotherDMS.degrees * 3600 + anotherDMS.minutes * 60 + anotherDMS.seconds);

        int centerSeconds = (totalSeconds1 + totalSeconds2) / 2;
        unsigned int centerDegrees = std::abs(centerSeconds) / 3600;
        unsigned int centerMinutes = (std::abs(centerSeconds) % 3600) / 60;
        unsigned int centerSecondsRemainder = std::abs(centerSeconds) % 60;

        char newDirection;

        if (centerSeconds < 0) {
            newDirection = (direction == 'S' || anotherDMS.direction == 'S') ? 'S' : 'W';
        } else {
            newDirection = (direction == 'N' || anotherDMS.direction == 'N') ? 'N' : 'E';
        }

        return {centerDegrees, centerMinutes, centerSecondsRemainder, newDirection};

    }

    // Compare this DMS with another DMS.
    [[nodiscard]] bool equals(const DMS& other) const {
        return (degrees == other.degrees) &&
               (minutes == other.minutes) &&
               (seconds == other.seconds) &&
               (direction == other.direction);
    }

    void addSeconds(int additionalSeconds) {
        // Convert to total seconds
        int totalSeconds = (int) (degrees * 3600 + minutes * 60 + this->seconds);

        // Add the additional seconds
        totalSeconds += additionalSeconds;

        degrees = std::abs(totalSeconds) / 3600;
        minutes = (std::abs(totalSeconds) % 3600) / 60;
        seconds = std::abs(totalSeconds) % 60;

        if (totalSeconds < 0) {

            if (direction == 'S' || direction == 'W') {
                direction = (direction == 'N') ? 'N' : 'E';
            }

        }
    }
};

#endif //DMS_H
