///////////////////////////////////////////////////////////////
//                                                           //
//   Writer.h                                                //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef TOPOGRAPHER_WRITER_H
#define TOPOGRAPHER_WRITER_H

#include "Types.h"
#include "BinFile.h"
#include "ITranslation.h"
#include <experimental/filesystem>

namespace topographer
{
    class Writer
    {
    public:
        // Creates a writer for all the output files.
        //   outputPath, string, path to the output directory
        //   bf, *BinFile, pointer to the parsed BinFile
        //   tr, *ITranslation, pointer to the parsed translation. Default: nullptr
        Writer(const std::string & outputPath, const BinFile * bf, const ITranslation * tr = nullptr);
        ~Writer();

        // Dumps Section 1 of the BIN file
        // Returns: bool, true if written successfully, false otherwise
        bool DumpSection1() const;

        // Writes Atlas script
        // Returns: bool, true if written successfully, false otherwise
        bool WriteAtlas() const;

        // Writes extracted translations strings (useful dor debugging)
        // Returns: bool, true if written successfully, false otherwise
        bool WriteTranslationStrings() const;

        // Writes bat file for compiling Atlas scripts using Atlas.exe
        // Returns: bool, true if written successfully, false otherwise
        bool WriteBatFile() const;

    private:
        // The BIN filename, which is used as the core name in all of the
        //   output files.
        std::string _stem;

        // Output directory
        std::string _outputPath;

        const BinFile * _bf;
        const ITranslation * _tr;
    };
}

#endif

