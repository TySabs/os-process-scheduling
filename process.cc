#include <iostream>
#include "process.h"

using namespace std;

Process::Process() {
  processName = "TestProcess";
}

void Process::hello() {
  cerr << "Hello processes!" << endl;
  cerr << "Process Name: " << processName << endl;
}
