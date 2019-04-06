///////////////////////////////////////////////////////////////
//                                                           //
//   InstructionDict.cpp                                     //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "InstructionDict.h"

using namespace std;
using namespace gpdaobjtool;

InstructionDict::InstructionDict() { }
InstructionDict::~InstructionDict() { }

uint16_t InstructionDict::operatorType() const
{
    return OPERATOR;
}

vector<string*> InstructionDict::GetText()
{
    vector<string*> ret;

    for(Entry & e : entries)
    {
        ret.push_back(&e.str1);
        ret.push_back(&e.str2);
    }

    return ret;
}

void InstructionDict::ParseRaw(const BVec & data)
{
    size_t offset = 6;
    uint8_t szOptions = data[offset];

    blob = BVec(data.begin() + offset, 4);
    offset += 4;

    for (uint8_t i = 0; i < szOptions; i++)
    {
        Entry e;

        e.blob1 = BVec(data.begin() + offset, 4);
        offset += 4;

        size_t szOptionName = data[offset] * 2;
        offset += 4;

        e.str1 = BVec(data.begin() + offset, szOptionName).UTF16LEtoUTF8string();
        offset += szOptionName;

        e.blob2 = BVec(data.begin() + offset, 4);
        offset += 4;
        if (e.blob2.front() == 1)
        {
            szOptionName = data[offset] * 2;
            offset += 4;

            e.str2 = BVec(data.begin() + offset, szOptionName).UTF16LEtoUTF8string();
            offset += szOptionName;
        }

        entries.push_back(e);
    }
}

void InstructionDict::ParseScript(const vector<string> & script)
{
    blob = IInstruction::GetScriptParam(script[1], 6);

    uint8_t offset = 2;
    uint8_t szOptions = blob.front();

    for (uint8_t i = 0; i < szOptions; i++)
    {
        Entry e;

        e.blob1 = IInstruction::GetScriptParam(script[offset], 6);
        offset++;

        e.str1 = script[offset];
        offset++;

        e.blob2 = IInstruction::GetScriptParam(script[offset], 6);
        offset++;

        if (e.blob2.front() == 1)
        {
            e.str2 = script[offset];
            offset++;
        }

        entries.push_back(e);
    }
}

BVec InstructionDict::ToRaw() const
{
    BVec ret;
    ret.assign({ 0, 0, 0, 0 });

    // Operator type, pushed in reverse (so that it is LE)
    ret.push_back((uint8_t)(OPERATOR));
    ret.push_back((uint8_t)(OPERATOR >> 8));

    ret.insert(ret.end(), blob.begin(), blob.end());

    for(const Entry & e : entries)
    {
        ret.insert(ret.end(), e.blob1.begin(), e.blob1.end());

        BVec rawString = IInstruction::UTF8ToRaw(e.str1);
        ret.insert(ret.end(), rawString.begin(), rawString.end());

        ret.insert(ret.end(), e.blob2.begin(), e.blob2.end());

        if (!e.blob2.empty() && e.blob2.front() == 1)
        {
            rawString = IInstruction::UTF8ToRaw(e.str2);
            ret.insert(ret.end(), rawString.begin(), rawString.end());
        }
    }

    // Pads instruction and writes size
    size_t size = PadUp(ret);
    ret.at(0) = (uint8_t)(size);
    ret.at(1) = (uint8_t)(size >> 8);

    return ret;
}

vector<string> InstructionDict::ToScript() const
{
    vector<string> ret;

    ret.push_back("{BLOB} " + blob.ToHexString());

    for(const Entry & e : entries)
    {
        ret.push_back("{BLOB} " + e.blob1.ToHexString());
        ret.push_back(e.str1);

        ret.push_back("{BLOB} " + e.blob2.ToHexString());
        if (!e.blob2.empty() && e.blob2.front() == 1)
        {
            ret.push_back(e.str2);
        }
    }

    return ret;
}
