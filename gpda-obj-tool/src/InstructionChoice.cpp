///////////////////////////////////////////////////////////////
//                                                           //
//   InstructionChoice.cpp                                   //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "InstructionChoice.h"

using namespace std;
using namespace gpdaobjtool;

InstructionChoice::InstructionChoice() { }
InstructionChoice::~InstructionChoice() { }

uint16_t InstructionChoice::operatorType() const
{
    return OPERATOR;
}

vector<string*> InstructionChoice::GetText()
{
    vector<string*> ret;

    for(MainOption & mo : options)
    {
        ret.push_back(&mo.name);
    }

    return ret;
}

void InstructionChoice::ParseRaw(const BVec & data)
{
    size_t offset = 6;
    uint8_t szOptions = data[6];

    for (uint8_t i = 0; i < szOptions; i++)
    {
        MainOption mo;

        mo.blob = BVec(data.begin() + offset, 8);
        offset += 8;

        size_t szOptionName = data[offset] * 2;
        offset += 4;

        mo.name = BVec(data.begin() + offset, szOptionName).UTF16LEtoUTF8string();
        offset += szOptionName;

        options.push_back(mo);
    }
}

void InstructionChoice::ParseScript(const vector<string> & script)
{
    uint8_t offset = 1;
    uint8_t szOptions = 0;

    while(true)
    {
        MainOption mo;
        mo.blob = GetScriptParam(script[offset], 6);

        if (offset == 1)
        {
            szOptions = mo.blob.front();
        }
        offset++;

        mo.name = script[offset];
        offset++;

        options.push_back(mo);

        if (options.size() >= szOptions)
        {
            break;
        }
    }
}

BVec InstructionChoice::ToRaw() const
{
    BVec ret;
    ret.assign({ 0, 0, 0, 0 });

    // Operator type, pushed in reverse (so that it is LE)
    ret.push_back((uint8_t)(OPERATOR));
    ret.push_back((uint8_t)(OPERATOR >> 8));

    for(const MainOption & mo : options)
    {
        ret.insert(ret.end(), mo.blob.begin(), mo.blob.end());

        BVec rawString = IInstruction::UTF8ToRaw(mo.name);
        ret.insert(ret.end(), rawString.begin(), rawString.end());
    }

    // Pads instruction and writes size
    size_t size = PadUp(ret);
    ret.at(0) = (uint8_t)(size);
    ret.at(1) = (uint8_t)(size >> 8);

    return ret;
}

vector<string> InstructionChoice::ToScript() const
{
    vector<string> ret;

    for(const MainOption & mo : options)
    {
        ret.push_back("{BLOB} " + mo.blob.ToHexString());
        ret.push_back(mo.name);
    }

    return ret;
}
