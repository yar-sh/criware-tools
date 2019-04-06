///////////////////////////////////////////////////////////////
//                                                           //
//   TranslationFile.h                                       //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef GDPA_OBJ_TOOL_TRANSLATION_FILE_H
#define GDPA_OBJ_TOOL_TRANSLATION_FILE_H

#include "Types.h"
#include "Utils.h"
#include <fstream>

namespace gpdaobjtool
{
    class TranslationFile
    {
    public:
        std::vector<std::string> text;

        TranslationFile() {};
        ~TranslationFile() {};

        bool Load(const std::string & filepath)
        {
            std::ifstream file(filepath, std::ios::in);
            if(file.bad())
            {
                return false;
            }

            while(true)
            {
                if (file.eof() || file.tellg() == -1)
                {
                    break;
                }

                std::string line;
                getline(file, line);

                Utils::TrimString(&line);

                // Skip empty lines and comments
                if (line.empty())
                {
                    continue;
                }

                text.push_back(line);
            }

            file.close();
            return true;
        }
    };
}
#endif
