#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

enum class State {
  IDLE,
  STACK,
  QUEUE,
  LIST
};

class JumpTable {
private:
  State state;
  
  vector<char> list;
  stack<char> stack;
  queue<char> queue;
  
  const string FILE_LIST = "list.txt";
  const string FILE_STACK = "stack.txt";
  const string FILE_QUEUE = "queue.txt";
  
  unordered_map<State, void(JumpTable::*)(void)> stateEnterMeths;
  unordered_map<State, bool(JumpTable::*)(void)> stateStayMeths;
  unordered_map<State, void(JumpTable::*)(void)> stateExitMeths;
  
  /// Helper methods
  void clearScreen() {
    cout << "\033[H\033[2J";
  }
  void drawStack() {
    vector<char> stacklist;
    while (!stack.empty()) {
      stacklist.push_back(stack.top());
      stack.pop();
    }
    for (auto it = stacklist.rbegin(); it != stacklist.rend(); ++it) {
      cout << "|---|" << endl << "| " << *it << " |" << endl;
    }
    cout << "|   |" << endl;
    for (char ch : stacklist) {
      stack.push(ch);
    }
  }
  void drawQueue() {
    vector<char> queuelist;
    while (!queue.empty()) {
      queuelist.push_back(queue.front());
      queue.pop();
    }
    for (char ch : queuelist) {
      cout << "| " << ch << " ";
    }
    cout << "|" << endl;
    for (char ch : queuelist) {
      queue.push(ch);
    }
  }
  void drawList() {
    cout << "{ ";
    for (char ch : list) {
      cout << ch << ", ";
    }
    cout << "}" << endl;
  }
  
  /// State Enter methods
  void stateEnterIdle() {}
  void stateEnterStack() {
    stack = {};
    ifstream file(FILE_STACK);
    char ch;
    while (file >> ch) {
      if (ch != ',') {
        stack.push(ch);
      }
    }
    file.close();
  }
  void stateEnterQueue() {
    queue = {};
    ifstream file(FILE_QUEUE);
    char ch;
    while (file >> ch) {
      if (ch != ',') {
        queue.push(ch);
      }
    }
    file.close();
  }
  void stateEnterList() {
    list.clear();
    ifstream file(FILE_LIST);
    char ch;
    while (file >> ch) {
      if (ch != ',') {
        list.push_back(ch);
      }
    }
    file.close();
  }
  
  /// State Stay methods
  bool stateStayIdle() {
    int input;
    cin >> input;
    switch (input) {
      case 1: changeState(State::STACK); break;
      case 2: changeState(State::QUEUE); break;
      case 3: changeState(State::LIST); break;
      case 4: return false;
    }
    return true;
  }
  bool stateStayStack() {
    clearScreen();
    drawStack();
    cout << "1. Push\n2. Pop\n3. Save & Move to Queue\n4. Save & Move to List\n5. Quit\n? ";
    int input;
    cin >> input;
    switch (input) {
      case 1: {
        char ch;
        cin >> ch;
        stack.push(ch);
        break;
      }
      case 2: if (!stack.empty()) stack.pop(); break;
      case 3: changeState(State::QUEUE); break;
      case 4: changeState(State::LIST); break;
      case 5: return false;
    }
    return true;
  }
  bool stateStayQueue() {
    clearScreen();
    drawQueue();
    cout << "1. Enqueue\n2. Dequeue\n3. Save & Move to Stack\n4. Save & Move to List\n5. Quit\n? ";
    int input;
    cin >> input;
    switch (input) {
      case 1: {
        char ch;
        cin >> ch;
        queue.push(ch);
        break;
      }
      case 2: if (!queue.empty()) queue.pop(); break;
      case 3: changeState(State::STACK); break;
      case 4: changeState(State::LIST); break;
      case 5: return false;
    }
    return true;
  }
  bool stateStayList() {
    clearScreen();
    drawList();
    cout << "1. Append\n2. Remove\n3. Save & Move to Stack\n4. Save & Move to Queue\n5. Quit\n? ";
    int input;
    cin >> input;
    switch (input) {
      case 1: {
        char ch;
        cin >> ch;
        list.push_back(ch);
        break;
      }
      case 2: if (!list.empty()) list.pop_back(); break;
      case 3: changeState(State::STACK); break;
      case 4: changeState(State::QUEUE); break;
      case 5: return false;
    }
    return true;
  }
  
  /// State Exit methods
  void stateExitIdle() {}
  void stateExitStack() {
    vector<char> stacklist;
    while (!stack.empty()) {
      stacklist.push_back(stack.top());
      stack.pop();
    }
    reverse(stacklist.begin(), stacklist.end());
    ofstream file(FILE_STACK);
    for (char ch : stacklist) {
      file << ch << ",";
    }
    file.close();
  }
  void stateExitQueue() {
    ofstream file(FILE_QUEUE);
    while (!queue.empty()) {
      file << queue.front() << ",";
      queue.pop();
    }
    file.close();
  }
  void stateExitList() {
    ofstream file(FILE_LIST);
    for (char ch : list) {
      file << ch << ",";
    }
    file.close();
  }

public:
  JumpTable() {
    stateEnterMeths[State::IDLE] = &JumpTable::stateEnterIdle;
    stateEnterMeths[State::STACK] = &JumpTable::stateEnterStack;
    stateEnterMeths[State::QUEUE] = &JumpTable::stateEnterQueue;
    stateEnterMeths[State::LIST] = &JumpTable::stateEnterList;

    stateStayMeths[State::IDLE] = &JumpTable::stateStayIdle;
    stateStayMeths[State::STACK] = &JumpTable::stateStayStack;
    stateStayMeths[State::QUEUE] = &JumpTable::stateStayQueue;
    stateStayMeths[State::LIST] = &JumpTable::stateStayList;

    stateExitMeths[State::IDLE] = &JumpTable::stateExitIdle;
    stateExitMeths[State::STACK] = &JumpTable::stateExitStack;
    stateExitMeths[State::QUEUE] = &JumpTable::stateExitQueue;
    stateExitMeths[State::LIST] = &JumpTable::stateExitList;

    state = State::IDLE;
  }
  
  void changeState(State newState) {
    if (state != newState) {
      if (stateExitMeths.find(state) != stateExitMeths.end()) {
        (this->*stateExitMeths[state])();
      }
      if (stateEnterMeths.find(newState) != stateEnterMeths.end()) {
        (this->*stateEnterMeths[newState])();
      }
      state = newState;
    }
  }
  bool doState() {
    if (stateStayMeths.find(state) != stateStayMeths.end()) {
      return (this->*stateStayMeths[state])();
    }
    return false;
  }
};

int main() {
  JumpTable jt;
  bool keepRunning = true;
  cout << "1. Stack\n2. Queue\n3. List\n4. Quit\n? ";
  while (keepRunning) {
    keepRunning = jt.doState();
  }
  jt.changeState(State::IDLE);
}