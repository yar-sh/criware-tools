///////////////////////////////////////////////////////////////
//                                                           //
//   Translation_SG_Android_ant08.h                          //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef TOPOGRAPHER_TRANSLATION_SG_ANDROID_ANT08_H
#define TOPOGRAPHER_TRANSLATION_SG_ANDROID_ANT08_H

#include "ITranslation.h"
#include <iostream>
#include <fstream>

namespace topographer
{
    // A translation class is responsible for loading and
    //   parsing the translation files. Basically it extracts
    //   the English strings and stores them in the same
    //   key indexed way as Japanese strings in original BINs.
    class Translation_SG_Android_ant08 : public ITranslation
    {
    public:
        ~Translation_SG_Android_ant08();

        // Loads a translation file
        //   filename, string, the ifle name of the translation file
        // Returns: bool, true if loaded successfully, false otherwise
        bool Load(const std::string & filepath) override;

    private:
        std::string _filepath;

        // Sanitizes the string and properly indexes it as either a
        //   key string or inner string
        void _ParseString(const std::string & str);
    };
}

#endif
