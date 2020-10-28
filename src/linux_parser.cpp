#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::istringstream;
using std::map;
namespace fs = std::filesystem;
using std::regex;
using std::regex_match;


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
      std::istringstream lineStream(line);
      while (lineStream >> key >> value) {
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
  string os;
  string kernel;
  string version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> os >> version >> kernel;
  }
  return kernel;
}

// DONE BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  std::vector<int> pids;
  fs::path path{kProcDirectory};
  for(const fs::directory_entry &entry: fs::directory_iterator{path}) {
    if (entry.is_directory() ) {
      std::string dirname{entry.path().filename()};
      if (regex_match(dirname, regex("[1-9]\\d*|0\\d+"))) {
        pids.push_back(std::stoi(dirname));
      }
    }
  }
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  float value;
  map<string, float> memoryMap;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream lineStream(line);
      while (lineStream >> key >> value) {
        key.pop_back();
        memoryMap[key] = value;
      }
    }
  }
    float memoryUsed = (memoryMap["MemTotal"] - memoryMap["MemAvailable"] )  /
                     memoryMap["MemTotal"];
    return memoryUsed;
  }

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long uptime{};
  long idleTime{};
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open()) {
    getline(filestream, line);
    std::istringstream lineStream(line);
    lineStream >> uptime >> idleTime;
  }
  return uptime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  float total{0};
  vector<float> cpuStats = CpuUtilization();
  for (float stat: cpuStats) {
    total += stat;
  }
  return long(total);
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string value;
  long jiffies{0};
  vector<string> fields{};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream lineStream(line);
    while (lineStream >> value) {
      fields.push_back(value);
    }
    jiffies = std::stol(fields[ProcessInfoStats::kUTime_]) +
              std::stol(fields[ProcessInfoStats::kSTime_]) +
              std::stol(fields[ProcessInfoStats::kCUTime_]) +
              std::stol(fields[ProcessInfoStats::kCSTime_]);
  }

  return jiffies;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<float> cpuStats = CpuUtilization();
  float total;
  total = cpuStats[CPUStates::kUser_] + cpuStats[CPUStates::kNice_] +
          cpuStats[CPUStates::kSystem_] + cpuStats[CPUStates::kIRQ_] +
          cpuStats[CPUStates::kSoftIRQ_] + cpuStats[CPUStates::kSteal_];
  return long(total);
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<float> cpuStats = CpuUtilization();
  float total;
  total = cpuStats[CPUStates::kIdle_] + cpuStats[CPUStates::kIOwait_];
  return long(total);
}

// DONE: Read and return CPU utilization
vector<float> LinuxParser::CpuUtilization() {
  string line;
  string cpu;
  float value;
  vector<float> cpuStats;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream lineStream(line);
    lineStream >> cpu;
    while (lineStream >> value) {
      cpuStats.push_back(value);
    }
  }
  return cpuStats;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int value{0}, total{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream lineStream(line);
      while (lineStream >> key >> value){
        if (key == "processes") {
          total = value;
        }
      }
    }
  }
  return total;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int value{0}, total{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream lineStream(line);
      while (lineStream >> key >> value){
        if (key == "procs_running") {
          total = value;
        }
      }
    }
  }
  return total; }

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::string command;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    getline(filestream, command);
  }
  return command;
}

// DONE: Read and return the memory used by a process
long LinuxParser::Ram(int pid) {
  std::string line;
  std::string key;
  long value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()){
    while (getline(filestream, line)) {
      std::istringstream lineStream(line);
      while (lineStream >> key >> value) {
        key.pop_back();
        //VmSize replaced by VmData per reviewer suggestion
        if (key == "VmData") {
          return value / 1000;
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while (getline(filestream, line)) {
      istringstream lineStream(line);
      while (lineStream >> key >> value) {
        key.pop_back();
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return string();
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string value;
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()){
    while (getline(filestream, line)) {
      istringstream lineStream(line);
      vector<std::string> fields{};
      while(getline(lineStream, value, ':')) {
        fields.push_back(value);
      }
      if (fields[2] == uid) {
        return fields[0];
      }
    }
  }
  return string();
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  string clockTicks;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream lineStream(line);
    for(int i = 0; i < 22; i++) {
      lineStream >> clockTicks;
    }
  }
  return UpTime() - std::stol(clockTicks) / sysconf(_SC_CLK_TCK);
}

long int LinuxParser::StartTime(int pid) {
  string line;
  string value;
  long startTime{0};
  vector<string> fields{};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream lineStream(line);
    while (lineStream >> value) {
      fields.push_back(value);
    }
    startTime = std::stol(fields[ProcessInfoStats::kStartTime_]) /
                sysconf(_SC_CLK_TCK);
  }
  return startTime;
}
