#ifndef PROCESS_H
  #define PROCESS_H

  #include <string>
  #include <map>

  using std::string;
  using std::map;

  class Process {
    public:
      string processName;
      unsigned int priority;
      unsigned int arrivalTime;
      map<char, int> history;
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

  };

#endif
