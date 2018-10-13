/*********************************************************************
 * Name: Tyler Saballus
 * Date: 10/12/2018
 * Operating Systems 480 - Hutchins - Boley/Crumpton
 * Assignment 4 - Process Scheduling
 ***********************************************************************/

#include <iostream>
#include <queue>
#include <fstream>
#include <map>
#include <iterator>
#include "process.h"

using std::cerr;
using std::string;
using std::endl;
using std::ifstream;
using std::map;
using std::queue;

const int MAX_TIME = 500,
            AT_ONCE = 5,
            QUEUE_SIZE = 20,
            ARRAY_SIZE = 10,
            HOW_OFTEN = 25;

int main(int argc, char *argv[]) {
  ifstream infile;
  queue<Process> entryQueue;

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
      newProcess.history[burstType] = burstTime;

      infile >> burstType;
      infile >> burstTime;
    }

    // p.hello();
    entryQueue.push(newProcess);

    string garbage;
    std::getline(infile, garbage);
  }

  infile.close();

  while (!entryQueue.empty()) {
    Process p = entryQueue.front();
    p.hello();
    entryQueue.pop();
  }


  return 0;
}
