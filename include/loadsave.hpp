#ifndef _LOADSAVE_H_
#define _LOADSAVE_H_

#include <lidar.hpp>
#include <window_io.hpp>
#include <window_fb.hpp>


namespace vl {

namespace lidar{
    
void showPointCloud(std::shared_ptr<vl::lidar::BaseLidarController> controller);

bool 
loadLidar(const String port, int flags); 

} //namespace lidar
} //namespace vl

#endif /*_LOADSAVE_H_*/