#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

namespace Ide::Ui {
    class ApiTokenDialog : public QDialog {
        Q_OBJECT
    public:
        explicit ApiTokenDialog(QWidget* parent = nullptr);

        QString getToken();
        QByteArray getBase64Token();

    private:
        void createLayout();
        void createConntetions();

        QScopedPointer<QLabel> m_dilogTextLabel;
        QScopedPointer<QPushButton> m_applyButton;
        QScopedPointer<QPushButton> m_cancelButton;
        QScopedPointer<QLineEdit> m_tokenEdit;

        QString m_token;
    };

}
