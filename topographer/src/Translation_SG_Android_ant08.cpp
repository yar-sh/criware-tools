///////////////////////////////////////////////////////////////
//                                                           //
//   Translation_SG_Android_ant08.cpp                        //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "Translation_SG_Android_ant08.h"

using namespace std;
using namespace topographer;

Translation_SG_Android_ant08::~Translation_SG_Android_ant08() { }

// Loads a translation file
//   filepath, string, the file name of the translation file
// Returns: bool, true if loaded successfully, false otherwise
bool Translation_SG_Android_ant08::Load(const string & filepath)
{
    _filepath = filepath;

    cout << "\t...Loading translation file: " << filepath << endl;

    ifstream file(filepath);
    if(file.bad())
    {
        return false;
    }

    while(true)
    {
        string str;
        getline(file, str);

        if (file.eof())
        {
            break;
        }

        Utils::TrimString(str);

        if (str.empty())
        {
            continue;
        }

        // Only care about string that start with "#mes"
        if (str.find("#mes") != 0)
        {
            continue;
        }

        // But ignore strings that start with "#mess"
        if (str.size() >= 5 && str[4] == 's')
        {
            continue;
        }

        _ParseString(str);
    }

    file.close();

    cout << "\tTotal translation strings: " << sl.GetDirectStrings().size() << endl;
    cout << "\tTotal translation key indexes: " << sl.GetKeyStrings().size() << endl;

    return true;
}

// Sanitizes the string and properly indexes it as either a
//   key string or inner string
void Translation_SG_Android_ant08::_ParseString(const string & str)
{
    // NOTE: in this case I don't really care about the correct type of token,
    //   so I'm making an entire translation string into a single token of
    //   type ttRaw.

    string s = str;

    // Replace a bunch of stuff in the translation string so that
    //   it is compatible with game control sequences
    Utils::ReplaceString(s, "&cTIPSC;", "<DICT>");
    Utils::ReplaceString(s, "&cMESC;", "</DICT>");
    Utils::ReplaceString(s, "&n", "<LINE>");
    Utils::ReplaceString(s, "&sp(142);", "%t052");
    Utils::ReplaceString(s, "&lfCR1;", "<LINE>\n");
    Utils::ReplaceString(s, "&lfCR2;", "<LINE>\n");
    Utils::ReplaceString(s, "&lfCR3;", "<LINE>\n");
    Utils::ReplaceString(s, "&lfCR4;", "<LINE>\n");
    Utils::ReplaceString(s, "&lfCR5;", "<LINE>\n");
    Utils::ReplaceString(s, "&lfCR6;", "<LINE>\n");
    Utils::ReplaceString(s, "&c5;", "%CFF008");
    Utils::ReplaceString(s, "&c00;", "%CE");
    Utils::ReplaceString(s, "“", " \"");
    Utils::ReplaceString(s, "”", "\" ");
    Utils::ReplaceString(s, "‘", "'");
    Utils::ReplaceString(s, "’", "'");
    Utils::ReplaceString(s, "&rs", "");
    Utils::ReplaceString(s, "&rt&re", "");
    Utils::ReplaceString(s, "&rt", "");
    Utils::ReplaceString(s, "&re", "");
    Utils::ReplaceString(s, "&cPhC;", "");
    Utils::ReplaceString(s, "&lc", "%LC");
    Utils::ReplaceString(s, "─", "-");
    Utils::ReplaceString(s, "ー", "-");
    Utils::ReplaceString(s, "、", ", ");
    Utils::ReplaceString(s, "……", "...");
    Utils::ReplaceString(s, "…", "...");
    Utils::ReplaceString(s, "　", " ");
    Utils::ReplaceString(s, "  ", " ");
    Utils::ReplaceString(s, "²", "⑯");
    Utils::ReplaceString(s, "〜", "~");
    Utils::ReplaceString(s, "？", "?");
    Utils::ReplaceString(s, "！", "!");

    bool isKeyIndex = s[4] == '2';

    // There is a small exception on how to extract string from SG03_06
    bool SG03_06_exception = _filepath.find("SG03_06") != string::npos &&
        s.find("$W(LR_LIP_CRS),VID_CRS,") != string::npos;

    if (SG03_06_exception)
    {
        string startAfter = "$W(LR_LIP_CRS),VID_CRS,";
        size_t lineStart = s.find(startAfter) + startAfter.size();
        size_t lineEnd = s.find_last_of('%');

        string data(s, lineStart, lineEnd - lineStart);
        data = string()
               + "<LINE>\n"
               + "<LINE>\n"
               + "<LINE>\n"
               + data;

        Utils::TrimString(data);

        GameString gs(ttRaw);
        gs.AppendToLastToken(data);

        sl.AddInnerString(gs);

        return;
    }

    // Perform some magic to extract strings and store them in the format I want
    if (isKeyIndex)
    {
        // Extracting dialog lines
        string delimSpeakerBegin = ",＠";
        string delimSpeakerEnd = "＠「";

        size_t speakerBegin = s.find(delimSpeakerBegin) + delimSpeakerBegin.size();
        size_t speakerEnd = s.find(delimSpeakerEnd);

        size_t lineEnd = s.find("」");
        string data(s, speakerEnd + delimSpeakerEnd.size(), lineEnd - speakerEnd - delimSpeakerEnd.size());

        Utils::TrimString(data);

        GameString gs(ttRaw);
        gs.AppendToLastToken(data);

        sl.AddKeyString(gs);
    }
    else
    {
        // Extracting normal lines
        size_t lineStart = s.find(' ') + 1;
        size_t lineEnd = s.find_last_of('%');

        string data(s, lineStart, lineEnd - lineStart);

        Utils::TrimString(data);

        GameString gs(ttRaw);
        gs.AppendToLastToken(data);

        sl.AddInnerString(gs);
    }
}

