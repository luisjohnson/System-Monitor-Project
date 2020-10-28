#include "processor.h"
#include "linux_parser.h"


// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  long totalDelta{0}, idleDelta{0}, jiffies, idleJiffies;
  float cpuPercentage{0.0};

  jiffies = LinuxParser::Jiffies();
  idleJiffies = LinuxParser::IdleJiffies();
  totalDelta = jiffies - prevTotalJiffies_;
  idleDelta = idleJiffies - prevIdleJiffies_;
  cpuPercentage = (float (totalDelta) - float(idleDelta))/float(totalDelta);
  prevTotalJiffies_ = jiffies;
  prevIdleJiffies_ = idleJiffies;
  return cpuPercentage;
}

Processor::Processor() : prevIdleJiffies_(0),
                         prevTotalJiffies_(0) {}