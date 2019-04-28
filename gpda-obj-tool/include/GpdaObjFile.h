///////////////////////////////////////////////////////////////
//                                                           //
//   GpdaObjFile.h                                           //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef GDPA_OBJ_TOOL_FILE_H
#define GDPA_OBJ_TOOL_FILE_H

#include "Types.h"
#include "Utils.h"
#include "IInstruction.h"
#include "InstructionText.h"
#include "InstructionBlob.h"
#include "InstructionChoice.h"
#include "InstructionComplexChoice.h"
#include "InstructionInfo.h"
#include "InstructionLabel.h"
#include "InstructionDict.h"
#include "InstructionStrArr.h"
#include <fstream>

namespace gpdaobjtool
{
    class GpdaObjFile
    {
    public:
        std::vector<IInstruction*> instructions;
        std::vector<std::string*> text;

        GpdaObjFile();
        ~GpdaObjFile();

        bool LoadObj(const std::string & filepath);
        bool LoadTxt(const std::string & filepath);

        bool WriteTxt(const std::string & filepath) const;
        bool WriteObj(const std::string & filepath) const;

    private:
        BVec _header;
    };
}

#endif
