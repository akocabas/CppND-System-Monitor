#include "processor.h"
#include "linux_parser.h"
#include <iostream>


// DONE TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  long active = LinuxParser::ActiveJiffies();
  long total = LinuxParser::Jiffies();
  
  float utilization = (active - Processor::previous_active_) / (float)(total - Processor::previous_total_);
  /*
  std::cout << "CPU utilization: " << utilization << "\n";
  std::cout << "active: " << active << "\n";
  std::cout << "Processor::previous_active_: " << Processor::previous_active_ << "\n";
  std::cout << "total: " << total << "\n";
  std::cout << "Processor::previous_total_: " << Processor::previous_total_ << "\n";
  */
  Processor::previous_active_ = active;
  Processor::previous_total_ = total;
  
  return utilization;
}