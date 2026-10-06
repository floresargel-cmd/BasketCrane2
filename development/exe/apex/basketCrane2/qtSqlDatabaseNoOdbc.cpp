// Recompile Qt's database registry for this application without its built-in
// ODBC factory. The shared Qt library and other applications stay unchanged.
// Use the matching Qt 5.7 source shipped with this repository; its license and
// copyright remain in that source. Custom QSqlDriver instances still work.
#define QT_BUILD_SQL_LIB
#include <QtCore/qglobal.h>
#undef QT_SQL_ODBC
#undef QT_SQL_PSQL
#undef QT_SQL_MYSQL
#undef QT_SQL_OCI
#undef QT_SQL_TDS
#undef QT_SQL_DB2
#undef QT_SQL_SQLITE
#undef QT_SQL_SQLITE2
#undef QT_SQL_IBASE
#include "qsqldatabase.cpp"
