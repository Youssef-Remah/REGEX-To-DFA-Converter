#include<iostream>
#include<stack>
#include<string>
#include<set>
#include<utility>
#include<map>
#include<vector>

#include "Node.h"
#include "Transitions.h"
#include "State.h"

using namespace std;


// initial size is 1 (position 0 is omitted)
vector< pair< char, set<int> > > followPositionTable(1);

Node* head = nullptr;

// Functions Prototype
void followPosSymbolInitializer(string regex);

void treeConstruction(std::string postfixRegex);

int operationPriority(char operation);

std::string regexHandlerPostfix(std::string newRegex);

std::string RegEx(std::string oldRegex);

void positionInitializerDfs(Node* node);

void operatorPositionInitializer(Node* node);

void followPositionTableInitializer(Node* node);

vector<State> DfaInitializer();

int searchForState(string stateName, vector<State> states);

bool validateString(string str, vector<State> states);

void displayDfaTable(vector<State> DFATable);

int main() {

    string regex, str;

    int testCases = 0;

    cout << "Read number of test cases: ";
    cin >> testCases;

    cout << "Read a Regular Expression: ";
    cin >> regex;

    //construct the tree
    treeConstruction(regexHandlerPostfix(RegEx(regex)));

    //construct the follow position table
    followPosSymbolInitializer(regex);

    //assign fpos & lpos for nodes
    positionInitializerDfs(head);

    //DFA Table
    vector<State> DFATable = DfaInitializer();


    displayDfaTable(DFATable);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (testCases--)
    {

        cout << "Read a String: ";

        getline(cin, str);

        //Validate the input string

        if (validateString(str, DFATable))
            cout << "String is Accepted" << endl << endl;
        else
            cout << "String is Rejected" << endl << endl;

    }

    //(a|b)*(a|b)
    //std::cout << RegEx("(a|b)*abb") << std::endl;
    //std::cout << RegEx("(a|b)*") << std::endl;
    //std::cout << RegEx("((a|b)*)*abb*") << std::endl;
    //std::cout << RegEx("abb*|(a|b)*") << std::endl;

    return 0;
}

void followPosSymbolInitializer(string regex) {

    for (char c : regex)
    {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9'))
        {
            //pass the symbol, and empty followPos set (will be updated with values later)

            followPositionTable.push_back({ c, {} });
        }
    }
}

void treeConstruction(std::string postfixRegex) {
    std::stack <Node*> nodeStk;
    int i = 0;
    while (i < postfixRegex.length())
    {
        // operand and #
        if (std::isalpha(postfixRegex[i]) || std::isdigit(postfixRegex[i]) || postfixRegex[i] == '#') {
            // create child node
            Node* child = new Node();
            child->value = postfixRegex[i];

            // push to stack
            nodeStk.push(child);
        }
        // unary operator '+' and '*'
        else if (postfixRegex[i] == '+' || postfixRegex[i] == '*') {
            // parent node
            Node* parent = new Node();
            parent->value = postfixRegex[i];
            if (parent->value == '*') { parent->nullable = true; }

            // pop child and link
            Node* child = nodeStk.top();
            nodeStk.pop();
            parent->right = child;

            // push parent to stack
            nodeStk.push(parent);
        }
        // binary operator
        else {
            // parent node
            Node* parent = new Node();
            parent->value = postfixRegex[i];

            // pop two nodes and link
            Node* rChild = nodeStk.top(); nodeStk.pop();
            Node* lChild = nodeStk.top(); nodeStk.pop();
            parent->right = rChild;
            parent->left = lChild;

            // set nullable to true if rC & lF are '*'
            if (rChild->value == '*' && lChild->value == '*') { parent->nullable = true; }

            // push parent to stack
            nodeStk.push(parent);
        }
        i++;
    }

    // Root node of the tree
    head = nodeStk.top();

}

int operationPriority(char operation) {
    if (operation == '|') {
        return 1;
    }
    else if (operation == '.') {
        return 2;
    }
    else if (operation == '*' || operation == '+') { //***
        return 3;
    }
    return 0;
}

std::string regexHandlerPostfix(std::string newRegex) {
    std::stack<char> expression;
    std::string postfixRegex = "";

    int i = 0;
    while (i < newRegex.length())
    {
        if (newRegex[i] == '(') {
            expression.push(newRegex[i]);
        }
        else if (std::isalpha(newRegex[i]) || std::isdigit(newRegex[i]) || newRegex[i] == '#') {
            postfixRegex += newRegex[i];
        }
        else if (newRegex[i] == ')') {
            while (expression.top() != '(') {
                postfixRegex += expression.top();
                expression.pop();
            }
            expression.pop();
        }
        else {
            while (!(expression.empty()) && (operationPriority(newRegex[i]) <= operationPriority(expression.top())))
            {
                postfixRegex += expression.top();
                expression.pop();
            }
            expression.push(newRegex[i]);
        }
        i++;
    }

    while (!(expression.empty())) {
        postfixRegex += expression.top();
        expression.pop();
    }

    return postfixRegex;
}

std::string RegEx(std::string oldRegex) {


    std::string newRegex = "";
    newRegex += oldRegex[0];

    int i = 1;
    while (i < oldRegex.length()) {
        if (
            ((std::isalpha(oldRegex[i - 1]) || std::isdigit(oldRegex[i - 1])) &&
                (std::isalpha(oldRegex[i]) || std::isdigit(oldRegex[i]))) ||
            ((std::isalpha(oldRegex[i - 1]) || std::isdigit(oldRegex[i - 1])) && oldRegex[i] == '(') ||
            (oldRegex[i - 1] == ')' && (std::isalpha(oldRegex[i]) || std::isdigit(oldRegex[i]))) ||
            (oldRegex[i - 1] == '*' && (std::isalpha(oldRegex[i]) || std::isdigit(oldRegex[i])) ||
                (oldRegex[i - 1] == ')' && oldRegex[i] == '(') || (oldRegex[i - 1] == '*' && oldRegex[i] == '('))) {
            newRegex += ".";
        }
        newRegex += oldRegex[i];

        i++;
    }

    newRegex = "(" + newRegex + ").#";
    return newRegex;
}

void positionInitializerDfs(Node* node) {

    static int pos = 1;

    if (node == nullptr)
        return;

    positionInitializerDfs(node->left);

    positionInitializerDfs(node->right);

    //if node is a leaf node

    if (node->left == nullptr && node->right == nullptr)
    {
        //assign a position for the leaf node
        node->position = pos;

        //compute Fpos & Lpos for the leaf node

        node->fPos.insert(pos);

        node->lPos.insert(pos);

        pos++;
    }

    //if node is an operator

    if (node->value == '.' || node->value == '|' || node->value == '*' || node->value == '+')
        operatorPositionInitializer(node);
}

void operatorPositionInitializer(Node* node) {

    if (node->value == '|')
    {
        //Compute firstPos(n)
        // 
        //Fpos(node) = Fpos(c1) U Fpos(c2)

        node->fPos.insert(node->left->fPos.begin(), node->left->fPos.end());

        node->fPos.insert(node->right->fPos.begin(), node->right->fPos.end());

        //Compute lastPos(n)
        // 
        //Lpos(node) = Lpos(c1) U Lpos(c2)

        node->lPos.insert(node->left->lPos.begin(), node->left->lPos.end());

        node->lPos.insert(node->right->lPos.begin(), node->right->lPos.end());
    }

    else if (node->value == '.')
    {
        //Compute firstPos(n)
        //
        //Fpos(n) = ( nullable(c1) ) ? Fpos(c1) U Fpos(c2) : Fpos(c1)

        if (node->left->nullable == true)
        {
            node->fPos.insert(node->left->fPos.begin(), node->left->fPos.end());

            node->fPos.insert(node->right->fPos.begin(), node->right->fPos.end());
        }
        else
            node->fPos = node->left->fPos;

        //Compute lastPos(n)
        //
        //Lpos(n) = ( nullable(c2) ) ? Lpos(c1) U Lpos(c2) : Lpos(c2)

        if (node->right->nullable == true)
        {
            node->lPos.insert(node->left->lPos.begin(), node->left->lPos.end());

            node->lPos.insert(node->right->lPos.begin(), node->right->lPos.end());
        }
        else
            node->lPos = node->right->lPos;

        //compute the followPos

        followPositionTableInitializer(node);
    }

    else if (node->value == '*' || node->value == '+')
    {
        //Compute firstPos(n)
        //
        //Fpos(n) = Fpos(c1)

        node->fPos = node->right->fPos;

        //Compute lastPos(n)
        //
        //Lpos(n) = Lpos(c1)

        node->lPos = node->right->lPos;

        //compute the followPos

        followPositionTableInitializer(node);
    }
}

void followPositionTableInitializer(Node* node) {

    if (node->value == '*')
    {
        for (auto iter = node->lPos.begin(); iter != node->lPos.end(); iter++)

            followPositionTable[*iter].second.insert(node->fPos.begin(), node->fPos.end());
    }

    else
    {
        for (auto iter = node->left->lPos.begin(); iter != node->left->lPos.end(); iter++)

            followPositionTable[*iter].second.insert(node->right->fPos.begin(), node->right->fPos.end());
    }
}

vector<State> DfaInitializer() {

    int counter = 1;

    vector<State> states;

    //initialize the initial state with the fpos of root node
    states.push_back(State("Q1", head->fPos, true));

    //loop on the states
    for (int i = 0; i < states.size(); i++)
    {
        for (auto iter = states[i].state.begin(); iter != states[i].state.end(); iter++)//loop on the integers in each state
        {
            if (*iter >= followPositionTable.size())
                break;

            states[i].statesAndTransitions[followPositionTable[*iter].first];

            states[i].statesAndTransitions[followPositionTable[*iter].first].transitions.insert
            (
                followPositionTable[*iter].second.begin(), followPositionTable[*iter].second.end()
            );
        }

        auto statesAndTransitionsCopy = states[i].statesAndTransitions;

        //check if new state is created
        for (int j = 0; (j < states.size()) && (!states[j].statesAndTransitions.empty()); j++) //to control states on the right side of the table
        {
            for (auto iter = statesAndTransitionsCopy.begin(); iter != statesAndTransitionsCopy.end(); iter++)
            {
                bool isNew = true;

                if (states[i].statesAndTransitions[iter->first].transitions == states[j].state)
                {
                    states[i].statesAndTransitions[iter->first].transitionName = states[j].stateName;
                }

                else
                {
                    auto it = states.begin();
                    while (it != states.end()) {

                        if (states[i].statesAndTransitions[iter->first].transitions == it->state)
                        {
                            states[i].statesAndTransitions[iter->first].transitionName = it->stateName;

                            isNew = false;

                            break;
                        }

                        it++;
                    }

                    if (isNew)
                    {
                        string tmpName = "Q" + to_string(++counter);

                        states[i].statesAndTransitions[iter->first].transitionName = tmpName;

                        states.push_back(State(tmpName, states[i].statesAndTransitions[iter->first].transitions));

                    }
                }
            }
        }
    }

    for (auto iter = states.begin(); iter != states.end(); iter++)
    {
        if (iter->state.find(head->right->position) != iter->state.end())
            iter->isFinal = true;
    }

    return states;
}

int searchForState(string stateName, vector<State> states) {

    for (int i = 0; i < states.size(); i++)
    {
        if (states[i].stateName == stateName)
            return i;
    }
}

bool validateString(string str, vector<State> states) {

    int sz = str.length();

    /* Cases of acceptance */

    //Case 1: Reached the a final state without consuming any input characters(i.e. having an empty string & the initial state is also final state)

    if ((sz == 0) && (states[0].isFinal == true))
    {
        return true;
    }

    //Iterating through the string and switching between transitions

    int statePtr = 0;

    for (int i = 0; i < sz; i++)
    {
        //start initially from the initial state Q1

        //if there is a transition with the current character symbol
        if (!states[statePtr].statesAndTransitions[str[i]].transitions.empty())
        {

            //search for the index of the given state name and return its index in the vector
            statePtr = searchForState(states[statePtr].statesAndTransitions[str[i]].transitionName, states);


            //if reached the end of the string
            if ((i + 1) == sz)
            {
                //if current state a final state
                if (states[statePtr].isFinal)
                    return true;

                //if current state is not a final state
                else
                    return false;
            }
        }

        //there is no transition with the current character symbol
        else
            return false;
    }
}

void displayDfaTable(vector<State> DFATable) {

    cout << "\n************************* DFA Table *************************\n\n";

    for (int i = 0; i < DFATable.size(); i++)
    {
        cout << DFATable[i].stateName << "\t";

        for (auto iter = DFATable[i].statesAndTransitions.begin(); iter != DFATable[i].statesAndTransitions.end(); iter++)
        {

            cout << iter->first << " -> " << iter->second.transitionName << "\t\t";
        }

        cout << endl << endl;
    }
}