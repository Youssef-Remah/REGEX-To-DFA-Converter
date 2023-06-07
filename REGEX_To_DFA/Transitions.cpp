#include "Transitions.h"


Transitions::Transitions() {
    this->transitionName = "";
}

Transitions::Transitions(string transitionName, set<int> transitions) {

    this->transitionName = transitionName;
    this->transitions = transitions;
}