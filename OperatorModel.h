#ifndef OPERATORMODEL_H
#define OPERATORMODEL_H

#include <QAbstractItemModel>
#include <QHash>
#include <QSqlDatabase>
#include <QVector>

class OperatorModelItem;

class OperatorModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit OperatorModel(const QString &dbPath, QObject *parent = nullptr);
    ~OperatorModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    QModelIndex findOperatorIndex(int mcc, int mnc) const;

    bool updateOperatorName(const QModelIndex &index, const QString &newName);
    bool addOperator(int mcc, int mnc, const QString &name);

    QString countryCode(int mcc) const;
    QString countryName(int mcc) const;
private:
    void initModelData(const QString &dbPath);
    OperatorModelItem *getItem(const QModelIndex &index) const;

    QSqlDatabase m_db;
    QHash<QString, QVariantList> m_countryMccs;
    QHash<int, QPair<QString, QString>> m_countryCodes;
    OperatorModelItem *m_rootItem;
};

#endif // OPERATORMODEL_H
