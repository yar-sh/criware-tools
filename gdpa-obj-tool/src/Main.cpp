///////////////////////////////////////////////////////////////
//                                                           //
//   Main.cpp                                                //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "GdpaObjTool.h"
#include <iostream>
#include <experimental/filesystem>
#include <fstream>

using namespace std;
using namespace gpdaobjtool;
namespace fs = std::experimental::filesystem;

// Parse program launch argments
// Returns: bool, true if no errors, false otherwise
bool ParseArguments(int argc, char **argv, Arguments & args)
{
    if (argc < 4)
    {/*
        cout << "topographer is a tool made by HaselLoyance, 2018-2019." << endl;
        cout << "topographer provides a way to extract strings from the" << endl;
        cout << "BIN files of SciAdv PSP games into the Atlas scripts." << endl;
        cout << "There is also an option for automatic translation" << endl;
        cout << "loader/merger, but you would have to code your own" << endl;
        cout << "implementation based on your specific needs." << endl;
        cout << endl << endl;*/
        cout << "Usage:" << endl;
        cout << "\tgdpa-obj-unpacker d <obj_file> <text_script_file>" << endl;
        cout << "\tgdpa-obj-unpacker d A01_07AA1.obj A01_07AA1.txt" << endl;
        cout << "OR" << endl;
        cout << "\tgdpa-obj-unpacker c <text_script_file> <obj_file>" << endl;
        cout << "\tgdpa-obj-unpacker c A01_07AA1.txt A01_07AA1.obj" << endl;
        cout << "OR" << endl;
        cout << "\tgdpa-obj-unpacker t <text_script_file> <translation_file>" << endl;
        cout << "\tgdpa-obj-unpacker t A01_07AA1.txt 00.txt" << endl;
        /*
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
*/
        return false;
    }

    // Determining operation type
    string t = string(argv[1]);
    if (t != "d" && t != "c" && t != "t")
    {
        cout << "Wrong operation flag. Use: " << endl;
        cout << "\td - decompress" << endl;
        cout << "\tc - compress" << endl;
        cout << "\tt - translation mode" << endl;
        return false;
    }

    if (t == "d")
    {
        args.operationType = Arguments::OperationType::otDecompress;
    }
    else if (t == "c")
    {
        args.operationType = Arguments::OperationType::otCompress;
    }
    else // t == "t"
    {
        args.operationType = Arguments::OperationType::otTranslate;

        // Check for the existance of the translation file
        args.fileParam2 = string(argv[3]);
        if (!fs::exists(args.fileParam2) || fs::path(args.fileParam2).filename() == "")
        {
            cout << "Translation file doesn't exist" << endl;
            return false;
        }
    }

    args.fileParam1 = string(argv[2]);
    if (!fs::exists(args.fileParam1) || fs::path(args.fileParam1).filename() == "")
    {
        cout << "Input file doesn't exist" << endl;
        return false;
    }

    args.fileParam2 = string(argv[3]);

    return true;
}

int HandleDecompress(const Arguments & args)
{
    GpdaObjFile gof;

    cout << "Input OBJ file: " << args.fileParam1 << endl;
    if (!gof.LoadObj(&args.fileParam1))
    {
        cout << "[ERROR] Unable to parse input file: " << args.fileParam1 << endl;
        return -1;
    }

    cout << "\tTotal instructions: " << gof.instructions()->size() << endl;
    cout << "\tTotal text lines: " << (unsigned)gof.textLines() << endl;

    cout << "\tWriting text script: " << args.fileParam2 << endl;
    if(!gof.WriteTxt(&args.fileParam2))
    {
        cout << "[WARNING] Wasn't able to write text script." << endl;
        return -1;
    }

    cout << "\tSuccess" << endl;

    return 0;
}

int HandleCompress(const Arguments & args)
{
    GpdaObjFile gof;

    cout << "Input text script file: " << args.fileParam1 << endl;
    if (!gof.LoadTxt(&args.fileParam1))
    {
        cout << "[ERROR] Unable to parse input file: " << args.fileParam1 << endl;
        return -1;
    }

    cout << "\tTotal instructions: " << gof.instructions()->size() << endl;
    cout << "\tTotal text lines: " << (unsigned)gof.textLines() << endl;

    cout << "\tWriting OBJ file: " << args.fileParam2 << endl;
    if(!gof.WriteObj(&args.fileParam2))
    {
        cout << "[WARNING] Wasn't able to write OBJ file." << endl;
        return -1;
    }

    cout << "\tSuccess" << endl;
    return 0;
}

void PrintActionsHelp()
{
    cout << "\nActions:" << endl;
    cout << "\t[1] - Skip game text" << endl;
    cout << "\t[2] - Merge game text with translation line" << endl;
    cout << "\t[3] - Select previous translation line" << endl;
    cout << "\t[4] - Select next translation line" << endl;
    cout << "\t[5] - Auto-merge onwards" << endl;
    cout << "\t[6] - Enter translation line manually" << endl;
    cout << "\t[7] - Save text script" << endl;
    cout << "\t[0] - Show this help message" << endl;
}

// todo????
/*
string * GetNextTranslationLine(const GpdaObjFile & gof, uint16_t currentLine)
{
    
    for(uint16_t lineCounter = 0, i = 0; i < gof.instructions().size(); i++)
    {
        if (gof.instruction)
    }
    return "";
}*/

int HandleTranslate(const Arguments & args)
{
    GpdaObjFile gof;
    TranslationFile tf;

    cout << "Input text script file: " << args.fileParam1 << endl;
    if (!gof.LoadTxt(&args.fileParam1))
    {
        cout << "[ERROR] Unable to parse input file: " << args.fileParam1 << endl;
        return -1;
    }

    cout << "\tTotal instructions: " << gof.instructions()->size() << endl;
    cout << "\tTotal text lines: " << (unsigned)gof.textLines() << endl;

    cout << "\tInput translation file: " << args.fileParam2 << endl;
    if (!tf.Load(&args.fileParam2))
    {
        cout << "[ERROR] Unable to parse translation file: " << args.fileParam2 << endl;
        return -1;
    }

    cout << "\tTotal translation lines: " << tf.lines()->size() << endl;

    PrintActionsHelp();

    while(true)
    {
        cout << "Game text:";
        cout << "Trn. line:";

        string input;
        cout << ">: ";
        getline(cin, input);

        Utils::TrimString(&input);

        if (input.size() != 1)
        {
            if (input.size() > 1)
            {
                cout << "Invalid action: " << input << endl;
            }
            continue;
        }

        if (input[0] == '0')
        {
            PrintActionsHelp();
        }
        else if (input[0] == '1')
        {

        }
        else if (input[0] == '2')
        {
            
        }
        else if (input[0] == '3')
        {
            
        }
        else if (input[0] == '4')
        {
            
        }
        else if (input[0] == '5')
        {
            
        }
        else if (input[0] == '6')
        {
            
        }
        else if (input[0] == '7')
        {
            
        }
        else
        {
            cout << "Invalid action: " << input << endl;
        }
    }

    return 0;
}
int main(int argc, char **argv)
{
    // Check that the passed arguments are valid
    Arguments args;
    if (!ParseArguments(argc, argv, args))
    {
        return -1;
    }

    if (args.operationType == Arguments::OperationType::otDecompress)
    {
        return HandleDecompress(args);
    }
    else if (args.operationType == Arguments::OperationType::otCompress)
    {
        return HandleCompress(args);
    }
    else if (args.operationType == Arguments::OperationType::otTranslate)
    {
        return HandleTranslate(args);
    }

    return 0;
}
