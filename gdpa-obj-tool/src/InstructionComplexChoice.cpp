///////////////////////////////////////////////////////////////
//                                                           //
//   InstructionComplexChoice.cpp                            //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "InstructionComplexChoice.h"

using namespace std;
using namespace gpdaobjtool;

InstructionComplexChoice::InstructionComplexChoice() { }
InstructionComplexChoice::~InstructionComplexChoice() { }

uint16_t InstructionComplexChoice::operatorType() const
{
    return OPERATOR;
}

vector<string*> InstructionComplexChoice::GetText()
{
    vector<string*> ret;

    for(MainOption & mo : options)
    {
        ret.push_back(&mo.name);

        for (MainOption::SubOption & so : mo.options)
        {
            ret.push_back(&so.name);
        }
    }

    return ret;
}

void InstructionComplexChoice::ParseRaw(const BVec & data)
{
    size_t offset = 6;
    uint8_t szOptions = data[6];

    // Save number of main options as preblob
    blob = BVec(data.begin() + offset, 4);
    offset += 4;

    for (uint8_t i = 0; i < szOptions; i++)
    {
        size_t szOptionName = data[offset] * 2;
        MainOption mo;

        // size of main option name and two 00
        offset += 4;

        mo.name = BVec(data.begin() + offset, szOptionName).UTF16LEtoUTF8string();
        offset += szOptionName;

        // last 8 bytes after the name of the main option
        // (some uknown data and number of suboptions)
        mo.blob = BVec(data.begin() + offset, 8);
        offset += 8;

        // We skipped it in the offset, so we read back -4 bytes
        uint8_t szSubOptions = data[offset - 4];

        for (uint8_t j = 0; j < szSubOptions; j++)
        {
            size_t szSubOptionName = data[offset] * 2;
            MainOption::SubOption so;

            // size of sub option name and two 00
            offset += 4;

            so.name = BVec(data.begin() + offset, szSubOptionName).UTF16LEtoUTF8string();
            offset += szSubOptionName;

            so.blob = BVec(data.begin() + offset, 8);
            offset += 8;

            mo.options.push_back(so);
        }

        options.push_back(mo);
    }
}

void InstructionComplexChoice::ParseScript(const vector<string> & script)
{
    blob = IInstruction::GetScriptParam(script[1], 6);

    uint8_t offset = 2;
    uint8_t szOptions = blob.front();

    for (uint8_t i = 0; i < szOptions; i++)
    {
        MainOption mo;
        mo.name = script[offset];
        offset++;

        mo.blob = GetScriptParam(script[offset], 6);
        offset++;

        uint8_t szSubOptions = mo.blob[4];
        for (uint8_t j = 0; j < szSubOptions; j++)
        {
            MainOption::SubOption so;

            so.name = script[offset];
            offset++;

            so.blob = GetScriptParam(script[offset], 6);
            offset++;

            mo.options.push_back(so);
        }

        options.push_back(mo);
    }
}

BVec InstructionComplexChoice::ToRaw() const
{
    BVec ret;
    ret.assign({ 0, 0, 0, 0 });

    // Operator type, pushed in reverse (so that it is LE)
    ret.push_back((uint8_t)(OPERATOR));
    ret.push_back((uint8_t)(OPERATOR >> 8));

    ret.insert(ret.end(), blob.begin(), blob.end());

    for(const MainOption & mo : options)
    {
        BVec rawString = IInstruction::UTF8ToRaw(mo.name);
        ret.insert(ret.end(), rawString.begin(), rawString.end());

        ret.insert(ret.end(), mo.blob.begin(), mo.blob.end());

        for (const MainOption::SubOption & so : mo.options)
        {
            rawString = IInstruction::UTF8ToRaw(so.name);
            ret.insert(ret.end(), rawString.begin(), rawString.end());

            ret.insert(ret.end(), so.blob.begin(), so.blob.end());
        }
    }

    // Pads instruction and writes size
    size_t size = PadUp(ret);
    ret.at(0) = (uint8_t)(size);
    ret.at(1) = (uint8_t)(size >> 8);

    return ret;
}

vector<string> InstructionComplexChoice::ToScript() const
{
    vector<string> ret;

    ret.push_back("{BLOB} " + blob.ToHexString());

    for(const MainOption & mo : options)
    {
        ret.push_back(mo.name);
        ret.push_back("{BLOB} " + mo.blob.ToHexString());

        for (const MainOption::SubOption & so : mo.options)
        {
            ret.push_back(so.name);
            ret.push_back("{BLOB} " + so.blob.ToHexString());
        }
    }

    return ret;
}
