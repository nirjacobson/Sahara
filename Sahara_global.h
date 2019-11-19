#ifndef SAHARA_GLOBAL_H
#define SAHARA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SAHARA_LIBRARY)
#  define SAHARA_EXPORT Q_DECL_EXPORT
#else
#  define SAHARA_EXPORT Q_DECL_IMPORT
#endif

#endif // SAHARA_GLOBAL_H
