/*********************************************************************
 * Name: Tyler Saballus
 * Date: 10/12/2018
 * Operating Systems 480 - Hutchins - Boley/Crumpton
 * Assignment 4 - Process Scheduling
 ***********************************************************************/

#include <iostream>
#include <queue>
#include "process.h"

using namespace std;

const int MAX_TIME = 500,
            AT_ONCE = 5,
            QUEUE_SIZE = 20,
            ARRAY_SIZE = 10,
            HOW_OFTEN = 25;

int main(int argc, char *argv[]) {
  cerr << "Hello World!" << endl;

  Process p;

  p.hello();

  return 0;
}
