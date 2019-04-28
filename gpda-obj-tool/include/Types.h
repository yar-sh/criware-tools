///////////////////////////////////////////////////////////////
//                                                           //
//   Types.h                                                 //
//   HaselLoyance 2018-2019, Unlicense                       //
//   https://github.com/HaselLoyance/criware-tools           //
//                                                           //
///////////////////////////////////////////////////////////////

#ifndef GDPA_OBJ_TOOL_TYPES_H
#define GDPA_OBJ_TOOL_TYPES_H

#include "BVec.h"
#include <vector>
#include <string>

#define GPDA_OBJ_HEADER_SZ 3 * 16
#define GPDA_OBJ_INSTRUCTION_MAX_SZ 2048

namespace gpdaobjtool
{
    // Arguments that are passed to the program on its launch.
    //   inputFile, string, path to the BIN file
    //   outputFile, string, path to the output directory
    // TODO
    struct Arguments
    {
        std::string fileParam1;
        std::string fileParam2;

        enum OperationType
        {
            otCompress,
            otDecompress,
            otTranslate,
        };

        OperationType operationType = otDecompress;
    };
}

#endif
