///////////////////////////////////////////////////////////////
//                                                           //
//   InstructionText.cpp                                     //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "InstructionText.h"

using namespace std;
using namespace gpdaobjtool;

InstructionText::InstructionText() { }
InstructionText::~InstructionText() { }

uint16_t InstructionText::operatorType() const
{
    return OPERATOR;
}

vector<string*> InstructionText::GetText()
{
    return { &utf8 };
}

void InstructionText::ParseRaw(const BVec & data)
{
    blob = BVec(data.begin() + 6, 4);

    uint16_t szText = ((data[11] << 8) | (data[10] << 0)) * 2;
    utf8 = BVec(data.begin() + 14, szText).UTF16LEtoUTF8string();
}

void InstructionText::ParseScript(const vector<string> & script)
{
    blob = GetScriptParam(script[1], 6);
    utf8 = script[2];
}

BVec InstructionText::ToRaw() const
{
    BVec ret;

    ret.assign({ 0, 0, 0, 0 });

    ret.push_back((uint8_t)(OPERATOR));
    ret.push_back((uint8_t)(OPERATOR >> 8));

    ret.insert(ret.end(), blob.begin(), blob.end());

    BVec rawString = IInstruction::UTF8ToRaw(utf8);
    ret.insert(ret.end(), rawString.begin(), rawString.end());

    size_t size = PadUp(ret);
    ret.at(0) = (uint8_t)(size);
    ret.at(1) = (uint8_t)(size >> 8);

    return ret;
}

vector<string> InstructionText::ToScript() const
{
    vector<string> ret;

    ret.push_back("{BLOB} " + blob.ToHexString());
    ret.push_back(utf8);

    return ret;
}
