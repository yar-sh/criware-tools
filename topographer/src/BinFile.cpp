///////////////////////////////////////////////////////////////
//                                                           //
//   BinFile.cpp                                             //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

//
// CANONICALLY LEAVING THIS FILE UNDOCUMENTED (tm)
//

#include "BinFile.h"

using namespace std;
using namespace topographer;

BinFile::BinFile(EncodingTable * table)
{
    _encTable = table;
}

BinFile::~BinFile() { }

bool BinFile::Load(const string & filepath)
{
    _filepath = filepath;

    cout << "\t...Loading BIN file: " << filepath << endl;

    ifstream file(filepath);
    if(file.bad())
    {
        return false;
    }


    while(true)
    {
        uint8_t bytes[2];

        file.read((char *)bytes, 2);

        if (file.eof())
        {
            break;
        }

        if (bytes[0] == 0x0D &&
            bytes[1] == 0x02)
        {
            _splitAddress = file.tellg();
        }

        _section1.insert(_section1.end(), bytes, bytes + 2);
    }

    file.close();

    cout << "\tTotal size " << _section1.size() << " bytes" << endl;

    return true;
}

void BinFile::Process()
{
    _SplitSections();
    _MapPointers();
    _ParseStrings();
}

void BinFile::_SplitSections()
{
    cout << "\t...Splitting into Sections" << endl;

    _section2 = BArr(
        _section1.begin() + _splitAddress,
        _section1.end()
    );

    _section1.resize(_splitAddress);

    cout << "\tSection 1 size " << _section1.size() << " bytes" << endl;
    cout << "\tSection 2 size " << _section2.size() << " bytes" << endl;
    cout << "\tSplit Address 0x" << Utils::ValueToHexStr(_splitAddress) << endl;
}

void BinFile::_MapPointers()
{
    cout << "\t...Mapping pointers" << endl;

    uint32_t offset = 0;

    for(size_t i = 0; i < _section2.size(); i++)
    {
        // Go to the end of string, and skip the first 0x00 in a string
        if (_section2[i] != 0x00 || offset == i)
        {
            continue;
        }

        // Claculate absolute address to the beginning of the string
        uint32_t stringAddress = _splitAddress + offset;

        // Adjust the number of bits per pointer
        if (_psz == psz16b && stringAddress >= 0xFFFF)
        {
            _psz = psz32b;
        }

        // Look for the address that contains current
        //   absolute in the reversed order
        uint32_t pointerAddress = _FindPointerToAddress(stringAddress);

        // At this point no matter if address is found or not
        //   I want this to be set to the beginning of the
        //   next string
        offset = i + 1;

        if (pointerAddress == 0)
        {
            continue;
        }

        // If Pointer Address exists, then save it
        pointerTable.push_back({
            pointerAddress,
            stringAddress,
        });
    }

    cout << "\tTotal " << dec << pointerTable.size() << " pointers" << endl;
    cout << "\tString Address range from 0x"
         << Utils::ValueToHexStr(pointerTable.front().stringAddress)
         << " to 0x"
         << Utils::ValueToHexStr(pointerTable.back().stringAddress)
         << endl;
}

void BinFile::_ParseStrings()
{
    cout << "\t...Parsing and applying encoding to strings" << endl;

    // Go through every pointer, extract the string and divide it into the
    //   the main part and the ending part.
    size_t ptSize = pointerTable.size();
    for (size_t i = 0; i < ptSize; i++)
    {
        Pointer & p = pointerTable[i];

        // Get the endpoints of the entire string
        BArr::iterator startPoint = _section2.begin() + p.stringAddress - _splitAddress;
        BArr::iterator endPoint;

        if (i + 1 < ptSize)
        {
            endPoint = _section2.begin() + pointerTable[i + 1].stringAddress - _splitAddress;
        }
        else
        {
            endPoint = _section2.end();
        }

        // The entire unsplitted string
        BArr str(startPoint, endPoint);

        // Prepare the string for storing it in a StringList
        GameString gs(ttRaw);

        _encTable->ApplyEncoding(str, gs);

        // If it is a spoken string, then add it as a key string,
        //   otherwise add it as an inner string
        if (!gs.GetTokens().empty() && gs.GetTokens().front().type == ttName)
        {
            sl.AddKeyString(gs);
        }
        else
        {
            sl.AddInnerString(gs);
        }
    }

    cout << "\tTotal strings: " << sl.GetDirectStrings().size() << endl;
    cout << "\tTotal key indexes: " << sl.GetKeyStrings().size() << endl;
}

uint32_t BinFile::_FindPointerToAddress(uint32_t address) const
{
    // Store bytes in reverse
    uint8_t b1 = ((address >> 24) & 0xFF);
    uint8_t b2 = ((address >> 16) & 0xFF);
    uint8_t b3 = ((address >> 8) & 0xFF);
    uint8_t b4 =  ((address >> 0) & 0xFF);

    size_t i = (pointerTable.size() > 0) ? (pointerTable.back().address) : 0;
    for(; i < _section1.size(); i++)
    {
        // Skip when on the odd address
        if (i % 2 == 1)
        {
            continue;
        }

        // Instantly quit if we are at the end of the Section 1
        if ((_psz == psz16b && i + 2 >= _section1.size()) ||
            (_psz == psz32b && i + 4 >= _section1.size()))
        {
            return 0;
        }

        // Perform a byte-by-byte search to check that searchFor can be
        //   found in searchIn

        // b4 and b3 are always present
        if (b4 != _section1[i] || b3 != _section1[i + 1])
        {
            continue;
        }

        // b2 and b1 are only checked when I have pointers with a size
        //   of 32 bits
        if (_psz == psz32b)
        {
            if(b2 != _section1[i + 2] && b1 != _section1[i + 3])
            {
                continue;
            }
        }

        // Found the correct pointer
        return i;
    }

    return 0;
}
