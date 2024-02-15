#ifndef OPERATOREDITORVIEWMODEL_H
#define OPERATOREDITORVIEWMODEL_H

#include <QString>
#include "Constants.h"

class OperatorModel;

class OperatorEditorViewModel
{
public:
    explicit OperatorEditorViewModel(OperatorEditorMode mode = OperatorEditorMode::Edit,
                                     const QString &name = {},
                                     int mcc = 0,
                                     int mnc = 0,
                                     OperatorModel *model = nullptr);

    OperatorEditorMode editMode() const { return m_editMode; }
    const QString &name() const { return m_name; }
    int mcc() const { return m_mcc; }
    int mnc() const { return m_mnc; }
    const QString &lastError() const { return m_lastError; }

    bool updateOperatorName(int mcc, int mnc, const QString &name);
    bool addOperator(int mcc, int mnc, const QString &name);

    QString countryCode(int mcc);


private:
    OperatorEditorMode m_editMode;
    QString m_name;
    QString m_lastError;
    int m_mcc;
    int m_mnc;
    OperatorModel *m_model;

};

#endif // OPERATOREDITORVIEWMODEL_H
