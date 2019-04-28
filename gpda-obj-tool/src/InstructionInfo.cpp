///////////////////////////////////////////////////////////////
//                                                           //
//   InstructionInfo.cpp                                     //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "InstructionInfo.h"

using namespace std;
using namespace gpdaobjtool;

InstructionInfo::InstructionInfo() { }
InstructionInfo::~InstructionInfo() { }

uint16_t InstructionInfo::operatorType() const
{
    return OPERATOR;
}

vector<string*> InstructionInfo::GetText()
{
    return { &utf8 };
}

void InstructionInfo::ParseRaw(const BVec & data)
{
    uint16_t szText = ((data[7] << 8) | (data[6] << 0)) * 2;
    utf8 = BVec(data.begin() + 10, szText).UTF16LEtoUTF8string();

    blob = BVec(data.begin() + 10 + szText, 16);
}

void InstructionInfo::ParseScript(const vector<string> & script)
{
    utf8 = script[1];
    blob = GetScriptParam(script[2], 6);
}

BVec InstructionInfo::ToRaw() const
{
    BVec ret;

    ret.assign({ 0, 0, 0, 0 });

    ret.push_back((uint8_t)(OPERATOR));
    ret.push_back((uint8_t)(OPERATOR >> 8));

    BVec rawString = IInstruction::UTF8ToRaw(utf8);
    ret.insert(ret.end(), rawString.begin(), rawString.end());

    ret.insert(ret.end(), blob.begin(), blob.end());

    size_t size = PadUp(ret);
    ret.at(0) = (uint8_t)(size);
    ret.at(1) = (uint8_t)(size >> 8);

    return ret;
}

vector<string> InstructionInfo::ToScript() const
{
    vector<string> ret;

    ret.push_back(utf8);
    ret.push_back("{BLOB} " + blob.ToHexString());

    return ret;
}
