#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <iostream>
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel,version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >>kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string line,mTotal,mFree;
  float memTotal, memFree;
  std::ifstream fstream(kProcDirectory + kMeminfoFilename );
  if(fstream.is_open()){
    //get memtotal data
  	std::getline(fstream,line);
    std::istringstream stream(line);
    stream >>mTotal >>memTotal;
    //get memfree data
  	std::getline(fstream,line);
    std::istringstream stream2(line);
    stream2 >>mFree >>memFree;
  }
  return memFree/memTotal; 
}


long LinuxParser::UpTime() { 
  long int upTime,idle;
  string line;	
  std::ifstream fstream(kProcDirectory + kUptimeFilename);
  if(fstream.is_open()){
  	std::getline(fstream,line);  
    std::istringstream stream(line);
    stream >> upTime >> idle;
	}
  return upTime;
}


vector<string> LinuxParser::CpuUtilization() {
  string proc,line;
  vector<string> cpuRet;
  std::ifstream stream(kProcDirectory + kStatFilename );
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream s(line);
    s >> proc;
    while(s >> proc){
    	cpuRet.push_back(proc);
    }
  }  
  return cpuRet;
}
vector<string> LinuxParser::CpuUtilization(int pid) {
  string proc,line;
  vector<string> cpuRet;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename );
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream s(line);
    
    //#14 utime - CPU time spent in user code, measured in clock ticks
    //#15 stime - CPU time spent in kernel code, measured in clock ticks
    //#16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
    //#17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
    //#22 starttime - Time when the process started, measured in clock ticks
    for(int i = 0;i <17;i++){ 
    	s >> proc;
    	if(i >=13){cpuRet.push_back(proc);}
    }
  }  
  return cpuRet;
}
int LinuxParser::TotalProcesses() { 
  string line;
  string key,value;
  std::ifstream stream(kProcDirectory + kStatFilename );
  while(std::getline(stream,line)){
    std::istringstream  stringstream(line);  
    stringstream >> key >> value;
    if( key == "processes") return (int)stoi(value);
  }
  return 0;
}


int LinuxParser::RunningProcesses() { 
   string line;
  string key,value;
  std::ifstream stream(kProcDirectory + kStatFilename );
  while(std::getline(stream,line)){
    std::istringstream  stringstream(line);  
    stringstream >> key >> value;
    if( key == "procs_running") return (int)stoi(value);
  }
  return 0;
   }

// TODO: Read and return the command associated with a process

string LinuxParser::Command(int pid) {
  string line;
  string cmd;
  string temp;
  std::ifstream fstream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  while(std::getline(fstream,line))
  {
    std::istringstream s(line);
    while(s>> temp){
    cmd += temp;
    }
  }
  return cmd; 
}


string LinuxParser::Ram(int pid) { 
  std::ifstream stream(kProcDirectory+ to_string(pid) + kStatusFilename);
  string line;
  string key,value;
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::istringstream s(line);
      s >> key >>value;
      if(key == "VmSize:"){
        return to_string(stoi(value) /1000);
      }
    }
  }
  return string(); 
}


string LinuxParser::Uid(int pid) { 
  string line,key,value;
  std::ifstream fstream(kProcDirectory+ to_string(pid) + kStatusFilename);
  //Uuid: 1000
  if(fstream.is_open()){
    while(std::getline(fstream,line)){
      std::istringstream s(line);
      s >> key >> value;
      if(key  == "Uid:"){
        return value;
      }
    }
  
  }
  return string(); 
}

string LinuxParser::User(int pid) { 
  string parsedId,line,name;
  string uid = LinuxParser::Uid(pid);
  string delimiter = ":";
  size_t pos = 0;
  std::ifstream fstream(LinuxParser::kPasswordPath);
  if(fstream.is_open()){
   while(std::getline(fstream,line)){
     //root:x:0:0:root
     pos = line.find(delimiter);
     name = line.substr(0,pos);
     line.erase(0,pos + delimiter.length());
     
     //x:0:0:root
     pos = line.find(delimiter);
     line.erase(0,pos + delimiter.length());
     
     //0:0:root
     parsedId = line.substr(0,line.find(delimiter));
     if(uid == parsedId) return name;
   }
  }
  return string();
}


long LinuxParser::UpTime(int pid) {
  long time;
  string line,value;
  std::ifstream stream(kProcDirectory+ to_string(pid) + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line))
    {
      std::istringstream s(line);
      for(int i = 0; i<22; i++){
       s >> value;
      }
      time = stof(value)/ sysconf(_SC_CLK_TCK); //sysconf is 100
      return  time ;
      
    }
  }
  return 0; 
}