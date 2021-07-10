#ifndef STRINGREPLACINGH
#define STRINGREPLACINGH
#include <string>
using namespace std;
namespace IronServer
{
    namespace internal
    {
        //replace substring
        string stringReplace(string input, string what, string replace)
        {
            if (input.find(input.find(what) != string::npos))
            {
                input = input.replace(input.find(what), what.size(), replace);
            }
            return input;
        }
        //replace all substrings
        string replaceAll(string input, string what, string replace)
        {
            string str = input;
            while (str.find(what) != string::npos)
            {
                str = str.replace(str.find(what), what.size(), replace);
            }
            return str;
        }
    }
}
#endif