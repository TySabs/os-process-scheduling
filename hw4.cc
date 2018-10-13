/*********************************************************************
 * Name: Tyler Saballus
 * Date: 10/12/2018
 * Operating Systems 480 - Hutchins - Boley/Crumpton
 * Assignment 4 - Process Scheduling
 ***********************************************************************/

#include <iostream>
#include <queue>
#include <fstream>
#include "process.h"

using std::cerr;
using std::string;
using std::endl;
using std::ifstream;

const int MAX_TIME = 500,
            AT_ONCE = 5,
            QUEUE_SIZE = 20,
            ARRAY_SIZE = 10,
            HOW_OFTEN = 25;

int main(int argc, char *argv[]) {
  ifstream infile;

  infile.open("./data4.txt");

  if (!infile) {
    cerr << "Unable to open file data4.txt";
    exit(1);
  }

  string processName;
  unsigned int priority, arrivalTime;

  infile >> processName;
  infile >> priority;
  infile >> arrivalTime;

  Process p(processName, priority, arrivalTime);
  p.hello();

  infile.close();


  return 0;
}
