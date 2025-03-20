#ifndef SOFTWARESTATE_H
#define SOFTWARESTATE_H
#include "main_classes.h"

struct SoftwareState final {
    /**
     * Enum representing the different state the software can be in.
     */
    enum Value {
        MAIN_MENU,
        PAUSED,
        PLAYING,
    };

    /**
     * @return the actual value of the state of the software.
     */
    [[nodiscard]] Value getValue() const {
        return value;
    }

    /**
     * Switch to the new state of the software.
     * @param value the new value of the state of the software.
     */
    void switchTo(const Value value) {
        if (value != this->value)
            this->value = value;
    }

    /**
     * Default constructor.
     * Default value of the state of the software is MAIN_MENU.
     */
    explicit SoftwareState(const Game* game): game(game), value(MAIN_MENU) {}

    Value operator() () const {
        return value;
    }

private:
    const Game *game;

    /**
     * The actual value of the state of the software.
     */
    Value value;
};

#endif //SOFTWARESTATE_H
