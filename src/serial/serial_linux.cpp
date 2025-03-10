// #include <serial/serial.h>
// #include <glob.h>

// using namespace std;

// static vector<string> glob(const vector<string> &patterns);

// vector<string> glob(const vector<string> &patterns)
// {
//     vector<string> paths_found;

//     if(patterns.size() == 0) {
//         return paths_found;
//     }

//     glob_t glob_results;

//     int glob_retval = glob(patterns[0].c_str(), 0, NULL, &glob_results);

//     vector<string>::const_iterator iter = patterns.begin();

//     while(++iter != patterns.end()) {
//         glob(iter->c_str(), GLOB_APPEND, NULL, &glob_results);
//     }

//     for(size_t path_index = 0; path_index < glob_results.gl_pathc; path_index++) {
//         paths_found.push_back(glob_results.gl_pathv[path_index]);
//     }

//     globfree(&glob_results);

//     return paths_found;
// };

// namespace vl {
// namespace lidar{
// namespace serial {

// }
// }
// }