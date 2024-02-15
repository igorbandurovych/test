#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Qt>

enum class OperatorEditorMode
{
    Edit,
    Add
};

enum OperatorModelItemType
{
    Root = 0,
    Country,
    Operator
};

enum OperatorModelRole
{
    ItemTypeRole = Qt::UserRole + 1,
    NameRole,
    MccRole,
    MncRole
};

#endif // CONSTANTS_H
