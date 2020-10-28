#include "processor.h"
#include "linux_parser.h"


// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  long totalDelta{0};
  long idleDelta{0};
  long jiffies{0};
  long idleJiffies{0};
  float cpuPercentage{0.0};

  jiffies = LinuxParser::Jiffies();
  idleJiffies = LinuxParser::IdleJiffies();
  totalDelta = jiffies - prevTotalJiffies_;
  idleDelta = idleJiffies - prevIdleJiffies_;
  cpuPercentage = ( static_cast<float>(totalDelta) -
                   static_cast<float>(idleDelta) /
                       static_cast<float>(totalDelta));
  prevTotalJiffies_ = jiffies;
  prevIdleJiffies_ = idleJiffies;
  return cpuPercentage;
}

Processor::Processor() : prevIdleJiffies_(0),
                         prevTotalJiffies_(0) {}