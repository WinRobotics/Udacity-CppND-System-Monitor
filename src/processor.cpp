#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 

   //CPU Percentage = (total jiffies - idle jiffies) / total jiffies
   float uti = (static_cast<float> (LinuxParser::Jiffies()) - static_cast<float>(LinuxParser::IdleJiffies()))/static_cast<float>(LinuxParser::Jiffies());
   return uti;
}