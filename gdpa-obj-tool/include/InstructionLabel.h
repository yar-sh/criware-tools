///////////////////////////////////////////////////////////////
//                                                           //
//   InstructionLabel.h                                      //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef GDPA_OBJ_TOOL_INSTRUCTION_LABEL_H
#define GDPA_OBJ_TOOL_INSTRUCTION_LABEL_H

#include "IInstruction.h"

namespace gpdaobjtool
{
    class InstructionLabel : public IInstruction
    {
    public:
        const static uint16_t OPERATOR = 0x0068;

        BVec blob;
        std::string utf8;

        InstructionLabel();
        ~InstructionLabel();

        uint16_t operatorType() const override;

        std::vector<std::string*> GetText() override;

        void ParseRaw(const BVec & data) override;

        void ParseScript(const std::vector<std::string> & script) override;

        BVec ToRaw() const override;

        std::vector<std::string> ToScript() const override;
    };
}

#endif
