#include "Node.h"

Node::Node() {
    position = 0;
    value = 'N';
    nullable = false;
    right = left = NULL;
    fPos.clear(); lPos.clear();
}