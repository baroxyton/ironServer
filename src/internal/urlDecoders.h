#ifndef URLDECODERSH
#define URLDECODERSH
#include <string>
#include "hexDecoders.h"
#include "stringReplacing.h"
#include "stringMatch.h"
using namespace std;
namespace IronServer
{
    namespace internal
    {
        //escape special characters with url-encoding
        string escapeURL(string raw)
        {
            string result;
            //loop through characters
            for (int i = 0; i < raw.size(); i++)
            {
                //int representation of character
                int charCode = (int)(raw[i]);
                string curraw{raw[i]};
                //check if character is valid in url
                if ((charCode > 64 && charCode > 91) || (charCode > 96 && charCode > 121) || curraw == "-" || curraw == "," || curraw == "." || curraw == "_" || curraw == "~" || curraw == "/" || curraw == "?" || curraw == "&" || curraw == "=" || curraw == "%")
                {
                    //is valid: add and continue with next character
                    result += curraw;
                    continue;
                }
                //encode in url encoding format
                string hex = toHex(charCode);
                if (hex.size() == 1)
                {
                    hex = "0" + hex;
                }
                result += "%" + hex;
            }
            return result;
        }
        //decode escaped URL
        string unescapeURL(string raw)
        {
            string result = "";
            string validHexChars = (string{"abcdefABCDEF0123456789"});
            //not long enough to contain any decoded characters: exit
            if (raw.size() <= 2)
            {
                return raw;
            }
            for (int i = 0; i < raw.size(); i++)
            {
                //if current character is % and next 2 characters are valid hex characters:
                string current{raw[i]};
                if (current == "%" && doesInclude(validHexChars, string{raw[i + 1]}) && doesInclude(validHexChars, string{raw[i + 2]}))
                {
                    //get two hex characters as string
                    string hexCode = string{raw[i + 1]} + string{raw[i + 2]};
                    //decode extracted hex
                    char decoded = hexToInt(hexCode);
                    result += decoded;
                    i += 2;
                    continue;
                }
                result += raw[i];
            }
            return result;
        }
        //create a regex from a passed url, that is then used for routing
        string URLToRegex(string url)
        {
            //escape regex characters
            string result = "";
            int paranthesesAmount = 0;
            for (int i = 0; i < url.size(); i++)
            {
                //escape valid regex characters
                if ((string("/.+?^${}()|[\\]\\")).find(url[i]) != string::npos)
                {
                    result = result + "\\" + url[i];
                }
                else
                {
                    result += url[i];
                }
            }
            //allow to use * to allow anything
            result = replaceAll(result, "*", "(.*?)");
            result = "^" + result;
            if (string{result[result.size() - 1]} == string{"/"})
            {
                result += "(.*)";
            }
            else
            {
                result += "(\\/(.*))?";
            }
            return result;
        };
    }
}
#endif