//#include "FBSQLite/SQLiteObfuscateCodecInterface.h"
//#include <sqlite3.h>

#ifndef SQLITE_OMIT_DISKIO
#    ifdef SQLITE_HAS_CODEC

extern "C" {

#        ifdef __cplusplus
using Bool = unsigned char;
#        endif
}

#    endif  // SQLITE_HAS_CODEC

#endif  // SQLITE_OMIT_DISKIO
