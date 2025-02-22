#ifndef SEGMENTTYPE_H
#define SEGMENTTYPE_H

#include <string>
#include <stdexcept> // for the std::invalid_argument exception.

#define BASE_PATH "/home/rgld_/01-dev-projects/cpp-projects/Driving-Sim/images/"

struct SegmentType final {
    /**
     * Enum representing the different types of segments.
     */
    enum Value {
        SMALL_STRAIGHT,
        LONG_STRAIGHT,
        SMALL_TURN,
        MEDIUM_TURN,
        LARGE_TURN,
        S_TURN,
        U_TURN
    };

    /**
     * Get the path to the image corresponding to the segment type.
     * @param value the segment type
     * @return the path to the image corresponding to the segment type
     */
    static std::string getPath(const Value value) {
        const SegmentType type(value);
        return type.getPath();
    }

private:

    explicit SegmentType(const Value v): value(v) {}
    Value value;

    /**
     * Get the path to the image corresponding to the segment type.
     * @return the path to the image corresponding to the segment type
     */
    [[nodiscard]] std::string getPath() const {
        std::string filename;
        switch (this->value) {
            case SMALL_STRAIGHT:
                filename = "small_straight.png";
                break;
            case LONG_STRAIGHT:
                filename = "long_straight.png";
                break;
            case SMALL_TURN:
                filename = "small_turn.png";
                break;
            case MEDIUM_TURN:
                filename = "medium_turn.png";
                break;
            case LARGE_TURN:
                filename = "large_turn.png";
                break;
            case S_TURN:
                filename = "s_turn.png";
                break;
            case U_TURN:
                filename = "u_turn.png";
                break;
            default: throw std::invalid_argument("Invalid segmentType: not implemented yet maybe ?");
        }

        return BASE_PATH + filename;
    }
};

#endif //SEGMENTTYPE_H
