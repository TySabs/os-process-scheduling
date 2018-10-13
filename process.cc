#include <iostream>
#include <string>
#include <map>
#include <iterator>
#include "process.h"

using std::cerr;
using std::endl;
using std::multimap;
using std::pair;

Process::Process() {
  processName = "NULL PROCESS";
}

Process::Process(string newName, unsigned int newPriority, unsigned int newTime) {
  processName = newName;
  priority = newPriority;
  arrivalTime = newTime;

  cpuTimer = 0;
  ioTimer = 0;
  cpuTotal = 0;
  iTotal = 0;
  oTotal = 0;
  cpuCount = 0;
  iCount = 0;
  oCount = 0;
}

void Process::hello() {
  cerr << "Process Name: " << processName << endl;
  cerr << "Priority: " << priority << endl;
  cerr << "Arrival: " << arrivalTime << endl;
  cerr << "--------------" << endl;


  vector<pair<char, int> >::iterator it;
  for (it = history.begin(); it != history.end(); ++it) {
    cerr << "Burst Type/Time: " << it->first << " -- " << it->second << endl;
  }

  cerr << "Counts (C/I/O): " << cpuCount << " -- " << iCount << " -- " << oCount << endl;

  cerr << endl;
}

void Process::calculateBurstCounts() {
  vector<pair<char, int> >::iterator it;
  for (it = history.begin(); it != history.end(); ++it) {
      if (it->first == 'C') cpuCount++;
      if (it->first == 'I') iCount++;
      if (it->first == 'O') oCount++;
  }
}

void Process::printReadyPush(int pushTime) {
  cerr << "Pushed " << processName << " process at " << pushTime << endl; 
}
