#pragma once

enum class TransitionState {
    FADE_IN_FIRST,
    FADE_IN,
    FADE_OUT,
    FADE_OUT_END,
    LEFT_IN,
    LEFT_OUT,
    RIGHT_IN,
    RIGHT_OUT,
    CHANGE,
    END,
    NONE
};