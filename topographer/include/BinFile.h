///////////////////////////////////////////////////////////////
//                                                           //
//   BinFile.h                                               //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

//
// CANONICALLY LEAVING THIS FILE UNDOCUMENTED (tm)
//

#ifndef TOPOGRAPHER_BIN_FILE_H
#define TOPOGRAPHER_BIN_FILE_H

#include "Types.h"
#include "Utils.h"
#include "EncodingTable.h"
#include "ITranslation.h"
#include "GameString.h"

namespace topographer
{
    class BinFile
    {
        friend class Writer;

    public:
        BinFile(EncodingTable * table);
        ~BinFile();

        bool Load(const std::string & filepath);
        void Process();

        // All pointers in a BIN file
        std::vector<Pointer> pointerTable;

        // List of strings in a BIN file
        StringList sl;

    private:
        void _SplitSections();
        void _MapPointers();
        void _ParseStrings();

        uint32_t _FindPointerToAddress(uint32_t address) const;

        std::string _filepath;
        uint32_t _splitAddress;
        BArr _section1;
        BArr _section2;
        PointerSz _psz = psz16b;
        const EncodingTable * _encTable;
    };
}

#endif

