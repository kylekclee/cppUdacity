#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"
using std::string;
using std::to_string;
using std::vector;



int Process::Pid() const { return PID_; }


float Process::CpuUtilization() {
  vector<string> cpu_values = LinuxParser::CpuUtilization(PID_);
  //utime + stime
  float totalCPUTime = std::stof(cpu_values[0]) + std::stof(cpu_values[1]);
  
  long totalTimeUp = LinuxParser::UpTime();
  long processUpTime = Process::UpTime();
  long seconds = totalTimeUp - processUpTime;
 
  CPU_ = (totalCPUTime / sysconf(_SC_CLK_TCK)) / seconds;
  return  CPU_;
                                
  }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(PID_) ; }


string Process::Ram() { return LinuxParser::Ram(PID_); }


string Process::User() { return LinuxParser::User(PID_); }

long int Process::UpTime() { return (LinuxParser::UpTime(PID_)); }

bool Process::operator<(Process const& a) const { 
  return CPU_ < a.CPU_;
}