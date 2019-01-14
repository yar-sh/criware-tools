# topographer
This is a tool for extracting strings from the BIN files of the PSP SciAdv games. The name `topographer` comes from the synonym to the word `Cartographer`, which is "a program designed to extract the script(s) from a file and dump them in such a way as to be easily inserted using Atlas" [(source)](https://www.romhacking.net/utilities/647/).

Since I was unable to use `Cartographer` with SciAdv games (mainly, [Steins;Gate](https://github.com/HaselLoyance/steins-gate-psp-patch)), I wrote my own tool to do this.

It takes in an encoding table and a BIN file and produces an Atlas script, which can be easily edited in any text editor and then compiled back into the BIN file using [Atlas](https://www.romhacking.net/utilities/224/).

As a nice addition, `topographer` has a feature to merge existing translation from a file into the generated Atlas script.

**Note**: The generated Atlas scripts rely on encoding table file named `encoding.tbl` which should be one directory above the script file (that is, `../<output_directory>`). And generated bat files rely on `Atlas.exe`, which also must be one directory above (that is, `../<output_directory>`).

#### Compiling
- Linux: `make clean && make all`. This will get you an executable file `./build/topographer.run`.
- Windows: Either use bash subsystem or Visual Studio with project in `./vs`. Visual Studio will build an executable file `./build/topographer-xXX.exe`, where XX is either `86` or `64`.
- I'll post Release some time in the future.

#### Usage
- `topographer <BIN_file> <output_directory>`
This command will load encoding rules from `./encoding.tbl`. It would then load and parse a pointer table from `<BIN_file>`. This will result in 3 output files that are then placed in the `<output_directory>`. These files are:
  - `<BIN_file>.ATLAS`, the resulting Atlas script, which can be modified as plain text.
  - `<BIN_file>-section1.BIN`, the pointer table data of the BIN file, which is required for compiling the Atlas script.
  - `<BIN_file>.bat`, batch file to compile final BIN file from Atlas script and section1.
- `topographer <BIN_file> <translation_file> <output_directory>`
This command will load encoding rules from `./encoding.tbl`. It would then load and parse a pointer table from `<BIN_file>`. Next, it will parse translated game strings from `<translation_file>` and merge them with the strings in `<BIN_file>`. This will result in 4 output files that are then placed in the `<output_directory>`. These files are:
  - `<BIN_file>.ATLAS`, the resulting Atlas script, which can be modified as plain text.
  - `<BIN_file>-section1.BIN`, the pointer table data of the BIN file, which is required for compiling the Atlas script.
  - `<BIN_file>-strings.TXT`, translation strings extracted from translation file.
  - `<BIN_file>.bat`, batch file to compile final BIN file from Atlas script and section1.

#### Translation process
**Disclaimer: it gets techincal**.
As mentioned before, `topographer` supports automatic translation loading and merging. This requires to have a proper translation parser and translation merger implemented. This is relatively easy, since `topographer` was made to be extendable, but it would require C++ knowledge as well as understanding of some `topographer` internals (*pssst, the source code is documented*).

Please understand, that the merge will never be perfect, as control codes in BIN files might appear inside of the Japanese string, but they might not appear in the translated English string. This would still require you to manually check every file.

The other important thing is that not all translations are exact. In some cases, they can be so different, that this entire process of merging would be useless to you. (eg: different number of key strings, different number of inner strings, etc).

For `Translation` parser:
Lets assume you have translated game strings in some kind of file format. In order for `topographer` to work, this strings have to be loaded in a proper way. For example:
1. Make a new file `./include/Translation_New_Parser.h` with the minimum following code:
```cpp
#ifndef TOPOGRAPHER_TRANSLATION_NEW_PARSER_H
#define TOPOGRAPHER_TRANSLATION_NEW_PARSER_H

#include "ITranslation.h"
#include <iostream>
#include <fstream>

namespace topographer
{
    class Translation_New_Parser : public ITranslation
    {
    public:
        ~Translation_New_Parser();
        bool Load(const std::string & filepath);
    };
}
#endif
```
2. Then make another file `./src/Translation_New_Parser.cpp` with the minimum following code:
```cpp
#include "Translation_New_Parser.h"

using namespace std;
using namespace topographer;

Translation_New_Parser::~Translation_New_Parser() { }

bool Translation_New_Parser::Load(const string & filepath)
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

        // Parse and add the string however you like
        // Example:
        // GameString gs(ttRaw);
        // gs.AppendToLastToken(BArr(str.begin(), str.end()));
        // sl.AddInnerString(gs);
    }

    file.close();

    cout << "\tTotal translation strings: " << sl.GetDirectStrings().size() << endl;
    cout << "\tTotal translation key indexes: " << sl.GetKeyStrings().size() << endl;

    return true;
}
```
3. Write the proper parser for strings that suits your translation file format. The goal here is to populate `sl` (derived from `ITranslation`), which is a list of strings, via methods `.AddInnerString(GameString)` and `.AddKeyString(GameString)`.
In my case with Steins;Gate I decided that all spoken lines (that have a character name) are key strings and therefore have an associated `KeyIndex`, and everything else (non spoken lines) are inner strings. Inner strings lie inside of the key indexes.
Here is a layout of how you can think about this:
    ```
    Upon entering the assembly hall on the 8th floor we're greeted by a cheap-looking set up. // Direct string 0, doesn't belong to any key index

    A sign says 'Doctor Nakabachi's Time Machine Invention Press Conference'. // Direct string 1, also doesn't belong to any key index

    【まゆり】Anyway Okarin Okarin. // Direct string 2, keyIndex 0

    She's been calling me that for a while now but that is neither my real name nor my codename. It's a nickname she's given me that even I don't like. // Direct string 3, innerString 0 of keyIndex 0

    blah-blah // Direct string 4, innerString 1 of keyIndex 0

    【倫太郎】Mayuri I keep telling you. Do not call me 'Okarin'. // Direct string 5, keyIndex 1

    【まゆり】Eh? But I've been calling you that since a long time ago. // Direct string 6, keyIndex 2
    <...>
    ```
    The BIN files are parsed in the same way, so that the strings can be matched by chunks of key indexes between BIN and translation files.

    My suggestion is for you is to read the source code, since it is mostly documented.

At this point you would have a filled `sl` with translated game strings. Now you have to come up with a `Merger`. For exaple:
1. Make a new file `./include/Merger_New_Merge.h` with the minimum following code:
```cpp
#ifndef TOPOGRAPHER_MERGER_NEW_MERGE_H
#define TOPOGRAPHER_MERGER_NEW_MERGE_H

#include "IMerger.h"
#include <iostream>

namespace topographer
{
    class Merger_New_Merge : public IMerger
    {
    public:
        ~Merger_New_Merge();
        void Merge(StringList & into, const StringList & from);
    };
}
#endif
```
2. Then make another file `./src/Merger_New_Merge.cpp` with the minimum following code:
```cpp
#include "Merger_New_Merge.h"

using namespace std;
using namespace topographer;

Merger_New_Merge::~Merger_New_Merge() { }

void Merger_New_Merge::Merge(StringList & into, const StringList & from)
{
    cout << "\t...Merging translations" << endl;
}
```
3. Now write your merger. You need to think of a strategy to find correspondance between strings in original game and translated strings. The key index model I presented above is very nice for this.
    - if there is the same number of key strings and the same number of inner strings, then there is a **bijection** (never thought I will use this word) between translated strings and game strings. Translated strings can be matched exactly onto game strings.
    - if there is the same number of key strings, but different number of inner strings, then we can match together all key indexes that have the same number of inner strings in translated and original StringLists. The ones that differ in the number of inner strings can be left untranslated and reported to be fix manually.
    - if therese is a different number of key strings, then it's kinda bad. You can start going through the list of key indexes from the beginning until you stumble upon key index that doesn't have the same number of inner strings. Then you can repeat this process backwards. Everything else would have to be fixed manually.

    For the record, this the exact merging technique I'm using in my merger. Again, this might not work for your purposes, but the source code is available.

Now the entire thing has to be called in the code.
1. Go to `./include/Types.h` and change defines `TRANSLATION_CLASS` and `MERGER_CLASS` to the classes you implemented.
2. Recompile `topographer` either through `make clean && make all` or through Visual Studio.

#### Questions?
You can contact me by using the means explained in the "[Contact](https://haselloyance.github.io/#contact)" section on my website. For bugs/crashes please use issues.

#### ☕ Maybe sponsor my coffee addiction?
If you find my work interesting, then please feel free to sponsor my [coffee](https://buymeacoff.ee/rwly8PutT) and [ko-fi](https://www.ko-fi.com/haselloyance) addictions that are currently covered entirely by my ever-growing student loans.
