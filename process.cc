#include <iostream>
#include <string>
#include "process.h"

using std::cerr;
using std::endl;

Process::Process() {
  processName = "NULL PROCESS";
}

Process::Process(string newName, unsigned int newPriority, unsigned int newTime) {
  processName = newName;
  priority = newPriority;
  arrivalTime = newTime;

}

void Process::hello() {
  cerr << "Hello processes!" << endl;
  cerr << "Process Name: " << processName << endl;
  cerr << "Priority: " << priority << endl;
  cerr << "Arrival: " << arrivalTime << endl;
}
