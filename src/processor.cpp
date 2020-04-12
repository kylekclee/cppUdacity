#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

using std::stof;

float Processor::Utilization() { 

  static long prevNonIdle = 0;
  static long prevIdle = 0;
  std::vector<std::string> cpuValues = LinuxParser::CpuUtilization() ;
    //return stoi(cpuValues[0]);
  //idle
  long idle = stof(cpuValues[3]);
  long iowait = stof(cpuValues[4]);

  //non idle
  long user = stof(cpuValues[0]);
  long nice = stof(cpuValues[1]);
  long system = stof(cpuValues[2]);
  long irq = stof(cpuValues[5]);
  long softirq = stof(cpuValues[6]);
  long steal = stof(cpuValues[7]);
  
  
  long Idle = idle + iowait;
  long NonIdle = user + nice + system + irq + softirq + steal;
  
  float deltaIdle = Idle - prevIdle;
  float deltaNonIdle = NonIdle - prevNonIdle;
  float deltaTotal = deltaNonIdle + deltaIdle;
  
  float cpu_percentage = (deltaTotal - deltaIdle)/deltaTotal;
  
  prevIdle = Idle;
  prevNonIdle = NonIdle;
  
  return cpu_percentage; 
}

