///////////////////////////////////////////////////////////////
//                                                           //
//   EncodingTable.cpp                                       //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "EncodingTable.h"

using namespace std;
using namespace topographer;

EncodingTable::EncodingTable() { }

EncodingTable::~EncodingTable() { }

// Loads encoding table
//   filepath, string, the file name of the encoding table file
// Returns: bool, true if loaded successfully, false otherwise
bool EncodingTable::Load(const string & filepath)
{
    cout << "Encoding table file: " << filepath << endl;

    ifstream file(filepath);
    if(file.bad())
    {
        return false;
    }

    while (true)
    {
        string str;
        getline(file, str);

        if (file.eof())
        {
            break;
        }

        EncodingRule rule;
        if(_ParseRuleString(str, &rule))
        {
            _rules.push_back(rule);
        }
    }

    file.close();

    // The ' ' rule gets trimmed, so we add it back in
    _rules.push_back({
        rtRaw,
        { 0x20 },
        { ' ' }
    });

    cout << "Total encoding rules: " << _rules.size() << endl;

    return true;
}

// Find the last control sequence in the byte array
//   input, BArr, the string in which to look for control sequence
//   out, &BArr, the output of which control sequence was found
// Returns: size_t, -1 if no sequence was found, otherwise a
//   position at which sequence was found
size_t EncodingTable::FindLastControlSequence(const BArr & input, BArr & out) const
{
    out.clear();

    for (size_t i = 0; i < _rules.size(); i++)
    {
        const EncodingRule & r = _rules[i];

        // Only check the rules that start with '<'
        // The encoding table is formatted in a way such that
        //   all controlling sequences of rules go in the beginning, so
        //   once we reach the one that is not a control sequence, we can
        //   ignore everything that follows
        if (r.replacement.front() != 0x3C)
        {
            return -1;
        }

        // If the rule corresponds to the proper byte sequence, then
        //   it is the one we are looking for
        if (Utils::MatchByteArray(input, r.original, input.size() - r.original.size()))
        {
            out = r.replacement;
            return input.size() - r.original.size();
        }
    }

    return -1;
}

// Replaces all the characters in the byte array by using encoding rules
//   in, BArrr, the string in which to replaces characters
//   out, &GameString, the output string with replaces characters
void EncodingTable::ApplyEncoding(const BArr & in, GameString & out) const
{
    out.Clear();

    bool leftNameBracket = false;

    for (size_t i = 0; i < in.size(); i++)
    {
        for(size_t j = 0; j < _rules.size(); j++)
        {
            EncodingRule r = _rules[j];

            // If this is not the matching rule then move onto the next one
            if (!Utils::MatchByteArray(in, r.original, i))
            {
                // Should never happen if you have a correct encoding table
                if (j + 1 == _rules.size())
                {
                    cout << "[WARN] This string contains unreplaceable byte:\n\t";

                    for(size_t z = 0; z < in.size(); z++)
                    {
                        cout << (char)in[z] << "  ";
                    }

                    cout << "\n\t";

                    for(size_t z = 0; z < in.size(); z++)
                    {
                        cout << Utils::ValueToHexStr(in[z], 2) << " ";
                    }

                    cout << endl << "[WARN] at position: " << (unsigned int)i << endl;

                    out.AddNewTokenWithData(ttRaw, BArr({ in[i] }));

                    break;
                }

                continue;
            }

            // Insert control code
            if (r.type == rtControlCode)
            {
                out.AddNewTokenWithData(ttControlCode, r.replacement);
            }
            else // rrtRaw
            {

                if (!leftNameBracket && r.original == LEFT_NAME_BRACKET)
                {
                    leftNameBracket = true;

                    out.AddNewToken(ttName);
                }

                if (!leftNameBracket && !out.GetTokens().empty()
                    && out.GetTokens().back().type != ttRaw)
                {
                    out.AddNewToken(ttRaw);
                }

                out.AppendToLastToken(r.replacement);

                // If we are copying a character name and get to the RIGHT_NAME_BRACKET,
                //   then end the ttName token.
                if (leftNameBracket && r.original == RIGHT_NAME_BRACKET)
                {
                    leftNameBracket = false;
                }
            }

            // Skip over bytes of the sequence in the original string, so that
            //   there are no lookups of already matched bytes
            i += r.original.size() - 1;

            break;
        }
    }

    //out.encodingApplied = true;
}

// Parses a rule string in a table
//   str, string, a rule string to parse (eg: "1337=<NICE>")
//   rule, *EncodingRule, an output encoding rule
// Returns: bool, true if parsing was successful and the rule is valid,
//   false otherwise
bool EncodingTable::_ParseRuleString(const std::string & str, EncodingRule * rule)
{
    string s = str;

    Utils::TrimString(s);

    if (s.empty())
    {
        return false;
    }

    size_t pos = s.find('=');
    if (pos == string::npos || pos == s.size() - 1)
    {
        return false;
    }

    // Divides the rule into two parts
    string original(s, 0, pos); // "0x13 0x37" from example
    string replacement(s, pos + 1, s.size() - pos); // "<NICE>" from example

    // Parse hex byte string into byte array
    BArr bytes;
    for (size_t i = 0; i < original.size(); i += 2)
    {
        uint8_t b = (uint8_t)strtol(original.substr(i, 2).c_str(), NULL, 16);
        bytes.push_back(b);
    }

    // Save the rule data in the structure
    rule->original = bytes;
    rule->replacement = BArr(replacement.begin(), replacement.end());

    // If this is a control code, then set a proper type.
    //   (It's rrtRawby default)
    if (replacement.front() == '<' && replacement.back() == '>')
    {
        rule->type = rtControlCode;
    }

    return true;
}
