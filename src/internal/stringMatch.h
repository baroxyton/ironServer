#ifndef STRINGMATCHH
#define STRINGMATCHH
#include <string>
#include <regex>
using namespace std;
namespace IronServer
{
    namespace internal
    {
        //check if string includes substring
        bool doesInclude(string str, string what)
        {
            return str.find(what) != string::npos;
        }
        //check if string matches regex
        bool doesMatchRegex(string toMatch, string reg)
        {
            regex regRegex(reg);
            bool result = regex_match(toMatch, regRegex);
            return result;
        }
    }
}
#endif