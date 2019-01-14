///////////////////////////////////////////////////////////////
//                                                           //
//   Main.cpp                                                //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "Topographer.h"
#include <iostream>
#include <experimental/filesystem>

using namespace std;
using namespace topographer;
namespace fs = std::experimental::filesystem;

// Parse program launch argments
// Returns: bool, true if no errors, false otherwise
bool ParseArguments(int argc, char **argv, Arguments & args)
{
    if (argc < 3 || argc > 4)
    {
        cout << "topographer is a tool made by HaselLoyance, 2018-2019." << endl;
        cout << "topographer provides a way to extract strings from the" << endl;
        cout << "BIN files of SciAdv PSP games into the Atlas scripts." << endl;
        cout << "There is also an option for automatic translation" << endl;
        cout << "loader/merger, but you would have to code your own" << endl;
        cout << "implementation based on your specific needs." << endl;
        cout << endl << endl;
        cout << "Usage:" << endl;
        cout << "\ttopographer <BIN_file> <output_dir>" << endl;
        cout << "\ttopographer SG00_01.BIN atlas-scripts" << endl;
        cout << endl;
        cout << "\tThis command would take string encoding rules from" << endl;
        cout << "\tencoding.tbl file. It would then parse the SG00_01.BIN" << endl;
        cout << "\tand put the assembled Atlas script, dump of Section 1" << endl;
        cout << "\tand bat file for Atlas compilation into the" << endl;
        cout << "\tatlas-scripts directory." << endl;
        cout << endl << endl;
        cout << "\ttopographer <BIN_file> <translation_file> <output_dir>" << endl;
        cout << "\ttopographer SG00_01.BIN SG00_01.src atlas-scripts" << endl;
        cout << endl;
        cout << "\tThis command would take string encoding rules from" << endl;
        cout << "\tencoding.tbl file. It would then parse the SG00_01.BIN" << endl;
        cout << "\tand SG00_01.src. The translation strings will then be" << endl;
        cout << "\tmerged with original strings in the SG00_01.BIN. Then" << endl;
        cout << "\tit would put the assembled Atlas script, dump of" << endl;
        cout << "\tSection 1 and bat file for Atlas compilation into the" << endl;
        cout << "\tatlas-scripts directory." << endl;

        return false;
    }

    args.inputFile = string(argv[1]);
    if (!fs::exists(args.inputFile) || fs::path(args.inputFile).filename() == "")
    {
        cout << "Input file doesn't exist" << endl;
        return false;
    }

    if (argc == 4)
    {
        args.translationFile = string(argv[2]);
        if (!fs::exists(args.translationFile) || fs::path(args.translationFile).filename() == "")
        {
            cout << "[WARNING] Translation file is specified, but it doesn't exist. Translation will not be applied." << endl;
        }
        else
        {
            args.useTranslation = true;
        }

        args.outputPath = string(argv[3]);
    }
    else
    {
        args.outputPath = string(argv[2]);
    }

    return true;
}

int main(int argc, char **argv)
{
    // Check that the passed arguments are valid
    Arguments args;
    if (!ParseArguments(argc, argv, args))
    {
        return -1;
    }

    // Attempt to load encoding table
    EncodingTable et;
    if (!et.Load(ENCODING_TABLE_FILE))
    {
        cout << "[ERROR] Unable to open encoding table file: " << ENCODING_TABLE_FILE << endl;
        return -1;
    }

    // Attempt to load BIN file
    BinFile bf(&et);
    if (!bf.Load(args.inputFile))
    {
        cout << "[ERROR] Unable to open input file: " << args.inputFile << endl;
        return -1;
    }
    bf.Process();

    ITranslation * tr = nullptr;
    if (args.useTranslation)
    {
        tr = new TRANSLATION_CLASS;

        // Attempt to load translation file
        if(!tr->Load(args.translationFile))
        {
            cout << "[WARNING] Unable to open translation file. Skipping translation." << endl;

            args.useTranslation = false;

            delete tr;
            tr = nullptr;
        }
        else
        {
            // Attempt to merge translation with original strings
            IMerger * mr = new MERGER_CLASS;
            mr->Merge(bf.sl, tr->sl);
        }
    }

    Writer w(args.outputPath, &bf, tr);

    if(!w.WriteAtlas())
    {
        cout << "[WARNING] Wasn't able to write atlas script." << endl;
    }

    if(!w.DumpSection1())
    {
        cout << "[WARNING] Wasn't able to dump Section 1." << endl;
    }

    if(!w.WriteTranslationStrings())
    {
        cout << "[WARNING] Wasn't able to write translation strings." << endl;
    }

    if(!w.WriteBatFile())
    {
        cout << "[WARNING] Wasn't able to write bat file." << endl;
    }

    if (tr != nullptr)
    {
        delete tr;
    }

    return 0;
}
