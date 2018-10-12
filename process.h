#ifndef PROCESS_H
  #define PROCESS_H

  #include <string>

  using std::string;

  class Process {
    public:
      string processName;
      uint priority;
      int arrivalTime;
      // TODO add History - an array of pairs of the form (letter, value) // max 10 pairs
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

  };

#endif
