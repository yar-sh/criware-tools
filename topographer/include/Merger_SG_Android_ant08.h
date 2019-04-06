///////////////////////////////////////////////////////////////
//                                                           //
//   Merger_SG_Android_ant08.h                               //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef TOPOGRAPHER_MERGER_SG_ANDROID_ANT08_H
#define TOPOGRAPHER_MERGER_SG_ANDROID_ANT08_H

#include "IMerger.h"
#include <iostream>

namespace topographer
{
    // A translation class is responsible for loading and
    //   parsing the translation files. Basically it extracts
    //   the English strings and stores them in the same
    //   key indexed way as Japanese strings in original BINs.
    class Merger_SG_Android_ant08 : public IMerger
    {
    public:
        ~Merger_SG_Android_ant08();

        // Merges two StringLists into one
        //   into, &StringList, original and also output StringList (JAP)
        //   from, &StringList, list with translation (ENG)
        void Merge(StringList & into, const StringList & from) override;

    private:
        GameString _AssembleGameString(const GameString & inJap, const GameString & inEng) const;
    };
}

#endif
