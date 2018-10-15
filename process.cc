/*********************************************************************
 * Name: Tyler Saballus
 * Date: 10/12/2018
 * Operating Systems 480 - Hutchins - Boley/Crumpton
 * Assignment 4 - Process Scheduling
 ***********************************************************************/

#include <iostream>
#include <iterator>
#include "process.h"

using std::cerr;
using std::endl;

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
  readyTime = 0;
}

/***********************************************************************
 * Process.goodbye method
 *
 * Arguments(1):
 *  1. endTime - the time that this process terminated
 *
 * Returns: and unsigned int - time spent idle
 *
 * Use: Displays info about the process at time of termination
 ***********************************************************************/
unsigned int Process::goodbye(unsigned int endTime) {
  cerr << "\tTerminating Process" << endl;
  cerr << "><><><><><><><><><><><><><><<><><" << endl;
  cerr << "Process Name: " << processName << endl;
  cerr << "Priority: " << priority << endl;
  cerr << "Arrival Time: " << arrivalTime << endl;
  cerr << "Start Time: " << readyTime << endl;
  cerr << "Termination Time: " << endTime << endl << endl;
  cerr << "\tHistory" << endl;
  cerr << "-------------------------" << endl;

  // Loop through history vector
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
} // end Process.goodbye method


/***********************************************************************
 * Process.calculateIdle method
 *
 * Arguments(1):
 *  1. endTime - the time that this process terminated
 *
 * Returns: and unsigned int - time spent idle
 *
 * Use: Calculates the time spent idle
 ***********************************************************************/
unsigned int Process::calculateIdleTime(unsigned int endTime) {
  unsigned int idleTime = endTime - readyTime - cpuTotal - iTotal - oTotal;
  return idleTime;
}

/***********************************************************************
 * Process.calculateBurstCounts method
 *
 * Arguments(0):
 *
 * Returns: void
 *
 * Use: Calculates the count of bursts for each type of burst
 ***********************************************************************/
void Process::calculateBurstCounts() {
  vector<pair<char, int> >::iterator it;
  for (it = history.begin(); it != history.end(); ++it) {
      if (it->first == 'C') cpuCount++;
      if (it->first == 'I') iCount++;
      if (it->first == 'O') oCount++;
  }
}

/***********************************************************************
 * Process.printEntryPop method
 *
 * Arguments(1):
 *  1. popTime - the time that this process was popped off the entryQueue
 *
 * Returns: void
 *
 * Use: Prints the entry that has been popped off the entry queue
 ***********************************************************************/
void Process::printEntryPop(unsigned int popTime) {
  readyTime = popTime;

  cerr << processName << " moved from the Entry Queue into the Ready Queue at time "
    << popTime << endl << endl;
}

/***********************************************************************
 * Process.runCpu method
 *
 * Arguments(0): None
 *
 * Returns: bool - true if still running, false if done
 *
 * Use: Runs the Cpu for one tick and indicates if process has ended
 ***********************************************************************/
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

/***********************************************************************
 * Process.runIO method
 *
 * Arguments(0): None
 *
 * Returns: bool - true if still running, false if done
 *
 * Use: Runs I/O for one tick and indicates if process has ended
 ***********************************************************************/
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
