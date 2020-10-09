#include <string>
#include <ctime>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  auto t = (time_t)seconds;
  struct tm * timeInfo;
  char buffer[80];

  time(&t);
  timeInfo = localtime(&t);

  strftime(buffer, sizeof(buffer),"%H:%M:%S", timeInfo);

  return string(buffer);
}