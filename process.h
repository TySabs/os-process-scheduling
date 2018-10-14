#ifndef PROCESS_H
  #define PROCESS_H

  #include <string>
  #include <vector>

  using std::string;
  using std::pair;
  using std::vector;

  class Process {
    private:
      string buildPrefixType(string output);


    public:
      string processName;
      unsigned int priority;
      unsigned int arrivalTime;
      vector<pair<char, int> > history;
      int historyIndex;
      int cpuTimer;
      int ioTimer;
      int cpuTotal;
      int iTotal;
      int oTotal;
      int cpuCount;
      int iCount;
      int oCount;

      void hello();
      Process();
      Process(string, unsigned int, unsigned int);
      void calculateBurstCounts();
      void printEntryPop(unsigned int);
      void printQueuePush(string, unsigned int);
      void printActiveState(string, unsigned int);
      int runProcess();


  };

#endif
