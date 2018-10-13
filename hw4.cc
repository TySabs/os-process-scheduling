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
using std::make_pair;

const int MAX_TIME = 500,
            AT_ONCE = 5,
            QUEUE_SIZE = 20,
            HOW_OFTEN = 25;

int main(int argc, char *argv[]) {
  ifstream infile;
  queue<Process> entryQueue, readyQueue;
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

    newProcess.calculateBurstCounts();

    processPool.push_back(newProcess);

    string garbage;
    std::getline(infile, garbage);
  }

  infile.close();

  unsigned int mainTimer = 0;

  while (mainTimer < MAX_TIME && !processPool.empty()) {


    vector<Process>::iterator it;
    for (it = processPool.begin(); it != processPool.end(); ++it) {
      if (it->arrivalTime <= mainTimer) {
        it->printReadyPush(mainTimer);
        entryQueue.push(*it);
        processPool.erase(it);
      }

      if (processPool.empty()) break;
    }


    mainTimer++;
  }

  cerr << "Final time: " << mainTimer << endl;

  return 0;
}
