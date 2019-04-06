///////////////////////////////////////////////////////////////
//                                                           //
//   InstructionDict.h                                       //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef GDPA_OBJ_TOOL_INSTRUCTION_DICT_H
#define GDPA_OBJ_TOOL_INSTRUCTION_DICT_H

#include "IInstruction.h"

namespace gpdaobjtool
{
    class InstructionDict : public IInstruction
    {
    public:
        const static uint16_t OPERATOR = 0x006B;

        struct Entry
        {
            BVec blob1;
            std::string str1;
            BVec blob2;
            std::string str2;
        };

        BVec blob;
        std::vector<Entry> entries;

        InstructionDict();
        ~InstructionDict();

        uint16_t operatorType() const override;

        std::vector<std::string*> GetText() override;

        void ParseRaw(const BVec & data) override;

        void ParseScript(const std::vector<std::string> & script) override;

        BVec ToRaw() const override;

        std::vector<std::string> ToScript() const override;
    };
}

#endif
