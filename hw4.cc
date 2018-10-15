/*********************************************************************
 * Name: Tyler Saballus
 * Date: 10/12/2018
 * Operating Systems 480 - Hutchins - Boley/Crumpton
 * Assignment 4 - Process Scheduling
 ***********************************************************************/

#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <iterator>
#include "process.h"

using std::cerr;
using std::string;
using std::endl;
using std::ifstream;
using std::vector;
using std::queue;
using std::priority_queue;
using std::make_pair;

const int MAX_TIME = 500,
            AT_ONCE = 5,
            HOW_OFTEN = 25;

// Used to sort priority queues by a processes's priority field
struct GreaterThanByPriority {
  bool operator()(const Process& lhs, const Process& rhs) const {
    return lhs.priority < rhs.priority;
  }
};

/***********************************************************************
 * printEntryQueue function
 *
 * Arguments(1):
 *   1. entryQ - the entry queue
 *
 * Returns: void
 *
 * Use: Prints the contents of the entry queue
 ***********************************************************************/
void printEntryQueue(queue<Process> entryQ) {
  cerr << "Contents of the Entry Queue: " << endl;

  if (entryQ.empty()) {
    cerr << "(Empty)" << endl;
  } else {
    for (unsigned int i = 0; i < entryQ.size(); i++) {
      Process entry = entryQ.front();

      cerr << entry.processName << " Priority = " << entry.priority
            << " arriving at time = " << entry.arrivalTime << endl;

      // Cycle entry theough the queue
      entryQ.pop();
      entryQ.push(entry);
    }
  }
} // end printEntryQueue function

/***********************************************************************
 * printPriorityQueue function
 *
 * Arguments(2):
 *   1. targetQ - the priority queue to be printed
 *   2. qName - the name of the priority queue
 *
 * Returns: void
 *
 * Use: Prints the contents of the priority queue
 ***********************************************************************/
void printPriorityQueue(priority_queue<Process, vector<Process>, GreaterThanByPriority> targetQ,
      string qName) {
  cerr << "Contents of the " << qName << " Queue: " << endl;

  if (targetQ.empty()) {
    cerr << "(Empty)" << endl;
  } else {
    // Create a copy of the targetQ, to be used for iteration
    priority_queue<Process, vector<Process>, GreaterThanByPriority> copyQueue = targetQ;

    // Loop through copy queue and print contents
    while (!copyQueue.empty()) {
      Process p = copyQueue.top();
      cerr << p.processName << "(" << p.priority << ")\t";
      copyQueue.pop();
    }

    cerr << endl;
  }
} // end printPriorityQueue function


/***********************************************************************
 * printProcesses function
 *
 * Arguments(3):
 *   1. aPtr - Active process
 *   2. iPtr - Input process
 *   3. oPtr - Output process
 *
 * Returns: void
 *
 * Use: Prints the Active, IActive, and OActive processes
 ***********************************************************************/
void printProcesses(Process *aPtr, Process *iPtr, Process *oPtr) {
  string activeName = "0", inputName = "0", outputName = "0";

  // Set ptr to process name if ptr is not null
  if (aPtr) activeName = aPtr->processName;
  if (iPtr) inputName = iPtr->processName;
  if (oPtr) outputName = oPtr->processName;

  cerr << "Active is " << activeName << endl;
  cerr << "IActive is " << inputName << endl;
  cerr << "OActive is " << outputName << endl << endl;
}

/***********************************************************************
 * printStatus function
 *
 * Arguments(8):
 *   1. currentTime - Current time of main timer
 *   2. entryQ - the entry queue
 *   3. readyQ - the ready queue
 *   4. inputQ - the input queue
 *   5. outputQ - the output queue
 *   6. aPtr - pointer to the Active process
 *   7. iPtr - pointer to the IActive process
 *   7. oPtr - pointer to the OActive process
 *
 * Returns: void
 *
 * Use: A status including many details about the program at HOW_OFTEN intervals
 ***********************************************************************/
void printStatus(unsigned int currentTime, queue<Process> entryQ,
        priority_queue<Process, vector<Process>, GreaterThanByPriority> readyQ,
        priority_queue<Process, vector<Process>, GreaterThanByPriority> inputQ,
        priority_queue<Process, vector<Process>, GreaterThanByPriority> outputQ,
        Process *aPtr, Process *iPtr, Process *oPtr) {

  cerr << "*********************************" << endl;
  cerr << "Status Update at " << currentTime << endl;

  printProcesses(aPtr, iPtr, oPtr);
  printEntryQueue(entryQ);
  printPriorityQueue(readyQ, "Ready");
  printPriorityQueue(inputQ, "Input");
  printPriorityQueue(outputQ, "Output");

  cerr << "*********************************" << endl << endl;

  cerr << endl;
} // end printStatus function


/***********************************************************************
 *
 *                        Main Program
 *
 ***********************************************************************/
int main(int argc, char *argv[]) {
  ifstream infile;
  queue<Process> entryQueue;
  priority_queue<Process, vector<Process>, GreaterThanByPriority> readyQueue,
        inputQueue, outputQueue;

  // Open the file
  infile.open("./data4.txt");

  // Error check for file open error
  if (!infile) {
    cerr << "Unable to open file data4.txt";
    exit(1);
  }

  string processName;
  unsigned int priority, arrivalTime;
  char burstType;
  int burstTime;

  // Loop through the input file entil delimiter is found
  while (true) {
    infile >> processName;

    // End loop if delimiter is found
    if (processName.compare("STOPHERE") == 0) {
      break;
    }

    infile >> priority;
    infile >> arrivalTime;

    // Make a new process
    Process newProcess(processName, priority, arrivalTime);

    // Populate process's history vector
    infile >> burstType;
    infile >> burstTime;
    while (burstType != 'N') {
      newProcess.history.push_back(make_pair(burstType, burstTime));
      infile >> burstType;
      infile >> burstTime;
    }

    // Place new process on the entry queue
    entryQueue.push(newProcess);

    // Discard remainder of the line
    string garbage;
    std::getline(infile, garbage);
  } // end while - input file

  // Close file
  infile.close();

  //////////////////////////////////////////////////
  // End Main Pt1: Initialization
  // Begin Main Pt2: Running Processes
  //////////////////////////////////////////////////

  unsigned int mainTimer = 0,
                idleTimeTotal = 0;

  int processCount = 0,
        terminatedCount = 0;
  Process *Active = NULL,
          *IActive = NULL,
          *OActive = NULL;

  bool isCpuActive = false, isInputActive = false, isOutputActive = false;
  Process activeProcess, inputProcess, outputProcess;

  // Main program loop
  while (mainTimer < MAX_TIME) {

    // Loop through entry queue
    for (unsigned int i = 0; i < entryQueue.size(); i++) {
      Process entryProcess = entryQueue.front();

      // Add entry to ready queue if arrival time has been reached and count is less than max
      if (entryProcess.arrivalTime <= mainTimer && processCount <= AT_ONCE) {
        entryProcess.printEntryPop(mainTimer);
        entryQueue.pop();
        readyQueue.push(entryProcess);
        processCount++;
      // Cycle through if arrival or max process conditions have not been met
      } else {
        entryQueue.pop();
        entryQueue.push(entryProcess);
      }

      // End loop if entry queue is now empty
      if (entryQueue.empty()) break;
    } // end entry queue loop

    // Activate a process from the ready queue
    if (!readyQueue.empty() && !isCpuActive) {
      activeProcess = readyQueue.top();
      Active = &activeProcess;
      readyQueue.pop();
      isCpuActive = true;
    }

    // Handle active process
    if (Active && isCpuActive) {
      isCpuActive = Active->runCpu();

      // If process has ran all its bursts, end the process
      if (Active->historyIndex > Active->history.size()-1 && !isCpuActive) {
        idleTimeTotal += Active->goodbye(mainTimer);
        processCount--;
        terminatedCount++;
        Active = nullptr;
      // Else put the process on the input or output queues
      } else if (!isCpuActive) {
        switch (Active->history[Active->historyIndex].first) {
          case 'I':
            inputQueue.push(*Active);
            Active = nullptr;
            break;
          case 'O':
            outputQueue.push(*Active);
            Active = nullptr;
            break;
        }
      }
    } // end Active process conditional

    // Activate an input process from the input queue
    if (!inputQueue.empty() && !isInputActive) {
      inputProcess = inputQueue.top();
      IActive = &inputProcess;
      inputQueue.pop();
      isInputActive = true;
    }

    // Handle input process
    if (IActive && isInputActive) {
      isInputActive = IActive->runIO();

      if (!isInputActive) {
        readyQueue.push(*IActive);
        IActive = nullptr;
      }
    } // end IActive process conditional

    // Activate an output process from the output queue
    if (!outputQueue.empty() && !isOutputActive) {
      outputProcess = outputQueue.top();
      OActive = &outputProcess;
      outputQueue.pop();
      isOutputActive = true;
    }

    // Handle output process
    if (OActive && isOutputActive) {
      isOutputActive = OActive->runIO();

      if (!isOutputActive) {
        readyQueue.push(*OActive);
        OActive = nullptr;
      }
    } // end OActive process conditional

    // Break out of main loop if all processes have terminated
    if (entryQueue.empty() && processCount == 0) break;

    // Print status every HOW_OFTEN (25) intervals
    if ((mainTimer % HOW_OFTEN) == 0) {
      printStatus(mainTimer, entryQueue, readyQueue, inputQueue, outputQueue,
                    Active, IActive, OActive);
    }

    mainTimer++;
  } // end main loop

  // Print end of program output
  cerr << "The run has ended." << endl;
  cerr << "Final value of the timer: " << mainTimer << endl;
  cerr << "The amount of time spent idle was " << idleTimeTotal << endl;
  cerr << "Number of terminated processes " << terminatedCount << endl;

  printProcesses(Active, IActive, OActive);
  printEntryQueue(entryQueue);
  printPriorityQueue(readyQueue, "Ready");
  printPriorityQueue(inputQueue, "Input");
  printPriorityQueue(outputQueue, "Output");

  cerr << endl;

  return 0;
} // end main function
