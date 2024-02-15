#include "OperatorModel.h"
#include "OperatorModelItem.h"
#include "Constants.h"
#include "IconStorage.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPixmap>
#include <QColor>
#include <QDebug>

OperatorModel::OperatorModel(const QString &dbPath, QObject *parent):
    QAbstractItemModel{parent},
    m_rootItem{nullptr}
{
    initModelData(dbPath);
}

OperatorModel::~OperatorModel()
{
    delete m_rootItem;
    m_db.close();
}

QModelIndex OperatorModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column())
        return {};

    auto parentItem = getItem(parent);
    if (!parentItem)
        return {};

    auto childItem = parentItem->childAt(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return {};
}

QModelIndex OperatorModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return {};

    auto childItem = getItem(child);
    auto parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == m_rootItem || !parentItem)
        return {};

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int OperatorModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const auto parentItem = getItem(parent);
    return parentItem ? parentItem->childCount() : 0;
}

int OperatorModel::columnCount(const QModelIndex &parent) const
{
    return parent.column() > 0 ? 0 : 1;
}

enum CountryDataItems
{
    MccList = 0,
    Code,
    CountryName
};

enum OperatorDataItems
{
    Mcc,
    Mnc,
    OperatorName
};

QVariant OperatorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    auto item = getItem(index);
    if (!item)
        return {};

    switch (role)
    {
        case Qt::DisplayRole:
        {
            auto text = item->type() == OperatorModelItemType::Operator ?
                            item->data().at(OperatorName).toString() :
                            item->data().at(CountryName).toString();
            if (item->type() == OperatorModelItemType::Operator)
                text += QString{" (%1, %2)"}.arg(item->data().at(Mcc).toString(),
                                                 item->data().at(Mnc).toString());
            return text;
        }
        case Qt::DecorationRole:
            return item->type() == OperatorModelItemType::Country ?
                       IconStorage::instance().countryIcon(item->data().at(Code).toString()) :
                       IconStorage::instance().operatorIcon(item->data().at(Mcc).toInt(),
                                                            item->data().at(Mnc).toInt());;
        case ItemTypeRole:
            return item->type();
        case NameRole:
            return item->type() == OperatorModelItemType::Operator ?
                item->data().at(OperatorName) :
                item->data().at(CountryName);
        case MccRole:
            return item->type() == OperatorModelItemType::Operator ?
                       item->data().at(Mcc) :
                       item->data().at(MccList);
        case MncRole:
            return item->type() == OperatorModelItemType::Operator ?
                       item->data().at(Mnc) :
                       QVariant{};
        default:
            return {};
    }
}

bool OperatorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    auto item = getItem(index);
    bool result = item->setData(value.toList());
    if (result)
        emit dataChanged(index, index);
    return result;
}

bool OperatorModel::insertRows(int row, int count, const QModelIndex &parent)
{
    auto parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    bool success = parentItem->insertChildren(row, count);
    endInsertRows();

    return success;
}

bool OperatorModel::removeRows(int row, int count, const QModelIndex &parent)
{
    auto parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    bool success = parentItem->removeChildren(row, count);
    endRemoveRows();

    return success;
}

QModelIndex OperatorModel::findOperatorIndex(int mcc, int mnc) const
{
    if (!m_rootItem)
        return {};

    auto expectedCode = countryCode(mcc);
    if (expectedCode.isEmpty())
        return  {};

    for (int i = 0; i < m_rootItem->childCount(); ++i)
    {
        auto country = m_rootItem->childAt(i);
        if (country && country->data().at(Code).toString() == expectedCode)
        {
            for (int j = 0; j < country->childCount(); ++j)
            {
                auto op = country->childAt(j);
                if (op && op->data().at(Mcc) == mcc && op->data().at(Mnc) == mnc)
                    return createIndex(j, 0, op);
            }
        }
    }
    return {};
}

static const QString UpdateOperatorQuery =
        QStringLiteral("UPDATE operators "
                       "SET name = ? "
                       "WHERE mcc = ? AND mnc = ?;");

bool OperatorModel::updateOperatorName(const QModelIndex &index, const QString &newName)
{
    if (!index.isValid() || index.data(ItemTypeRole).toInt() != OperatorModelItemType::Operator)
        return false;

    int mcc = index.data(MccRole).toInt();
    int mnc = index.data(MncRole).toInt();
    QSqlQuery updateQuery;
    updateQuery.prepare(UpdateOperatorQuery);
    updateQuery.addBindValue(newName);
    updateQuery.addBindValue(mcc);
    updateQuery.addBindValue(mnc);

    bool success = updateQuery.exec();
    if (success)
        setData(index, QVariantList{mcc, mnc, newName});
    else
        qWarning() << "Failed to update (" << mcc << "," << mnc << ")";

    return success;
}


static const QString AddOperatorQuery =
        QStringLiteral("INSERT INTO operators (mcc, mnc, name) "
                       "VALUES (?, ?, ?);");

bool OperatorModel::addOperator(int mcc, int mnc, const QString &name)
{
    auto cCode = countryCode(mcc);
    if (cCode.isEmpty())
    {
        qWarning() << "country with mcc " << mcc << "not in the database";
        return false;
    }
    auto cName = countryName(mcc);

    QSqlQuery addQuery;
    addQuery.prepare(AddOperatorQuery);
    addQuery.addBindValue(mcc);
    addQuery.addBindValue(mnc);
    addQuery.addBindValue(name);

    bool success = addQuery.exec();
    if (success)
    {
        int countryRow = m_rootItem->childCount();
        for (int i = 0; i < m_rootItem->childCount(); ++i)
        {
            if (m_rootItem->childAt(i) && m_rootItem->childAt(i)->data().at(CountryName).toString() >= cName)
            {
                countryRow = i;
                break;
            }
        }
        if (countryRow == m_rootItem->childCount() ||
                m_rootItem->childAt(countryRow)->data().at(CountryName).toString() != cName)
        {
            beginInsertRows(QModelIndex{}, countryRow, countryRow);
            bool success = m_rootItem->insertChildren(countryRow, 1);
            if (success)
            {
                auto country = m_rootItem->childAt(countryRow);
                country->setData({m_countryMccs[cCode], cCode, cName});
            }
            endInsertRows();
            if (!success)
            {
                qWarning() << "failed to add" << countryName(mcc) << "to country list";
                return false;
            }
        }

        auto country = m_rootItem->childAt(countryRow);
        if (!country)
        {
            qWarning() << "failed to insert operator (" << mcc << "," << mnc << ") into list";
            return false;
        }
        int operatorRow = country->childCount();
        beginInsertRows(index(countryRow, 0, QModelIndex{}), operatorRow, operatorRow);
        country->appendChild({mcc, mnc, name});
        endInsertRows();
    }

    if (!success)
        qWarning() << "Failed to update (" << mcc << "," << mnc << ")";

    return success;
}

QString OperatorModel::countryCode(int mcc) const
{
    return m_countryCodes.value(mcc, QPair<QString, QString>{}).first;
}

QString OperatorModel::countryName(int mcc) const
{
    return m_countryCodes.value(mcc, QPair<QString, QString>{}).second;
}

static const QString AllCountriesQuery =
        QStringLiteral("SELECT DISTINCT code, name "
                       "FROM countries "
                       "ORDER BY name;");
static const QString CountryMccQuery =
        QStringLiteral("SELECT DISTINCT mcc "
                       "FROM countries "
                       "WHERE name = ?;");
static const QString OperatorsQuery =
        QStringLiteral("SELECT o.mcc, o.mnc, o.name "
                       "FROM operators AS o "
                       "INNER JOIN countries AS c "
                       "ON o.mcc = c.mcc "
                       "WHERE c.name = ? "
                       "ORDER BY o.name;");

void OperatorModel::initModelData(const QString &dbPath)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    if (!m_db.open())
    {
        qWarning() << "failed to open db";
        return;
    }

    m_rootItem = new OperatorModelItem{};

    QSqlQuery countriesQuery {AllCountriesQuery, m_db};
    while (countriesQuery.next())
    {
        QSqlQuery mccQuery;
        mccQuery.prepare(CountryMccQuery);
        mccQuery.addBindValue(countriesQuery.value(1));
        QVariantList mccList;
        if (mccQuery.exec())
        {
            while (mccQuery.next())
            {
                mccList.append(mccQuery.value(0));
                m_countryCodes.insert(mccQuery.value(0).toInt(),
                                      {countriesQuery.value(0).toString(),
                                       countriesQuery.value(1).toString()});
            }
        }
        m_countryMccs.insert(countriesQuery.value(0).toString(), mccList);
        QVariantList countryData;
        countryData << QVariant{mccList}
                    << countriesQuery.value(0)
                    << countriesQuery.value(1);

        auto item = new OperatorModelItem{countryData, m_rootItem};

        QSqlQuery operatorsQuery;
        operatorsQuery.prepare(OperatorsQuery);
        operatorsQuery.addBindValue(countriesQuery.value(1));
        if (operatorsQuery.exec())
        {
            while (operatorsQuery.next())
            {
                QVariantList operatorData;
                operatorData << operatorsQuery.value(0)
                             << operatorsQuery.value(1)
                             << operatorsQuery.value(2);
                item->appendChild(operatorData);
            }
        }
        if (item->childCount())
            m_rootItem->appendChild(item);
        else
            delete item;
    }
}

OperatorModelItem *OperatorModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        auto item = static_cast<OperatorModelItem *>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}
