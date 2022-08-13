#ifndef _NULLSPACE_OSS_WRAPPER_
#define _NULLSPACE_OSS_WRAPPER_
#include <QtCore/QObject>

#include <memory>

#ifdef OSS_WRAPPER_DLL
#define OSS_WRAPPER_API Q_DECL_EXPORT
#else
#define OSS_WRAPPER_API Q_DECL_IMPORT
#endif
#endif // _NULLSPACE_OSS_WRAPPER_
