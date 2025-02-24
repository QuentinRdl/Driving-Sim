#ifndef SEGMENTTYPE_H
#define SEGMENTTYPE_H

#include <string>
#include <stdexcept> // for the std::invalid_argument exception.

#define ASSETS_PATH "assets/images/"

struct ResourceType final {
    /**
     * Enum representing the different types of segments.
     */
    enum Value {
        /* Car */
        CAR,

        /* Segment Part */
        SEGMENT_SMALL_STRAIGHT,
        SEGMENT_LONG_STRAIGHT,
        SEGMENT_SMALL_TURN,
        SEGMENT_MEDIUM_TURN,
        SEGMENT_LARGE_TURN,
        SEGMENT_S_TURN,
        SEGMENT_U_TURN
    };

    /**
     * Get the path to the image corresponding to the resource
     * @param value the resource
     * @return the path to the image corresponding to the resource
     */
    static std::string getPath(const Value value) {
        const ResourceType type(value);
        return type.getPath();
    }

private:

    explicit ResourceType(const Value v): value(v) {}
    Value value;

    /**
     * Get the path to the image corresponding to the resource.
     * @return the path to the image corresponding to the resource
     */
    [[nodiscard]] std::string getPath() const {
        std::string filename;
        switch (this->value) {
            case SEGMENT_SMALL_STRAIGHT:
                filename = "small_straight.png";
                break;
            case SEGMENT_LONG_STRAIGHT:
                filename = "long_straight.png";
                break;
            case SEGMENT_SMALL_TURN:
                filename = "small_turn.png";
                break;
            case SEGMENT_MEDIUM_TURN:
                filename = "medium_turn.png";
                break;
            case SEGMENT_LARGE_TURN:
                filename = "large_turn.png";
                break;
            case SEGMENT_S_TURN:
                filename = "s_turn.png";
                break;
            case SEGMENT_U_TURN:
                filename = "u_turn.png";
                break;
            case CAR:
                filename = "car.png";
                break;
            default: throw std::invalid_argument("Invalid ResourceType: not implemented yet maybe ?");
        }

        return ASSETS_PATH + filename;
    }
};

#endif //SEGMENTTYPE_H
