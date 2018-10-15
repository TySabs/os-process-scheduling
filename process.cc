/*********************************************************************
 * Name: Tyler Saballus
 * Date: 10/12/2018
 * Operating Systems 480 - Hutchins - Boley/Crumpton
 * Assignment 4 - Process Scheduling
 ***********************************************************************/

#include <iostream>
#include <string>
#include <iterator>
#include "process.h"

using std::cerr;
using std::endl;
using std::pair;


/***********************************************************************
 * Process Constructor
 *
 * Arguments(0): None
 *
 * Use: Default constructor, not meant to be used
 ***********************************************************************/
Process::Process() {
  processName = "NULL PROCESS";
}

/***********************************************************************
 * Process Constructor
 *
 * Arguments(3):
 *  1. newName - the name of the process
 *  2. newPriority - the process priority
 *  3. newTime - the arrival time of the process
 *
 * Use: Main constructor - instantiates processes
 ***********************************************************************/
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

unsigned int Process::goodbye(unsigned int endTime) {
  cerr << "\tTerminating Process" << endl;
  cerr << "><><><><><><><><><><><><><><<><><" << endl;
  cerr << "Process Name: " << processName << endl;
  cerr << "Priority: " << priority << endl;
  cerr << "Arrival Time: " << arrivalTime << endl;
  cerr << "Termination Time: " << endTime << endl << endl;
  cerr << "\tHistory" << endl;
  cerr << "-------------------------" << endl;


  vector<pair<char, int> >::iterator it;
  for (it = history.begin(); it != history.end(); ++it) {
    cerr << "Burst Type: "  << it->first <<  " -- Time: " << it->second << endl;
  }

  calculateBurstCounts();
  unsigned int idleTime = calculateIdleTime(endTime);

  cerr << "Total CPU Time: " << cpuTotal << " in " << cpuCount <<  " CPU bursts" << endl;
  cerr << "Total Input Time: " << iTotal << " in " << iCount <<  " Input bursts" << endl;
  cerr << "Total Output Time: " << oTotal << " in " << oCount <<  " Output bursts" << endl;
  cerr << "Time spent waiting: " << idleTime << endl;

  cerr << "><><><><><><><><><><><><><><<><><" << endl << endl;

  return idleTime;
}

unsigned int Process::calculateIdleTime(unsigned int endTime) {
  unsigned int idleTime = endTime - arrivalTime - cpuTotal - iTotal - oTotal;
  return idleTime;
}

void Process::calculateBurstCounts() {
  vector<pair<char, int> >::iterator it;
  for (it = history.begin(); it != history.end(); ++it) {
      if (it->first == 'C') cpuCount++;
      if (it->first == 'I') iCount++;
      if (it->first == 'O') oCount++;
  }
}

void Process::printEntryPop(unsigned int popTime) {
  cerr << processName << " moved from the Entry Queue into the Ready Queue at time "
    << popTime << endl << endl;
}

bool Process::runCpu() {
  cpuTimer++;
  if (cpuTimer <= history[historyIndex].second) {
    cpuTotal++;
    return true;
  } else {
    historyIndex++;
    cpuTimer = 0;
    return false;
  }
}

bool Process::runIO() {
  ioTimer++;
  if (ioTimer <= history[historyIndex].second) {
    (history[historyIndex].first == 'I') ? iTotal++ : oTotal++;
    return true;
  } else {
    historyIndex++;
    ioTimer = 0;
    return false;
  }
}
