#ifndef PROCESS_H
  #define PROCESS_H

  /*********************************************************************
   * Name: Tyler Saballus
   * Date: 10/12/2018
   * Operating Systems 480 - Hutchins - Boley/Crumpton
   * Assignment 4 - Process Scheduling
   ***********************************************************************/

  #include <string>
  #include <vector>

  using std::string;
  using std::pair;
  using std::vector;

  class Process {
    private:
      // Private Data members
      int cpuTimer;
      int ioTimer;
      int cpuTotal;
      int iTotal;
      int oTotal;
      int cpuCount;
      int iCount;
      int oCount;
      int readyTime;

      // Private Methods
      void calculateBurstCounts();
      unsigned int calculateIdleTime(unsigned int);

    public:
      // Public Data members
      string processName;
      unsigned int priority;
      unsigned int arrivalTime;
      vector<pair<char, int> > history;
      unsigned int historyIndex;

      // Construcotrs
      Process();
      Process(string, unsigned int, unsigned int);

      // Public methods
      void printEntryPop(unsigned int);
      bool runCpu();
      bool runIO();
      unsigned int goodbye(unsigned int);
  };

#endif
