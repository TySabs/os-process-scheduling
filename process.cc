#include <iostream>
#include <string>
#include <map>
#include <iterator>
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
  cerr << "Process Name: " << processName << endl;
  cerr << "Priority: " << priority << endl;
  cerr << "Arrival: " << arrivalTime << endl;
  cerr << "--------------" << endl;


  map<char, int>::iterator it;
  for (it = history.begin(); it != history.end(); ++it) {
    cerr << "Burst Type - Time: " << it->first << " -- " << it->second << endl;
  }

  cerr << endl;
}
