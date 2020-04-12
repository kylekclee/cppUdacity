#include <string>
#include <iostream>
#include <iomanip>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
   int hours = (seconds / 60 / 60)%24;
   int minutes = (seconds / 60) %60;
   int second = seconds %60;
   std::ostringstream time;
  std::string timeStr;

  time << std::setfill('0') << std::setw(2) << hours << ":";
  time << std::setfill('0') << std::setw(2) << minutes<< ":";
  time << std::setfill('0') << std::setw(2) << second;
  timeStr = time.str();

  return timeStr; 
}