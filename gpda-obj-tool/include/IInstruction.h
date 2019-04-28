///////////////////////////////////////////////////////////////
//                                                           //
//   IInstruction.h                                          //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef GDPA_OBJ_TOOL_IINSTRUCTION_H
#define GDPA_OBJ_TOOL_IINSTRUCTION_H

#include "Types.h"
#include "Utils.h"

namespace gpdaobjtool
{
    class IInstruction
    {
    public:
        virtual ~IInstruction() {};

        virtual uint16_t operatorType() const = 0;

        virtual std::vector<std::string*> GetText() { return {}; }

        virtual void ParseRaw(const BVec & data) = 0;

        virtual void ParseScript(const std::vector<std::string> & script) = 0;

        virtual BVec ToRaw() const = 0;

        virtual std::vector<std::string> ToScript() const = 0;

        static BVec GetScriptParam(const std::string & instruction, size_t offset);

        static BVec UTF8ToRaw(const std::string & input);

        static size_t PadUp(BVec & data);
    };
}

#endif
