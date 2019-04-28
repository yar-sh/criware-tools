///////////////////////////////////////////////////////////////
//                                                           //
//   InstructionStrArr.cpp                                   //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "InstructionStrArr.h"

using namespace std;
using namespace gpdaobjtool;

InstructionStrArr::InstructionStrArr() { }
InstructionStrArr::~InstructionStrArr() { }

uint16_t InstructionStrArr::operatorType() const
{
    return OPERATOR;
}

vector<string*> InstructionStrArr::GetText()
{
    vector<string*> ret;

    for(Entry & e : entries)
    {
        ret.push_back(&e.str);
    }

    return ret;
}

void InstructionStrArr::ParseRaw(const BVec & data)
{
    size_t offset = 6;
    uint8_t szOptions = 255, i = 0;

    while (true)
    {
        Entry e;

        e.blob = BVec(data.begin() + offset, 8);
        offset += 8;

        size_t szOptionName = data[offset] * 2;
        offset += 4;

        e.str = BVec(data.begin() + offset, szOptionName).UTF16LEtoUTF8string();
        offset += szOptionName;

        if (i == 0)
        {
            szOptions = e.blob.front();
        }

        entries.push_back(e);
        i++;

        if (i == szOptions)
        {
            break;
        }
    }
}

void InstructionStrArr::ParseScript(const vector<string> & script)
{
    uint8_t offset = 1;
    uint8_t szOptions = 255, i = 0;

    while (true)
    {
        Entry e;

        e.blob = IInstruction::GetScriptParam(script[offset], 6);
        offset++;

        e.str = script[offset];
        offset++;

        if (i == 0)
        {
            szOptions = e.blob.front();
        }

        entries.push_back(e);
        i++;

        if (i == szOptions)
        {
            break;
        }
    }
}

BVec InstructionStrArr::ToRaw() const
{
    BVec ret;
    ret.assign({ 0, 0, 0, 0 });

    // Operator type, pushed in reverse (so that it is LE)
    ret.push_back((uint8_t)(OPERATOR));
    ret.push_back((uint8_t)(OPERATOR >> 8));

    for(const Entry & e : entries)
    {
        ret.insert(ret.end(), e.blob.begin(), e.blob.end());

        BVec rawString = IInstruction::UTF8ToRaw(e.str);
        ret.insert(ret.end(), rawString.begin(), rawString.end());
    }

    // Pads instruction and writes size
    size_t size = PadUp(ret);
    ret.at(0) = (uint8_t)(size);
    ret.at(1) = (uint8_t)(size >> 8);

    return ret;
}

vector<string> InstructionStrArr::ToScript() const
{
    vector<string> ret;
    for(const Entry & e : entries)
    {
        ret.push_back("{BLOB} " + e.blob.ToHexString());
        ret.push_back(e.str);
    }

    return ret;
}
