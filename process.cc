#include <iostream>
#include <string>
#include <iterator>
#include "process.h"

using std::cerr;
using std::endl;
using std::pair;

Process::Process() {
  processName = "NULL PROCESS";
}

Process::Process(string newName, unsigned int newPriority, unsigned int newTime) {
  processName = newName;
  priority = newPriority;
  arrivalTime = newTime;
  historyIndex = 0;
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

string Process::buildPrefixType(string output) {
  if (output.compare("Entry Queue") == 0) {
    return "(E) - ";
  } else if (output.compare("Ready Queue") == 0) {
    return "(R) - ";
  } else if (output.compare("Active") == 0) {
    return "(A) - ";
  } else {
    return "";
  }
}

void Process::printEntryPop(unsigned int popTime) {
  cerr << processName << " moved from the Entry Queue into the Ready Queue at time "
    << popTime << endl << endl;
}

void Process::printQueuePush(string qType, unsigned int pushTime) {
  string prefix = buildPrefixType(qType);

  cerr << prefix << "Pushed " << processName << " process onto "
    << qType << " at " << pushTime << endl << endl;
}

void Process::printActiveState(string stateType, unsigned int pushTime) {
  string prefix = buildPrefixType(stateType);

  cerr << prefix << processName << " process is Active. Times Main/CPU: "
      << pushTime << " -- " << cpuTimer << endl << endl;
}

int Process::runProcess() {
  cpuTimer++;
  if (cpuTimer <= history[historyIndex].second) {
    return true;
  } else {
    historyIndex++;
    cpuTimer = 0;
    return false;
  }
}
