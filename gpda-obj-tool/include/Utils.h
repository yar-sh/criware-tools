///////////////////////////////////////////////////////////////
//                                                           //
//   Utils.h                                                 //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef GDPA_OBJ_TOOL_UTILS_H
#define GDPA_OBJ_TOOL_UTILS_H

#include "Types.h"
#include <sstream>
#include <algorithm>
#include <iomanip>

namespace gpdaobjtool
{
    // Some utilities that are used throughout the topographer
    class Utils
    {
    public:
        // Converts an integer value into a formatted hex string
        //   value, uint32_t, the value to convert
        //   w, uint8_t, the number to align bytes to (passed to setw())
        // Returns: string, the passed value in a hex string form
        static std::string ValueToHexStr(uint32_t value, uint8_t w = 8);

        // Converts a hex string of bytes to byte array
        //   str, string, the string to convert to byte array
        //   out, &BArr, output byte array
       // static void HexStrToBytes(const std::string & str, BArr & out);

        // Checks to see if a sequence of bytes matches with
        //   other bytes at a given position
        //   bytes, BArr, the bytes in which to check the match
        //   sequence, BArr, the bytes of which to check the match
        //   offset, size_t, the position in bytes at which to
        //     check for match
        // Returns: bool, true if the matching `sequence` is found
        //   at the `offset` in `bytes`, false otherwise
     //   static bool MatchByteArray(const BArr & bytes, const BArr & sequence, size_t offset);

        // Trims a string from any extra spaces
        //   str, &string, string to trim
        static void TrimString(std::string * str);

        // Replaces occurances of a substring in a string with other string
        //   str, &string, string to perform replacement in
        //   from, string, the substring of which to replace occurances
        //   to, string, what to replace `from` occurances with
        static void ReplaceString(std::string * str, const std::string * from, const std::string * to);

    private:
        Utils();
        ~Utils();
    };
}

#endif
