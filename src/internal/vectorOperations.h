#ifndef VECTOROPERATIONSH
#define VECTOROPERATIONSH
#include <string>
#include <vector>
using namespace std;
namespace IronServer
{
    namespace internal
    {
        //split string to vector by substring
        vector<string> splitAll(string rawArg, string what)
        {
            string raw = rawArg;
            size_t pos = 0;
            vector<string> split;
            //run while what exists in string; set pos to the position of what
            while ((pos = raw.find(what)) != string::npos)
            {
                split.push_back(raw.substr(0, pos));
                raw.erase(0, pos + what.size());
            }
            split.push_back(raw);
            return split;
        }
        //split path to vector.
        //note: relative paths do not work
        vector<string> pathToVector(string path)
        {
            if (path.size() < 2)
            {
                return vector<string>{path};
            }
            string newString = path;
            bool startsWithSlash = false;
            //first, remove innecessary slashes
            if (string{newString.at(0)} == "/")
            {
                newString = newString.substr(1);
                startsWithSlash = true;
            }
            if (string{newString.at(newString.size() - 1)} == "/")
            {
                newString = newString.substr(0, newString.size() - 1);
            }
            //now split by / & return
            vector<string> result = splitAll(newString, "/");
            //if begins with /, add / to beginning of vector
            if (startsWithSlash)
            {
                result.insert(result.begin(), "/");
            }
            return result;
        }
        //join a vector into  a string
        string joinVector(vector<string> vectorToJoin, string what)
        {
            string result;
            for (int i = 0; i < vectorToJoin.size(); i++)
            {
                result += vectorToJoin[i];
                //if isn't in last element
                if (i != vectorToJoin.size() - 1)
                {
                    result += what;
                }
            }
            return result;
        }
    }
}
#endif