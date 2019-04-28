///////////////////////////////////////////////////////////////
//                                                           //
//   InstructionBlob.cpp                                     //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "InstructionBlob.h"

using namespace std;
using namespace gpdaobjtool;

InstructionBlob::InstructionBlob() { };
InstructionBlob::~InstructionBlob() { };

uint16_t InstructionBlob::operatorType() const
{
    return _operatorType;
}

void InstructionBlob::ParseRaw(const BVec & data)
{
    blob = data;
    _operatorType = (blob[5] << 8) | (blob[4] << 0);
}

void InstructionBlob::ParseScript(const vector<string> & script)
{
    blob = GetScriptParam(script.at(1), 6);
}

BVec InstructionBlob::ToRaw() const
{
    return blob;
}

vector<string> InstructionBlob::ToScript() const
{
    return { "{BLOB} " + blob.ToHexString()};
}
