#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::stringstream;
using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    long hours,minutes,second;
    hours = seconds /3600;
    minutes = (seconds%3600)/60;
    second =  seconds%60;
    std::stringstream ss; 
    ss<< std::setfill('0') << std::setw(2)<<hours<<":"<< std::setfill('0') << std::setw(2)<<minutes<<":"<< std::setfill('0') << std::setw(2)<<second;
    return ss.str();
}