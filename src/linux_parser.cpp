#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  string line;
  string key;
  string mem_total = "MemTotal:";
  string mem_avail = "MemAvailable:";
  float memtotal{0};
  float memavail{0};
  float mem_uti{0};
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {

    while(getline(stream,line))
      {
          std::istringstream linestream(line);
          linestream>>key;
          if(key == mem_total)
          {
            linestream>>memtotal;
          }
           if(key == mem_avail)
          {
            linestream>>memavail;
          }
      }
  }

  mem_uti = (memtotal - memavail) / memtotal;
  return mem_uti; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string line;
  long idle_time{0};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>idle_time;
    }
  stream.close();
  return idle_time; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
  string line;
  string cpu;
  long jiffies{0};
  long value{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) 
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>cpu;
    while(linestream>>value)
    {
      jiffies+=value;
    }
  }
  stream.close();
  return jiffies; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
  string line;
  int iterations{0};
  long active{0};
  long total_active{0};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open())
  {
    std::getline(stream,line);
    std::istringstream linestream(line);
    //Iteration to start of utime,stime,cutime,cstime 
    for(int i =0; i <13; ++i)
    {
      linestream>>iterations; 
    }
    for(int y=0; y<4;++y)
    {
      linestream>>active;
      total_active+= active;
    }

  }

  return total_active; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{ 
  string line;
  string cpu;
  long active_jiffies{0};
  long value{0};
  std::vector <long> result;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) 
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>cpu;
    while(linestream>>value)
    {
      result.push_back(value);
    }
  }

  stream.close();
  active_jiffies = result.at(CPUStates::kUser_) + result.at(CPUStates::kNice_) + result.at(CPUStates::kSystem_) + result.at(CPUStates::kIRQ_)
  + result.at(CPUStates::kSoftIRQ_) + result.at(CPUStates::kSteal_);
  return active_jiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  string line;
  string cpu;
  long idle_jiffies{0};
  long value{0};
  std::vector <long> result;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) 
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>cpu;
    while(linestream>>value)
    {
      result.push_back(value);
    }
  }

  stream.close();
  //idle_+iowait_
  idle_jiffies = result.at(CPUStates::kIdle_) + result.at(CPUStates::kIOwait_);
  return idle_jiffies; 
  
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  //not needed function
  string line,key,utime,stime,cutime,cstime,startime,totaltime;
  std::vector<string> cpu_uti;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
      std::getline(stream,line);
  
      std::istringstream linestream(line);
      for(int i =0; i <12 ; ++i)
      {
        linestream>>key;
      }
      linestream>>utime;
      cpu_uti.push_back(utime);
      linestream>>stime;
      cpu_uti.push_back(stime);
      linestream>>cutime;
      cpu_uti.push_back(cutime);
      linestream>>cstime;
      cpu_uti.push_back(cstime);
      //#18 ->22
      for(int i =0; i <4 ; ++i)
      {
        linestream>>key;
      }
      linestream>>startime;
      cpu_uti.push_back(startime);
      //totaltime = utime+stime+cutime+cstime+startime;
      //
  }
  return cpu_uti; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  string line;
  int total_proc{0};
  std::string key{};
  std::string processes ="processes";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) 
  {
      while(getline(stream,line))
      {
          std::istringstream linestream(line);
          linestream>>key>>total_proc;
          if(key == processes)
          {
            return total_proc; 
          }
      }
  }
  stream.close();
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  string line;
  int running_proc{0};
  std::string key{};
  std::string proc_running ="procs_running";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) 
  {
      while(getline(stream,line))
      {
          std::istringstream linestream(line);
          linestream>>key>>running_proc;
          if(key == proc_running)
          {
            return running_proc; 
          }
      }
  }
  stream.close();
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 

  string line,key,command;
  std::ifstream stream(kProcDirectory  +std::to_string(pid)+ kCmdlineFilename);
  if(stream.is_open())
  {
      std::getline(stream,line);
      std::istringstream linestream(line);
      linestream>>command;

    
  }
  return command; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 

  string line;
  long mem;
  string ram{};
  string key;;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
    std::istringstream linestream(line);
    linestream>>key;
      if(key =="VmSize:")
      {
        linestream>>mem;
        mem /= 1000;
        ram =to_string(mem);
      }
    }
  }
     return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  
  string line;
  int uid;
  string Uid{};
  string key;;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
    std::istringstream linestream(line);
    linestream>>key;
      if(key =="Uid:")
      {
        linestream>>uid;
        Uid = to_string(uid); 
      }
    }
  }
     return Uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  string uid = LinuxParser::Uid(pid);
  string line;
  string User{};
  string key{};
  string value{};
  std::ifstream stream(kPasswordPath);
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
    std::replace(line.begin(),line.end(),':',' ');
    std::replace(line.begin(),line.end(),'x',' ');  
    std::istringstream linestream(line);
    linestream>>value>>key;
      if(key == uid)
      {
          User = value;
      }
    }
  }
     return User;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  string line,key;
  long int value{0};
  long int Uptime{0};
  std::ifstream stream(kProcDirectory  +std::to_string(pid)+ kStatFilename);
  if(stream.is_open())
  {
      std::getline(stream,line);
  
      std::istringstream linestream(line);
      //(22) starttime %llu
      for(int i =0; i <21 ; ++i)
      {
        linestream>>key;
      }
      linestream>>value;
      Uptime =value / sysconf(_SC_CLK_TCK);
      return Uptime; 
    
  }
  return Uptime; 

}
