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
#include <iostream>
namespace gpdaobjtool
{
    class TranslationFile
    {
    public:
        TranslationFile() {};
        ~TranslationFile() {};
        bool Load(const std::string * filepath)
        {
            _Reset();
            _filepath = *filepath;

            std::ifstream file(_filepath, std::ios::in);
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

                /*
                // Convert utf8 string into BVec UTF16LE
                BVec lineUtf8;
                lineUtf8.assign(line.begin(), line.end());
                BVec lineUtf16;
                lineUtf16.FromUTF16LE(lineUtf8.UTF8toUTF16LE());
                */

                // Push the utf16le data
                //_lines.push_back(lineUtf16);
            }

            file.close();
            return true;
        }

        inline const std::vector<BVec>* lines() const { return &_lines; }

    private:
        void _Reset()
        {
            _totalLines = 0;
            _selectedLine = 0;
            _lines.clear();
            _filepath.clear();
        }

        std::vector<BVec> _lines;
        uint16_t _totalLines = 0;
        uint16_t _selectedLine = 0;
        std::string _filepath;
    };
}
#endif
