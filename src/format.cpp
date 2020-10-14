#include <string>
#include <ctime>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {

  int hours{}, minutes{};
  string hoursStr, minutesStr, secondsStr;

  hours = seconds / 3600;
  seconds %= 3600;
  minutes = seconds / 60;
  seconds%= 60;

  hoursStr = std::to_string(hours);
  minutesStr = std::to_string(minutes);
  secondsStr = std::to_string(seconds);

  if (hours < 10)  hoursStr = "0" + hoursStr;
  if (minutes < 10) minutesStr = "0" + minutesStr;
  if (seconds < 10) secondsStr = "0" + secondsStr;

  return hoursStr + ":" + minutesStr + ":" +secondsStr;
}