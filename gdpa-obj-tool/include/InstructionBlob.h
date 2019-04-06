///////////////////////////////////////////////////////////////
//                                                           //
//   InstructionBlob.h                                       //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef GDPA_OBJ_TOOL_INSTRUCTION_BLOB_H
#define GDPA_OBJ_TOOL_INSTRUCTION_BLOB_H

#include "IInstruction.h"

namespace gpdaobjtool
{
    class InstructionBlob : public IInstruction
    {
    public:
        BVec blob;

        InstructionBlob();
        ~InstructionBlob();

        uint16_t operatorType() const override;

        void ParseRaw(const BVec & data) override;

        void ParseScript(const std::vector<std::string> & script) override;

        BVec ToRaw() const override;

        std::vector<std::string> ToScript() const override;

    private:
        uint16_t _operatorType = 0;
    };
}

#endif
