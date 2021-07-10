#ifndef FILESYSTEMH
#define FILESYSTEMH
#include <string>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
using namespace std;
namespace IronServer
{
    namespace internal
    {
        string readFile(string dir)
        {
            //create file stream from argument file
            ifstream fileStream(dir);
            //hold file as buffer
            stringstream stringbuffer;
            //pipe into buffer
            stringbuffer << fileStream.rdbuf();
            //convert to std::string and return
            return stringbuffer.str();
        }
        //determine if file at path is a directory
        bool fileIsDirectory(string path)
        {
            //struct is used in stat() call for result
            struct stat stat_return;
            //make stat() call. Make std::string to C string and pass struct as reference
            stat(path.c_str(), &stat_return);
            //returns non-zero if file is directory: https://www.gnu.org/software/libc/manual/html_node/Testing-File-Type.html
            int result = S_ISDIR(stat_return.st_mode);
            return (result != 0);
        }
        bool fileDoesExist(string filePath)
        {
            //struct is used in stat() call for result
            struct stat buffer;
            //stat returns 0 upon successful execution, meaning the file was found and we have permission to access it.
            return (stat(filePath.c_str(), &buffer) == 0);
        }
    }
}
#endif