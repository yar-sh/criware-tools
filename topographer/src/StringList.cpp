///////////////////////////////////////////////////////////////
//                                                           //
//   StringList.cpp                                          //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "StringList.h"

using namespace std;
using namespace topographer;

StringList::StringList() { }
StringList::~StringList() { }

// Sets a string directly in the list of game strings
//   at, size_t, the position where to put game string
//   gs, GameString, what string to put
// Returns: bool, true if setting string is successful, false otherwise
bool StringList::SetDirect(const size_t at, const GameString & gs)
{
    if (at >= _strings.size())
    {
        return false;
    }

    _strings[at] = gs;

    return true;
}

// Adds an inner string to the last key index, as well as to the
// direct string list. If there is no prior key index available,
// then the string is only added directly
//   str, GameString, string to add as an inner string to the last
//     key string
void StringList::AddInnerString(const GameString & str)
{
    _strings.push_back(str);

    if (!_keys.empty())
    {
        KeyIndex & key = _keys.back();
        key.numInnerStrings++;
    }
}

// Adds a new key string and also stores it in the direct string
// list.
//   str, GameString, string to add as a new key string
void StringList::AddKeyString(const GameString & str)
{
    _strings.push_back(str);

    _keys.push_back({
        _strings.size() - 1,
        0
    });
}
// Returns a constant reference to the vector of ALL (key AND
//   inner) GameStrings.
const std::vector<GameString> & StringList::GetDirectStrings() const
{
    return _strings;
}

// Returns a constant reference to the vector of KEY GameStrings.
const std::vector<KeyIndex> & StringList::GetKeyStrings() const
{
    return _keys;
}
