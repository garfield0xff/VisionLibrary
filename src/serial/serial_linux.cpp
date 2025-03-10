#include <serial/serial.h>
#include <glob.h>

using namespace std;

static vector<string> glob(const vector<string> &patterns);

vector<string> glob(const vector<string> &patterns)
{
    vector<string> paths_found;

    if(patterns.size() == 0) {
        return paths_found;
    }

    glob_t glob_results;

    int glob_retval = glob(patterns[0].c_str(), 0, NULL, &glob_results);

    vector<string>::const_iterator iter = patterns.begin();

    while(++iter != patterns.end()) {
        glob(iter->c_str(), GLOB_APPEND, NULL, &glob_results);
    }

    for(size_t path_index = 0; path_index < glob_results.gl_pathc; path_index++) {
        paths_found.push_back(glob_results.gl_pathv[path_index]);
    }

    globfree(&glob_results);

    return paths_found;
};

namespace vl {
namespace lidar{
namespace serial {

std::vector<PortInfo> list_ports() {
    vector<PortInfo> results;

    vector<string> search_globs;
    search_globs.push_back("/dev/ttyUSB*");
    search_globs.push_back("/dev/tty.*");
    search_globs.push_back("/dev/cu.*");

    vector<string> device_found = glob(search_globs);

    vector<string>::iterator iter = device_found.begin();

    while(iter != device_found.end()) {
        string device = *iter++;

    }

    return results;
}

}
}
}