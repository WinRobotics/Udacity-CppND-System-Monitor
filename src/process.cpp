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

Process::Process(int pid):pid_{pid}
{
    
}

// TODO: Return this process's ID
int Process::Pid() 
{ 
    return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization () const 
{
   //active jiffies pid / (Uptime - Pid Uptime)
   long total_time_ = LinuxParser::ActiveJiffies(pid_);
   total_time_ = total_time_/sysconf(_SC_CLK_TCK);
   long Uptime_ = LinuxParser::UpTime();
   long Uptime_pid_ = LinuxParser::UpTime(pid_);
   long seconds_ = Uptime_ - Uptime_pid_;
   float cpu_uti_ = static_cast<float>((1.0*(total_time_) / seconds_));
   return cpu_uti_;   
}

// TODO: Return the command that generated this process.
string Process::Command() 
{ 
    
    return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() 
{ 
    return LinuxParser::Ram(pid_);
   
}   

// TODO: Return the user (name) that generated this process
string Process::User() 
{ 
    return LinuxParser::User(pid_); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() 
{ 
    return LinuxParser::UpTime(pid_); 
    
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ 
    return (CpuUtilization() > a.CpuUtilization());
}