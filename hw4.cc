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

struct GreaterThanByPriority {
  bool operator()(const Process& lhs, const Process& rhs) const {
    return lhs.priority < rhs.priority;
  }
};

void printEntryQueue(queue<Process> entryQ) {
  cerr << "Contents of the Entry Queue: " << endl;

  if (entryQ.empty()) {
    cerr << "(Empty)" << endl;
  } else {
    for (unsigned int i = 0; i < entryQ.size(); i++) {
      Process entry = entryQ.front();

      cerr << entry.processName << " Priority = " << entry.priority
            << " arriving at time = " << entry.arrivalTime << endl;

      entryQ.pop();
      entryQ.push(entry);
    }
  }
}

void printPriorityQueue(priority_queue<Process, vector<Process>, GreaterThanByPriority> targetQ,
      string qName) {
  cerr << "Contents of the " << qName << " Queue: " << endl;

  if (targetQ.empty()) {
    cerr << "(Empty)" << endl;
  } else {
    priority_queue<Process, vector<Process>, GreaterThanByPriority> copyQueue = targetQ;
    while (!copyQueue.empty()) {
      Process p = copyQueue.top();
      cerr << p.processName << "(" << p.priority << ")\t";
      copyQueue.pop();
    }

    cerr << endl;
  }
}

void printStatus(unsigned int currentTime, queue<Process> entryQ,
        priority_queue<Process, vector<Process>, GreaterThanByPriority> readyQ,
        priority_queue<Process, vector<Process>, GreaterThanByPriority> inputQ,
        priority_queue<Process, vector<Process>, GreaterThanByPriority> outputQ,
        Process* aPtr, Process* iPtr, Process* oPtr) {

  cerr << "Status Update at " << currentTime << endl;

  string activeName = "0", inputName = "0", outputName = "0";

  // Set ptr to process name if ptr is not null
  if (aPtr) activeName = aPtr->processName;
  if (iPtr) inputName = iPtr->processName;
  if (oPtr) outputName = oPtr->processName;

  cerr << "Active is " << activeName << endl;
  cerr << "IActive is " << inputName << endl;
  cerr << "OActive is " << outputName << endl << endl;

  printEntryQueue(entryQ);
  printPriorityQueue(readyQ, "Ready");
  printPriorityQueue(inputQ, "Input");
  printPriorityQueue(outputQ, "Output");
}

int main(int argc, char *argv[]) {
  ifstream infile;
  queue<Process> entryQueue;
  priority_queue<Process, vector<Process>, GreaterThanByPriority> readyQueue,
        inputQueue, outputQueue;
  vector<Process> processPool;

  infile.open("./data4.txt");

  if (!infile) {
    cerr << "Unable to open file data4.txt";
    exit(1);
  }

  string processName;
  unsigned int priority, arrivalTime;
  char burstType;
  int burstTime;

  while (true) {
    infile >> processName;

    if (processName.compare("STOPHERE") == 0) {
      break;
    }

    infile >> priority;
    infile >> arrivalTime;

    Process newProcess(processName, priority, arrivalTime);

    infile >> burstType;
    infile >> burstTime;


    while (burstType != 'N') {
      newProcess.history.push_back(make_pair(burstType, burstTime));
      infile >> burstType;
      infile >> burstTime;
    }

    entryQueue.push(newProcess);

    string garbage;
    std::getline(infile, garbage);
  }

  infile.close();

  unsigned int mainTimer = 0;
  int processCount = 0;
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
      if (entryProcess.arrivalTime <= mainTimer && processCount <= AT_ONCE) {
        entryProcess.printEntryPop(mainTimer);
        entryQueue.pop();
        readyQueue.push(entryProcess);
        processCount++;
      } else {
        entryQueue.pop();
        entryQueue.push(entryProcess);
      }

      if (entryQueue.empty()) break;
    } // end entry queue loop

    // Loop through ready queue
    while (!readyQueue.empty() && !isCpuActive) {
      activeProcess = readyQueue.top();
      Active = &activeProcess;
      readyQueue.pop();
      isCpuActive = true;
    }

    // Handle active process
    if (Active && isCpuActive) {
      isCpuActive = Active->runCpu();

      if (Active->historyIndex > Active->history.size()-1 && !isCpuActive) {
        Active->goodbye(mainTimer);
        processCount--;
        Active = nullptr;
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

    // Loop through input queue
    while (!inputQueue.empty() && !isInputActive) {
      inputProcess = inputQueue.top();
      IActive = &inputProcess;
      inputQueue.pop();
      isInputActive = true;
    }

    // Handle active process
    if (IActive && isInputActive) {
      isInputActive = IActive->runIO();
      // IActive->printActiveState("Input", mainTimer);

      if (!isInputActive) {
        readyQueue.push(*IActive);
        IActive = nullptr;
      }
    } // end IActive process conditional

    // Loop through input queue
    while (!outputQueue.empty() && !isOutputActive) {
      outputProcess = outputQueue.top();
      OActive = &outputProcess;
      outputQueue.pop();
      isOutputActive = true;
    }

    // Handle active process
    if (OActive && isOutputActive) {
      isOutputActive = OActive->runIO();

      if (!isOutputActive) {
        readyQueue.push(*OActive);
        OActive = nullptr;
      }
    } // end IActive process conditional

    if (entryQueue.empty() && processCount == 0) break;

    if ((mainTimer % HOW_OFTEN) == 0) {
      printStatus(mainTimer, entryQueue, readyQueue, inputQueue, outputQueue,
                    Active, IActive, OActive);
    }

    mainTimer++;
  } // end main loop

  cerr << "Final time: " << mainTimer << endl;

  return 0;
}
