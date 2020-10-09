#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
  auto totalTime = (float)LinuxParser::ActiveJiffies(pid_) /
      (float)sysconf(_SC_CLK_TCK);
  auto seconds = LinuxParser::UpTime() - startTime_;
  auto cpuUSage = totalTime/(float)seconds;
  return cpuUSage;
}

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() {
  long ram = LinuxParser::Ram(pid_);
  return to_string(ram);
}

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return LinuxParser::Ram(pid_) > LinuxParser::Ram(a.pid_);
}

Process::Process(int pid) : pid_(pid),
                            user_(LinuxParser::User(pid)),
                            command_(LinuxParser::Command(pid)),
                            uid_(LinuxParser::Uid(pid)),
                            startTime_(LinuxParser::StartTime(pid)){}
