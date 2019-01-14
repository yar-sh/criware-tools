///////////////////////////////////////////////////////////////
//                                                           //
//   EncodingTable.h                                         //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef TOPOGRAPHER_ENCODING_TABLE_H
#define TOPOGRAPHER_ENCODING_TABLE_H

#include "Types.h"
#include "Utils.h"
#include "GameString.h"
#include <fstream>
#include <iostream>

namespace topographer
{
    // Responsible for loading and accessing all the encoding rules
    //   from the encoding table file. Also provides functions to work
    //   with original game strings and their processing
    class EncodingTable
    {
    public:
        EncodingTable();
        ~EncodingTable();

        // Loads encoding table
        //   filepath, string, the file name of the encoding table file
        // Returns: bool, true if loaded successfully, false otherwise
        bool Load(const std::string & filename);

        // Find the last control sequence in the byte array
        //   input, BArr, the string in which to look for control sequence
        //   out, &BArr, the output of which control sequence was found
        // Returns: size_t, -1 if no sequence was found, otherwise a
        //   position at which sequence was found
        size_t FindLastControlSequence(const BArr & input, BArr & out) const;

        // Replaces all the characters in the byte array by using encoding rules
        //   in, BArrr, the string in which to replaces characters
        //   out, &GameString, the output string with replaces characters
        void ApplyEncoding(const BArr & in, GameString & out) const;

    private:
        // All encoding rules
        std::vector<EncodingRule> _rules;

        // Parses a rule string in a table
        //   str, string, a rule string to parse (eg: "1337=<NICE>")
        //   rule, *EncodingRule, an output encoding rule
        // Returns: bool, true if parsing was successful and the rule is valid,
        //   false otherwise
        bool _ParseRuleString(const std::string & str, EncodingRule * rule);
    };
}

#endif
