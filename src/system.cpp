#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


System::System(){
	kernel_ = LinuxParser::Kernel();
  	operatingSystem_ = LinuxParser::OperatingSystem();
}


Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  processes_.erase(processes_.begin(),processes_.end());
  for(int pid : pids)
  {
    Process proc(pid);
    processes_.push_back( proc);
  }
  std::sort(processes_.begin(),processes_.end());
  std::reverse(processes_.begin(),processes_.end());
  return processes_; 
}

string System::Kernel() { return kernel_; } 

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return operatingSystem_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }