# Regular Expression to DFA Converter

## Objective
The objective of this project is to implement a **REGEX** to **DFA** converter using an **Abstract Syntax Tree** (AST) instead of the traditional approach of converting the REGEX to a Non-deterministic Finite Automaton (NFA) and then converting the NFA to a DFA.

## Description
### 1. What does this program do?
This program converts a Regular Expression (REGEX) into a Deterministic Finite Automaton (DFA).

### 2. Technologies Used
This console application is developed using the C++ programming language. C++ is chosen as the primary language for this program due to its ability to effectively handle tree data structures, making it the ideal choice for utilizing the power of C++ pointers.

### 3. Approach
### 3.1. Conversion without creating an NFA

Instead of creating an NFA as an intermediate step, we directly convert the REGEX to a DFA using the following steps:

1. Augmented REGEX: Firstly, the regular expression is converted into an augmented regular expression by concatenating it with a '#' symbol. (r -> (r)#)

2. Postfix Expression: Next, the augmented REGEX is converted into a postfix expression.

3. Syntax Tree Construction: We construct a syntax tree for the postfix expression.

4. Tree Traversal: The tree is traversed to construct four essential functions: _**nullable**_, _**firstpos**_, _**lastpos**_, and _**followpos**_.

5. Follow Position Table: We construct the follow position table.

6. DFA Construction: Finally, we convert the REGEX directly to a DFA by constructing a new table using the follow position table and employing an approach similar to the **subset construction** algorithm.

### 4. Program Input

The program expects the following inputs:

- Number of test cases: The user enters the number of test cases.
- Regular Expression: For each test case, the user enters a regular expression (e.g., (a|b)*).
- Input Strings: For each test case, the user enters a string to check whether it is accepted or rejected by the DFA.

### 5. Program Output

The program produces the following output:

- DFA Table: The program displays a simple representation of the DFA table, including its states and transitions, to the console.
- Test Case Status: For each test case, the program displays its status, indicating whether it is accepted or rejected by the DFA.


## Team Collaboration and Contributions

I feel privileged to have partnered with **[Mazen Abdalla](https://github.com/MazenMohamedAbdalla)** on this project. I'm grateful for his hard work and commitment to the project's goals. The seamless teamwork and synergy we've shared have truly enriched this experience.
