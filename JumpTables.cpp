#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>

// Added this import for some string manipulation
#include <sstream>
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
    
    // similar to saveStack, we first move 'stack' contents to 'stacklist'
    while(!stack.empty()){
      stacklist.push_back(stack.top());
      stack.pop();
    }

    // draw the contents of stacklist on to terminal
    cout << "|     |"  << endl;
    cout << "| --- |" << endl;

    // Grabs the elements from the vector and then will display it to the terminal
    for(char c : stacklist){
      cout << "|  " << c << "  |" << endl;
      cout << "| --- |" << endl;
    }

    // reverse the contents of stacklist
    reverse(stacklist.begin(), stacklist.end());

    // put stacklist contents back into 'stack'
    for(char c : stacklist){
      stack.push(c);
    }
  }

  void drawQueue() {
    vector<char> queuelist;
    
    // similar to the stack, we first move 'queue' contents to 'queuelist'
    while(!queue.empty()){
      queuelist.push_back(queue.front());
      queue.pop();
    }
    // draw the contents of queuelist on to terminal
    cout << "|";

    // Grabs the elements from the vector and then will display it to the terminal
    for(char c : queuelist){
      cout << " " << c << " |";
    }

    cout << " " << endl;

    // do NOT reverse the contents
    // put queuelist contents back into 'queue'
    for(char c : queuelist){
      queue.push(c);
    }
  }

  void drawList() {
    // fortunately, C++ allows us to examine the contents of a vector without destroying it
    // so we can just iterate over the 'list' and draw the contents to the terminal
    cout << "{ ";

    // Initializing the drawing of the list
    for(char c : list){
      cout << c << ", ";
    }

    cout << " }" << endl;
  }
  
  /// State Enter methods
  void stateEnterIdle() {
    // this method can be left blank
  }

  void stateEnterStack() {
    // ensure 'stack' is cleared
    while(true){
      // Checks if the stack is empty and if not then it will pop the contents of the stack
      if(!stack.empty()){
        stack.pop();
      }
      else {
        break;
      }
    }

    // read from FILE_STACK file
    ifstream infile(FILE_STACK);
    string lineFile;
    getline(infile, lineFile);

    // Split the line by commas
    vector<char> stackCharVector(lineFile.begin(), lineFile.end());

    for(char c :stackCharVector){
      if(c != ','){
        // push into 'stack' the values from file
        stack.push(c);
      }
    }

    // close file
    infile.close();
  }
  
  void stateEnterQueue() {
    // ensure 'queue' is cleared
    while(true){
      // Checks if the queue is empty and if not then it will pop the contents of the queue
      if(!queue.empty()){
        queue.pop();
      }
      else {
        break;
      }
    }

    // read from FILE_QUEUE file
    ifstream infile(FILE_QUEUE);
    string lineFile;
    getline(infile, lineFile);

    // push into 'queue' the values from file
    vector<char> queueCharVector(lineFile.begin(), lineFile.end());

    for(char c :queueCharVector){
      if(c != ','){
        // push into 'queue' the values from file
        queue.push(c);
      }
    }

    // close file
    infile.close();
  }

  void stateEnterList() {
    // ensure 'list' is cleared
    list.clear();

    // read from FILE_LIST file
    ifstream infile(FILE_LIST);
    string lineFile;
    getline(infile, lineFile);

    // push_back into 'list' the values from file
    vector<char> listCharVector(lineFile.begin(), lineFile.end());

    for(char c :listCharVector){
      if(c != ','){
        // push into 'list' the values from file
        list.push_back(c);
      }
    }

    // close file
    infile.close();
  }
  
  /// State Stay methods
  bool stateStayIdle() {
    int input;
    cin >> input;
    switch(input) {
      case 1: changeState(State::STACK); break;
      case 2: changeState(State::QUEUE); break;
      case 3: changeState(State::LIST); break;
      case 4: return false;
    }
    return true;
  }

  bool stateStayStack() {
    // call clear screen
    clearScreen();

    // call draw stack
    drawStack();

    // draw the 5 options for the stack menu
    cout << "1. Push" << endl;
    cout << "2. Pop" << endl;
    cout << "3. Save & Move to Queue" << endl;
    cout << "4. Save & Move to List" << endl;
    cout << "5. Quit" << endl;

    // end with a "? " right under the menu
    cout << "? ";

    // grab input choice from user
    string input;
    getline(cin, input);

    // Used for splitting the input
    stringstream ss(input);
    int option = 0;
    char value = '\0';

    // Grabs the first part of the input and makes it an integer of option
    ss >> option;

    // perform action
    // Switch case the will perom all of the options, push, pop, and change the state once called
    switch(option) {
      case 1:
        if(ss >> value){
          stack.push(value);
        } 
        else{
          cout << "Invalid input. Please provide a value for push function" << endl;
        }
        break;

      case 2:
        if(!stack.empty()){
          stack.pop();
        }
        else{
          cout << "Stack is empty." << endl;
        }
        break;

      case 3: changeState(State::QUEUE); break;
      case 4: changeState(State::LIST); break;

      // return false if user choice was to quit, otherwise return true
      case 5: return false;

      default: cout << "Invalid option. Please try again." << endl;
    }

    return true;

  }

  bool stateStayQueue() {
    // call clear screen
    clearScreen();

    // call draw queue
    drawQueue();
    
    // draw the 5 options for the queue menu
    cout << "1. Enqueue" << endl;
    cout << "2. Dequeue" << endl;
    cout << "3. Save & Move to Stack" << endl;
    cout << "4. Save & Move to List" << endl;
    cout << "5. Quit" << endl;

    // end with a "? " right under the menu
    cout << "? ";
    
    // grab input choice from user
    string input;
    getline(cin, input);

    // Used for splitting the input
    stringstream ss(input);
    int option = 0;
    char value = '\0';

    // Grabs the first part of the input and makes it an integer of option
    ss >> option;

    // perform action
    // Switch case the will perom all of the options, push, pop, and change the state once called
    switch(option) {
      case 1: 
        if(ss >> value){
          queue.push(value);
        }
        else{
          cout << "Invalid input, please insert option and value for enqueue function" << endl;
        }
        break;

      case 2: 
        if(!queue.empty()){
          queue.pop();
        }
        else{
          cout << "Queue is empty" << endl;
        }
        break;

      case 3: changeState(State::STACK); break;
      case 4: changeState(State::LIST); break;

      // return false if user choice was to quit, otherwise return true
      case 5: return false;

      default: cout << "Invalid option. Please try again." << endl;
    }

    return true;
  }

  bool stateStayList() {
    // call clear screen
    clearScreen();

    // call draw queue
    drawList();
    
    // draw the 5 options for the queue menu
    cout << "1. Append" << endl;
    cout << "2. Remove" << endl;
    cout << "3. Save & Move to Stack" << endl;
    cout << "4. Save & Move to Queue" << endl;
    cout << "5. Quit" << endl;

    // end with a "? " right under the menu
    cout << "? ";
    
    // grab input choice from user
    string input;
    getline(cin, input);

    // Splits the string
    stringstream ss(input);
    int option = 0;
    char value = '\0';

    // Grabs the first part of the input and makes it an integer of option
    ss >> option;

    // perform action
    // Switch case the will perom all of the options, push, pop, and change the state once called
    switch(option) {
      case 1: 
        if(ss >> value){
          list.push_back(value);
        }
        else{
          cout << "Invalid input, please insert option and value for append function" << endl;
        }
        break;

      case 2: 
        if(!list.empty()){
          list.pop_back();
        }
        else{
          cout << "List is empty" << endl;
        }
        break;

      case 3: changeState(State::STACK); break;
      case 4: changeState(State::QUEUE); break;

      // return false if user choice was to quit, otherwise return true
      case 5: return false;

      default: cout << "Invalid option. Please try again." << endl;
    }

    return true;
  }
  
  /// State Exit methods
  void stateExitIdle() {
    // this method does nothing
  }

  void stateExitStack() {
    vector<char> stacklist;
    
    // move values from 'stack' into 'stacklist'
    while(!stack.empty()){
      stacklist.push_back(stack.top());
      stack.pop();
    }

    // it's ok if stack is empty after this
    // reverse the items in stacklist
    reverse(stacklist.begin(), stacklist.end());

    // open FILE_STACK file
    ofstream outFile(FILE_STACK, ios::out | ios::trunc);

    // put contents of stacklist into file, with commas after values
    string tempString = "";
    for(char c : stacklist){
      tempString += c;
      tempString += ",";
    }

    outFile << tempString;

    // close file
    outFile.close();
  }

  void stateExitQueue() {
    // we do NOT need to copy values over to another variable
    // open FILE_QUEUE file
    ofstream outFile(FILE_QUEUE, ios::out | ios::trunc);

    // put contents of queue into file, with commas after values
    string tempString = "";
    while(!queue.empty()){
      char c = queue.front();
      tempString += c;
      tempString += ",";
      queue.pop();
    }

    outFile << tempString;

    // its ok if queue is empty after this
    // close file
    outFile.close();
  }

  void stateExitList() {
    // we do NOT need to copy values over to another variable
    // open FILE_LIST file
    ofstream outFile(FILE_LIST, ios::out | ios::trunc);

    // put contents of list into file, with commas after values
    string tempString = "";
    for(char c : list){
      tempString += c;
      tempString += ",";
    }

    outFile << tempString;

    // its ok if list is empty after this
    // close file
    outFile.close();
  }

public:
  JumpTable() {
    stateEnterMeths[State::IDLE] = stateEnterIdle;
    stateEnterMeths[State::STACK] = stateEnterStack;
    stateEnterMeths[State::QUEUE] = stateEnterQueue;
    stateEnterMeths[State::LIST] = stateEnterList;

    stateStayMeths[State::IDLE] = stateStayIdle;
    stateStayMeths[State::STACK] = stateStayStack;
    stateStayMeths[State::QUEUE] = stateStayQueue;
    stateStayMeths[State::LIST] = stateStayList;

    stateExitMeths[State::IDLE] = stateExitIdle;
    stateExitMeths[State::STACK] = stateExitStack;
    stateExitMeths[State::QUEUE] = stateExitQueue;
    stateExitMeths[State::LIST] = stateExitList;

    state = State::IDLE;
  }
  
  /// Change and Do State methods
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
    else {
      return false;
    }
  }
};

/// MAIN
int main() {
  JumpTable jt;
  bool keepRunning = true;
  cout << "1. Stack" << endl;
  cout << "2. Queue" << endl;
  cout << "3. List" << endl;
  cout << "4. Quit" << endl;
  cout << "? ";
  while(keepRunning) {
    keepRunning = jt.doState();
  }
  jt.changeState(State::IDLE);
}
