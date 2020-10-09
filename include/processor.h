#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();  // TODO: See src/processor.cpp

  // DONE: Declare any necessary private members
 private:
  long prevActiveJiffies_, prevIdleJiffies_, prevTotalJiffies_;

};

#endif