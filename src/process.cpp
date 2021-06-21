#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  cpu_utilization_ = CpuUtilization();
  //std::cout << "cpu utilization: " << cpu_utilization_ << "\n";
}

// TODO: Return this process's ID

int Process::Pid() {
  return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  //std::cout << "Process::CpuUtilization()" << "\n";
  //std::cout << "(LinuxParser::UpTime() - LinuxParser::UpTime(pid_): " << LinuxParser::UpTime() - LinuxParser::UpTime(pid_) << "\n";
  
  float cpu_utilization = (((float) LinuxParser::ActiveJiffies(pid_) / (float) sysconf(_SC_CLK_TCK))) / ((float) LinuxParser::UpTime() - (float) LinuxParser::UpTime(pid_));
  //cpu_utilization *= 100;
  //std::cout << "cpu utilization: " << cpu_utilization << "\n";
  return cpu_utilization; 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  //std::cout << "this->cpu_utilization_: " << this->cpu_utilization_ << " " << "a.cpu_utilization_: " << a.cpu_utilization_ << "\n";
  return this->cpu_utilization_ > a.cpu_utilization_; 
}