#include "OperatorEditorViewModel.h"

#include "OperatorModel.h"
#include <QApplication>

OperatorEditorViewModel::OperatorEditorViewModel(OperatorEditorMode mode,
                                                 const QString &name,
                                                 int mcc,
                                                 int mnc,
                                                 OperatorModel *model):
    m_editMode{mode},
    m_name{name},
    m_lastError{},
    m_mcc{mcc},
    m_mnc{mnc},
    m_model{model}
{

}

bool OperatorEditorViewModel::updateOperatorName(int mcc, int mnc, const QString &name)
{
    if (!m_model)
        return false;

    m_lastError.clear();
    auto idx = m_model->findOperatorIndex(mcc, mnc);
    if (!idx.isValid())
    {
        const auto error = QT_TRANSLATE_NOOP("OperatorEditor",
                                             "Trying to edit operator that doesn't exist");
        m_lastError = QApplication::translate("OperatorEditor", error);
        return false;
    }
    if (name == idx.data(OperatorModelRole::NameRole))
    {
        const auto error = QT_TRANSLATE_NOOP("OperatorEditor",
                                             "Operator's name wasn't changed");
        m_lastError = QApplication::translate("OperatorEditor", error);
        return false;
    }
    if (name.isEmpty())
    {
        const auto error = QT_TRANSLATE_NOOP("OperatorEditor",
                                             "Operator's name can't be empty");
        m_lastError = QApplication::translate("OperatorEditor", error);
        return false;
    }
    if (!m_model->updateOperatorName(idx, name))
    {
        const auto error = QT_TRANSLATE_NOOP("OperatorEditor",
                                             "Failed to update operator with MCC %1 and MNC %2");
        m_lastError = QApplication::translate("OperatorEditor", error).arg(mcc).arg(mnc);
        return false;
    }
    return true;
}

bool OperatorEditorViewModel::addOperator(int mcc, int mnc, const QString &name)
{
    if (!m_model)
        return false;

    m_lastError.clear();
    auto idx = m_model->findOperatorIndex(mcc, mnc);
    if (idx.isValid())
    {
        const auto error = QT_TRANSLATE_NOOP("OperatorEditor",
                                             "Operator with MCC %1 and MNC %2 already exists");
        m_lastError = QApplication::translate("OperatorEditor", error).arg(mcc).arg(mnc);
        return false;
    }
    if (name.isEmpty())
    {
        const auto error = QT_TRANSLATE_NOOP("OperatorEditor",
                                             "Operator's name can't be empty");
        m_lastError = QApplication::translate("OperatorEditor", error);
        return false;
    }
    if (!m_model->addOperator(mcc, mnc, name))
    {
        const auto error = QT_TRANSLATE_NOOP("OperatorEditor",
                                             "Failed to add operator with MCC %1 and MNC %2");
        m_lastError = QApplication::translate("OperatorEditor", error).arg(mcc).arg(mnc);
        return false;
    }
    return true;
}

QString OperatorEditorViewModel::countryCode(int mcc)
{
    return m_model ? m_model->countryCode(mcc) : QString{};
}
