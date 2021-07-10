#ifndef HEXDECODERSH
#define HEXDECODERSH
#include <sstream>
#include <string>
using namespace std;
namespace IronServer
{
    namespace internal
    {
        //small hexodecimal utilities used for various purposes in the code
        string toHex(int input)
        {
            stringstream result;
            result << std::hex << input;
            return result.str();
        }
        int hexToInt(string hex)
        {
            int result;
            stringstream stream;
            stream << std::hex << hex;
            stream >> result;
            return result;
        }
    }
}
#endif