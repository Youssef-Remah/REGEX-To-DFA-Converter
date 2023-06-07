#include "State.h"

State::State() {
    stateName = "";

    isInitial = false;

    isFinal = false;
}

State::State(string stateName, set<int> state, bool isInitial, bool isFinal) {
    this->stateName = stateName;
    this->state = state;
    this->isInitial = isInitial;
    this->isFinal = isFinal;
}