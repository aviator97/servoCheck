#include "mdichildbase.h"
#include <QFileInfo>

QString MdiChildBase::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
