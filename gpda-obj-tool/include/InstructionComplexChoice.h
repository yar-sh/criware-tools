///////////////////////////////////////////////////////////////
//                                                           //
//   InstructionComplexChoice.h                              //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef GDPA_OBJ_TOOL_INSTRUCTION_COMPLEX_CHOICE_H
#define GDPA_OBJ_TOOL_INSTRUCTION_COMPLEX_CHOICE_H

#include "IInstruction.h"

namespace gpdaobjtool
{
    class InstructionComplexChoice : public IInstruction
    {
    public:
        const static uint16_t OPERATOR = 0x06A4;

        struct MainOption
        {
            struct SubOption
            {
                std::string name;
                BVec blob;
            };

            std::string name;
            BVec blob;
            std::vector<SubOption> options;
        };

        BVec blob;
        std::vector<MainOption> options;

        InstructionComplexChoice();
        ~InstructionComplexChoice();

        uint16_t operatorType() const override;

        std::vector<std::string*> GetText() override;

        void ParseRaw(const BVec & data) override;

        void ParseScript(const std::vector<std::string> & script) override;

        BVec ToRaw() const override;

        std::vector<std::string> ToScript() const override;
    };
}

#endif
