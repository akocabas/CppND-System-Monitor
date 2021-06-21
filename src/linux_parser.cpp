#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <numeric>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  //std::cout << "LinuxParser::OperatingSystem()" << "\n";
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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  //std::cout << "LinuxParser::Pids()" << "\n";

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

// DONE TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  //std::cout << "LinuxParser::MemoryUtilization()" << "\n";

  string line;
  string key;
  string value;
  float mem_total;
  float mem_free;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "MemTotal:"){
          mem_total = stof(value);
        }
        if (key == "MemFree:"){
          mem_free = stof(value);
        }
      }
    }
  }
  return (mem_total - mem_free) / mem_total; 
}

// DONE TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  //std::cout << "LinuxParser::UpTime()" << "\n";

  string line;
  string value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    //std::cout << "Uptime is: " << value << "\n";
    return stol(value);
  }
  return stol(value); 
}

// DONE TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  //std::cout << "LinuxParser::Jiffies()" << "\n";

  vector<string> cpu_values = CpuUtilization();
  long total{0};
  for (auto value : cpu_values) {
    total += stol(value);
  }
  return total; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function

long LinuxParser::ActiveJiffies(int pid) { 
  //std::cout << "LinuxParser::ActiveJiffies(int pid)" << "\n";

  string pid_s = to_string(pid);
  vector<string> process_values;
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + pid_s + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    //std::cout << "Process value is: " << line << "\n";
    while (linestream >> value){
      process_values.push_back(value);
      //std::cout << "Process value is: " << value << "\n";
    } 
    //std::cout << "Process value 13 is: " << process_values[13] << " " << "Process value 14 is: " << process_values[14] << " " << "Process value 15 is: " << process_values[15] << " " << "Process value 16 is: " << process_values[16] << "\n";
  }
  //std::cout << "Process value 13 is: " << process_values[13] << " " << "Process value 14 is: " << process_values[14] << " " << "Process value 15 is: " << process_values[15] << " " << "Process value 16 is: " << process_values[16] << "\n";
  return (long) (stoi(process_values[13]) + stoi(process_values[14]) + stoi(process_values[15]) + stoi(process_values[16])); 

}


// DONE TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  //std::cout << "LinuxParser::ActiveJiffies()" << "\n";

  return Jiffies() - IdleJiffies(); 
}

// DONE TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  //std::cout << "LinuxParser::IdleJiffies()" << "\n";

  vector<string> cpu_values = CpuUtilization();
  return stol(cpu_values[kIdle_]) + stol(cpu_values[kIOwait_]); 
}

// DONE TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  //std::cout << "LinuxParser::CpuUtilization()" << "\n";

  vector<string> cpu_values;
  string line;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value){
      cpu_values.push_back(value);
      //std::cout << "CPU value is: " << value << "\n";
    } 
  }
  cpu_values.erase(cpu_values.begin());
  return cpu_values; 
}

// DONE TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  //std::cout << "LinuxParser::TotalProcesses()" << "\n";

  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "processes"){
          return stoi(value);
        }      
      } 
    }
  }
  return stoi(value); 
}

// DONE TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  //std::cout << "LinuxParser::RunningProcesses()" << "\n";

  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "procs_running"){
          return stoi(value);
        }      
      } 
    }
  }
  return stoi(value); 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  //std::cout << "LinuxParser::Command(pid)" << "\n";

  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  //std::cout << "LinuxParser::Ram(pid)" << "\n";

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  string key;
  string value;
  string memory;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          memory = to_string(stoi(value) / 1000);
          //std::cout << "value: " << value << " " << "memory: " << memory << "\n";
          return memory;
        } 
      }
    }
  }
  return memory; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function

string LinuxParser::Uid(int pid) { 
  //std::cout << "LinuxParser::Uid(pid)" << "\n";

  string pid_s = to_string(pid);
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + pid_s + kStatusFilename);
  if(filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "Uid:"){
          return value;
        }      
      } 
    }
  }
  
  return value; 
}


// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
 // std::cout << "LinuxParser::User(pid)" << "\n";

  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  string line;
  string user;
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      string id;
      string x;
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> id;
      if (id == uid) {
        return user;
      } 
    }
  }
  
  return user; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  //std::cout << "LinuxParser::UpTime(pid)" << "\n";

  string line;
  string temp;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; i++) {
      linestream >> temp;
      //std::cout << "temp value: " << temp << "\n";
    }
    linestream >> value;
  }
  //std::cout << "Uptime value: " << value << "\n";
  //std::cout << "Uptime value from UPTIME(): " << UpTime() << "\n";
  //std::cout << "_SC_CLK_TCK: " << sysconf(_SC_CLK_TCK) << "\n";
  float uptime = stof(value) / sysconf(_SC_CLK_TCK); 
  //std::cout << "uptime: " << uptime << "\n";
  return (long) uptime; 
}
