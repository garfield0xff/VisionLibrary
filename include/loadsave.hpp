#ifndef _LOADSAVE_H_
#define _LOADSAVE_H_

#include <lidar.hpp>
#include <window_io.hpp>

namespace vl {

namespace lidar{
    
bool 
loadLidar(const String port, int flags); 

} //namespace lidar
} //namespace vl

#endif /*_LOADSAVE_H_*/