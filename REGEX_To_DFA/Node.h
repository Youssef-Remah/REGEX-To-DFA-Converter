#pragma once
#include <set>

using namespace std;

class Node {
public:
    char value;
    bool nullable;
    int position;
    std::set<int> fPos;
    std::set<int> lPos;
    Node* right;
    Node* left;

    Node();

};