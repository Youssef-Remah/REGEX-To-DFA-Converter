#pragma once
#include <string>
#include <set>

using namespace std;

class Transitions {

public:

    string transitionName;

    set<int> transitions;

    Transitions();

    Transitions(string transitionName, set<int> transitions);

};