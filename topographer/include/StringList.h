///////////////////////////////////////////////////////////////
//                                                           //
//   StringList.h                                            //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef TOPOGRAPHER_STRING_LIST_H
#define TOPOGRAPHER_STRING_LIST_H

#include "Types.h"
#include "GameString.h"

namespace topographer
{
    // An expansion over a normal list of strings. Provides the idea
    //   of an indexed list, where every dialog string (that is, a string
    //   that starts with '【') is a key, and every string between it and
    //   the next key is an inner string. This comes to a great use
    //   when trying to automatically merge English strings into Japanese ones,
    //   since key strings act as "blocks" on which the translation
    //   matching algorithm can operate on.
    class StringList
    {
    public:
        StringList();
        ~StringList();

        // Sets a string directly in the list of game strings
        //   at, size_t, the position where to put game string
        //   gs, GameString, what string to put
        // Returns: bool, true if setting string is successful, false otherwise
        bool SetDirect(const size_t at, const GameString & gs);

        // Adds an inner string to the last key index, as well as to the
        // direct string list. If there is no prior key index available,
        // then the string is only added directly
        //   str, GameString, string to add as an inner string to the last
        //     key string
        void AddInnerString(const GameString & str);

        // Adds a new key string and also stores it in the direct string
        // list.
        //   str, GameString, string to add as a new key string
        void AddKeyString(const GameString & str);

        // Returns a constant reference to the vector of ALL (key AND
        //   inner) GameStrings.
        const std::vector<GameString> & GetDirectStrings() const;

        // Returns a constant reference to the vector of KEY GameStrings.
        const std::vector<KeyIndex> & GetKeyStrings() const;

    private:
        // Vector of all game strings (includes key strings aka character lines)
        std::vector<GameString> _strings;

        // Vector of key indexes
        std::vector<KeyIndex> _keys;
    };
}

#endif
