#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  std::ostringstream os;
  
  int hours = seconds / (60 * 60);
  seconds = seconds % (60 * 60);
  int mins = seconds / 60;
  seconds = seconds % 60;
  os << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << mins << ":" << std::setw(2) << seconds;
  return os.str();  
}