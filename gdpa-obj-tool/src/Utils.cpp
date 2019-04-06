///////////////////////////////////////////////////////////////
//                                                           //
//   Utils.cpp                                               //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "Utils.h"

using namespace std;
using namespace gpdaobjtool;

// Converts an integer value into a formatted hex string
//   value, uint32_t, the value to convert
//   w, uint8_t, the number to align bytes to (passed to setw())
// Returns: string, the passed value in a hex string form
string Utils::ValueToHexStr(uint32_t value, uint8_t w)
{
    stringstream r;

    r << uppercase << hex << setfill('0') << setw(w)
       << (unsigned long)value
       << resetiosflags(r.flags());

    return r.str();
}

// Converts a hex string of bytes to byte array
//   str, string, the string to convert to byte array
//   out, &BArr, output byte array
/*
void Utils::HexStrToBytes(const std::string & str, BArr & out)
{
    out.clear();

    for (size_t i = 0; i < str.size(); i += 2)
    {
        uint8_t byte = (uint8_t)strtol(str.substr(i, 2).c_str(), NULL, 16);
        out.push_back(byte);
    }
}

// Checks to see if a sequence of bytes matches with
//   other bytes at a given position
//   bytes, BArr, the bytes to check for match in
//   sequence, BArr, the bytes to check for match of
//   offset, size_t, the position in bytes to check for match at
// Returns: bool, true if the matching sequence of bytes is found
//   at the offset in bytes, false otherwise
bool Utils::MatchByteArray(const BArr & bytes, const BArr & sequence, size_t offset)
{
    if (bytes.empty() || offset + sequence.size() > bytes.size())
    {
        return false;
    }

    for(size_t i = 0; i < sequence.size(); i++)
    {
        if (bytes[offset + i] != sequence[i])
        {
            return false;
        }
    }

    return true;
}

*/

// TODO: docs

// Trims a string from any extra spaces
//   str, &string, string to trim
void Utils::TrimString(string * str)
{
    str->erase(str->begin(), find_if(str->begin(), str->end(), [](int c) {
        return !isspace(c);
    }));
    str->erase(find_if(str->rbegin(), str->rend(), [](int c) {
        return !isspace(c);
    }).base(), str->end());
}

// Replaces occurances of a substring in a string with other string
//   str, &string, string to perform replacement in
//   from, string, the substring of which to replace occurances
//   to, string, what to replace `from` occurances with
void Utils::ReplaceString(string * str, const string * from, const string * to)
{
    size_t pos = 0;
    while((pos = str->find(*from, pos)) != string::npos)
    {
        str->replace(pos, from->size(), *to);
        pos += to->size();
    }
}
