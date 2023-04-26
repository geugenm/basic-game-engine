#pragma once

/**
 * @brief Represents the events related to a game controller.
 *
 */
enum class Event
{
    LEFT_PRESSED,
    LEFT_RELEASED,
    RIGHT_PRESSED,
    RIGHT_RELEASED,
    UP_PRESSED,
    UP_RELEASED,
    DOWN_PRESSED,
    DOWN_RELEASED,
    SELECT_PRESSED,
    SELECT_RELEASED,
    START_PRESSED,
    START_RELEASED,
    BUTTON1_PRESSED,
    BUTTON1_RELEASED,
    BUTTON2_PRESSED,
    BUTTON2_RELEASED,
    NOTHING_HAPPENED,
    WINDOW_CLOSE,
};
