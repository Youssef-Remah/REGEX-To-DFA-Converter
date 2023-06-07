#pragma once
#include <string>
#include <set>
#include <map>
#include "Transitions.h"

using namespace std;


class State {
public:
    string stateName;

    bool isInitial;

    bool isFinal;

    set<int> state;

    map<char, Transitions> statesAndTransitions;

    State();

    State(string stateName, set<int> state, bool isInitial = false, bool isFinal = false);
};