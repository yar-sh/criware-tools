///////////////////////////////////////////////////////////////
//                                                           //
//   IMerger.h                                               //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef TOPOGRAPHER_IMERGER_H
#define TOPOGRAPHER_IMERGER_H

#include "Types.h"
#include "StringList.h"

namespace topographer
{
    // A merger class is responsible for attempting to merge
    // two StringLists, where one is with the strings from the
    // original game, and the other one contains translated
    // strings
    class IMerger
    {
    public:
        IMerger() { };
        virtual ~IMerger() { };

        // Merges two StringLists into one
        //   into, &StringList, original and also output StringList (JAP)
        //   from, &StringList, list with translation (ENG)
        virtual void Merge(StringList & into, const StringList & from) = 0;
    };
}

#endif
