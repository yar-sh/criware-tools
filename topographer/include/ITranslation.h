///////////////////////////////////////////////////////////////
//                                                           //
//   ITranslation.h                                          //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef TOPOGRAPHER_ITRANSLATION_H
#define TOPOGRAPHER_ITRANSLATION_H

#include "Types.h"
#include "Utils.h"
#include "StringList.h"

namespace topographer
{
    // A translation class is responsible for loading and
    //   parsing the translation files. Basically it extracts
    //   the English strings and stores them in the same
    //   key indexed way as Japanese strings in original BINs.
    class ITranslation
    {
    public:
        ITranslation() { };
        virtual ~ITranslation() { };

        // Loads a translation file
        //   filename, string, the ifle name of the translation file
        // Returns: bool, true if loaded successfully, false otherwise
        virtual bool Load(const std::string & filepath) = 0;

        // Parsed and sanitized list of key and inner strings
        StringList sl;
    };
}

#endif
