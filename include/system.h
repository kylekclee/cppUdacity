#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();                   
  std::vector<Process>& Processes();  
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem();     

  // TODO: Define any necessary private members
 private:
  std::string kernel_;
  std::string operatingSystem_;
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif