///////////////////////////////////////////////////////////////
//                                                           //
//   IInstruction.cpp                                         //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "IInstruction.h"

using namespace std;
using namespace gpdaobjtool;

BVec IInstruction::GetScriptParam(const string & instruction, size_t offset)
{
    string str = instruction.substr(offset, instruction.size() - offset);
    Utils::TrimString(&str);
    return BVec::FromHexString(str);
}

BVec IInstruction::UTF8ToRaw(const string & input)
{
    BVec ret;

    BVec utf16le = BVec::FromUTF8(input);
    utf16le.UTF8toUTF16LE();

    size_t szText = utf16le.size() / 2;

    ret.push_back((uint8_t)(szText));
    ret.push_back((uint8_t)(szText >> 8));

    ret.push_back(0);
    ret.push_back(0);

    ret.insert(ret.end(), utf16le.begin(), utf16le.end());

    return ret;
}

size_t IInstruction::PadUp(BVec & data)
{
    uint8_t padding = (16 - data.size()  % 16);
    if (padding <= 4)
    {
        padding += 16;
    }

    for (; padding > 0; padding--)
        data.push_back(0);

    return data.size();
}
