///////////////////////////////////////////////////////////////
//                                                           //
//   GpdaObjFile.cpp                                         //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "GpdaObjFile.h"

using namespace std;
using namespace gpdaobjtool;

GpdaObjFile::GpdaObjFile() {}
GpdaObjFile::~GpdaObjFile() {}

bool GpdaObjFile::LoadObj(const string * filepath)
{
    _Reset();
    _filepath = *filepath;

    ifstream file(_filepath, ios::in | ios::binary);
    if(file.bad())
    {
        return false;
    }

    uint8_t buf[GPDA_OBJ_INSTRUCTION_MAX_SZ];

    // Read file header
    file.read((char*)buf, GPDA_OBJ_HEADER_SZ);
    _header.assign(&buf[0], &buf[0] + GPDA_OBJ_HEADER_SZ);

    while(true)
    {
        // Read instruction size
        auto instructionStart = file.tellg();
        file.read((char*)&buf[0], 2);

        uint16_t szInstruction = (buf[1] << 8) | (buf[0] << 0);

        #if 1
            if (szInstruction > GPDA_OBJ_INSTRUCTION_MAX_SZ)
            {
                printf("AHTUNG!!!! Greater than %d bytes\n", GPDA_OBJ_INSTRUCTION_MAX_SZ);
                return false;
            }
        #endif

        // Read the entire instruction as a blob
        file.seekg(instructionStart);
        file.read((char*)&buf[0], szInstruction);

        // This looks like it is out of order, but actually, this code works only if
        //   this check is here.
        if (file.eof() || file.tellg() == -1)
        {
            break;
        }

        // Convert blob to BVec, parse it, and save
        BVec data;
        data.assign(&buf[0], &buf[0] + szInstruction);

        uint16_t operatorType = (buf[5] << 8) | (buf[4] << 0);
        IInstruction * i;

        if (operatorType == InstructionText::OPERATOR)
        {
            i = new InstructionText();
        }
        else if (operatorType == InstructionChoice::OPERATOR)
        {
            i = new InstructionChoice();
        }
        else if (operatorType == InstructionComplexChoice::OPERATOR)
        {
            i = new InstructionComplexChoice();
        }
        else if (operatorType == InstructionInfo::OPERATOR)
        {
            i = new InstructionInfo();
        }
        else if (operatorType == InstructionLabel::OPERATOR)
        {
            i = new InstructionLabel();
        }
        else if (operatorType == InstructionDict::OPERATOR)
        {
            i = new InstructionDict();
        }
        else if (operatorType == InstructionStrArr::OPERATOR)
        {
            i = new InstructionStrArr();
        }
        else
        {
            i = new InstructionBlob();
        }

        i->ParseRaw(data);
        _textLines += i->GetText().size();

        _instructions.push_back(i);
    }

    file.close();
    return true;
}

bool GpdaObjFile::LoadTxt(const string * filepath)
{
    _Reset();
    _filepath = *filepath;

    ifstream file(_filepath, ios::in | ios::binary);
    if(file.bad())
    {
        return false;
    }

    bool inInstruction = false;
    vector<string> script;
    while(true)
    {
        if (file.eof() || file.tellg() == -1)
        {
            break;
        }

        string line;
        getline(file, line);

        Utils::TrimString(&line);

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        // End of instruction block -> save the buffered instruction
        if (inInstruction && line.find("{INSTRUCTION_END}") == 0)
        {
            uint16_t operatorType = IInstruction::GetScriptParam(script[0], 19).ToUint16_t();
            IInstruction * i;

            if (operatorType == InstructionText::OPERATOR)
            {
                i = new InstructionText();
            }
            else if (operatorType == InstructionChoice::OPERATOR)
            {
                i = new InstructionChoice();
            }
            else if (operatorType == InstructionComplexChoice::OPERATOR)
            {
                i = new InstructionComplexChoice();
            }
            else if (operatorType == InstructionInfo::OPERATOR)
            {
                i = new InstructionInfo();
            }
            else if (operatorType == InstructionLabel::OPERATOR)
            {
                i = new InstructionLabel();
            }
            else if (operatorType == InstructionDict::OPERATOR)
            {
                i = new InstructionDict();
            }
            else if (operatorType == InstructionStrArr::OPERATOR)
            {
                i = new InstructionStrArr();
            }
            else
            {
                i = new InstructionBlob();
            }

            i->ParseScript(script);
            _textLines += i->GetText().size();

            _instructions.push_back(i);

            inInstruction = false;
            script.clear();
            continue;
        }

        // Begin instruction block
        if (!inInstruction && line.find("{INSTRUCTION_BEGIN}") == 0)
        {
            inInstruction = true;
        }

        // Save instruction text if we are in the instruction block
        if (inInstruction)
        {
            script.push_back(line);
            continue;
        }

        // Save file header
        if (_header.empty() && line.find("{HEADER}", 0) == 0)
        {
            _header = IInstruction::GetScriptParam(line, 8);
            continue;
        }
    }

    if (_header.empty())
    {
        printf("\tNo {HEADER} in the text script\n");
        return false;
    }

    file.close();
    return true;
}

bool GpdaObjFile::WriteObj(const string * filepath) const
{
    ofstream file(*filepath, ios::out | ios::binary);
    if(file.bad())
    {
        return false;
    }

    for (uint8_t c : _header)
    {
        file.put(c);
    }

    for(const IInstruction * i : _instructions)
    {
        BVec data = i->ToRaw();
        for (uint8_t c : data)
        {
            file.put(c);
        }
    }

    file.close();
    return true;
}

bool GpdaObjFile::WriteTxt(const string * filepath) const
{
    ofstream file(*filepath, ios::out);
    if(file.bad())
    {
        return false;
    }

    file << "{HEADER} " << _header.ToHexString() << endl << endl;

    for(IInstruction * i : _instructions)
    {
        file << "{INSTRUCTION_BEGIN} " << Utils::ValueToHexStr(i->operatorType(), 4) << endl;

        uint16_t ot = i->operatorType();
        if (ot == InstructionText::OPERATOR ||
            ot == InstructionChoice::OPERATOR ||
            ot == InstructionComplexChoice::OPERATOR ||
            ot == InstructionInfo::OPERATOR ||
            ot == InstructionLabel::OPERATOR ||
            ot == InstructionDict::OPERATOR ||
            ot == InstructionStrArr::OPERATOR ||
            ot == 0x02BD || ot == 0x03EA ||
            ot == 0x00CC || ot == 0x0708 ||
            ot == 0x076C || ot == 0x06A8 ||
            ot == 0x06AA || ot == 0x06A9 ||
            ot == 0x00C8 || ot == 0x006A ||
            ot == 0x012C || ot == 0x0258 ||
            ot == 0x00CA || ot == 0x02C0 ||
            ot == 0x0259 || ot == 0x00C9 ||
            ot == 0x00D4 || ot == 0x012D ||
            ot == 0x02BF || ot == 0x044C ||
            ot == 0x0578 || ot == 0x02BC ||
            ot == 0x04B0 || ot == 0x025B ||
            ot == 0x00D5 || ot == 0x01FF ||
            ot == 0x01F5 || ot == 0x01F7 ||
            ot == 0x01F8 || ot == 0x01F4 ||
            ot == 0x00CD || ot == 0x01F4 ||
            ot == 0x00D6 || ot == 0x025A ||
            ot == 0x025C || ot == 0x0191 ||
            ot == 0x0192 || ot == 0x025C ||
            ot == 0x025D || ot == 0x01f6 ||
            ot == 0x02be || ot == 0x0000 ||
            ot == 0x06A5 || ot == 0x06A6 ||
            ot == 0x06A7 || ot == 0x0001 ||
            ot == 0x0514 || ot == 0x03E8 ||
            ot == 0x0640 || ot == 0x0198 ||
            ot == 0x0190 || ot == 0x0195 ||
            ot == 0x0898 || ot == 0x0197 ||
            ot == 0x01F9 || ot == 0x00CB ||
            ot == 0x02C8 || ot == 0x00CE ||
            ot == 0x02C2 || ot == 0x02C1)
        {
        }
        else
        {
            printf("\tUnknown operation type 0x%04X\n", ot);
        }

        auto script = i->ToScript();

        for (const string & s: script)
        {
            file << s << endl;
        }

        file << "{INSTRUCTION_END}" << endl << endl;
    }

    file.close();
    return true;
}

void GpdaObjFile::_Reset()
{
    _filepath.clear();
    _header.clear();
    _instructions.clear();
    _textLines = 0;
}
