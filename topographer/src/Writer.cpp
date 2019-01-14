///////////////////////////////////////////////////////////////
//                                                           //
//   Writer.cpp                                              //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/scriware-tools          //
//                                                           //
///////////////////////////////////////////////////////////////

#include "Writer.h"

using namespace std;
using namespace topographer;
namespace fs = std::experimental::filesystem;

// Creates a writer for all the output files.
//   outputPath, string, path to the output directory
//   bf, *BinFile, pointer to the parsed BinFile
//   tr, *ITranslation, pointer to the parsed translation. Default: nullptr
Writer::Writer(const std::string & outputPath, const BinFile * bf, const ITranslation * tr)
{
    _bf = bf;
    _tr = tr;

    _outputPath = outputPath;
	_stem = fs::path(_bf->_filepath).stem().generic_string();
}

Writer::~Writer() { }


// Dumps Section 1 of the BIN file
// Returns: bool, true if written successfully, false otherwise
bool Writer::DumpSection1() const
{
    string filepath = _outputPath + "/" + _stem + "-section1.BIN";

    cout << "\t...Dumping Section 1: " << filepath << endl;

    ofstream file(filepath, ios::binary | ios::trunc);
    if (file.bad())
    {
        return false;
    }

    file.write((char*)&_bf->_section1[0], _bf->_section1.size());

    file.close();

    return true;
}

// Writes Atlas script
// Returns: bool, true if written successfully, false otherwise
bool Writer::WriteAtlas() const
{
    string filepath = _outputPath + "/" + _stem + ".ATLAS";

    cout << "\t...Writing Atlas script: " << filepath << endl;

    ofstream file(filepath, ios::trunc);
    if (file.bad())
    {
        return false;
    }

    // Header of the Atlas file
    file << "#VAR(Table, TABLE)" << endl
         << "#ADDTBL(\"../" << ENCODING_TABLE_FILE << "\", Table)" << endl
         << "#ACTIVETBL(Table)" << endl
         << "#VAR(PTR, CUSTOMPOINTER)" << endl
         << "#CREATEPTR(PTR, \"LINEAR\", 0, " << (int)_bf->_psz << ")" << endl
         << "#JMP($" << Utils::ValueToHexStr(_bf->_splitAddress) << ")" << endl;

    // Write all pointers and strings
    size_t ptSz = _bf->pointerTable.size();
    for(size_t i = 0; i < ptSz; i++)
    {
        Pointer const & p = _bf->pointerTable[i];
        GameString const & gs = _bf->sl.GetDirectStrings()[i];

        file << endl << "#WRITE(PTR, $" << Utils::ValueToHexStr(p.address) << ")" << endl;

        for(auto const & token : gs.GetTokens())
        {
            file << string(token.data.begin(), token.data.end()) << endl;
        }
    }

    file.close();

    return true;
}

// Writes extracted translations strings (useful dor debugging)
// Returns: bool, true if written successfully, false otherwise
bool Writer::WriteTranslationStrings() const
{
    if (_tr == nullptr)
    {
        return true;
    }

    string filepath = _outputPath + "/" + _stem + "-strings.TXT";

    cout << "\t...Writing translation strings: " << filepath << endl;

    ofstream file(filepath, ios::trunc);
    if (file.bad())
    {
        return false;
    }

    for(auto const & str : _tr->sl.GetDirectStrings())
    {
        for(auto const & token : str.GetTokens())
        {
            file << string(token.data.begin(), token.data.end()) << endl;
        }
    }

    file.close();

    return true;
}

// Writes bat file for compiling Atlas scripts using Atlas.exe
// Returns: bool, true if written successfully, false otherwise
bool Writer::WriteBatFile() const
{
    string filepath = _outputPath + "/" + _stem + ".bat";

    cout << "\t...Writing bat file: " << filepath << endl;

    ofstream file(filepath, ios::trunc);
    if (file.bad())
    {
        return false;
    }

    // For copying a backup Section 1 BIN
    file << "xcopy /y "
         << _stem + "-section1.BIN "
         << _stem + ".BIN*" << endl;

    // For using Atlas.exe to compile scripts
    file << "\"..\\Atlas.exe\" "
         << _stem + ".BIN "
         << _stem + ".ATLAS" << endl;

    file.close();

    return true;
}
