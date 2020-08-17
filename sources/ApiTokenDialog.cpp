#include "ApiTokenDialog.hxx"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

namespace Ide::Ui {

ApiTokenDialog::ApiTokenDialog(QWidget *parent) :
    QDialog(parent),
    m_dilogTextLabel(new QLabel("API token required. Consider create token at: <a href=\"https://robocenter.net/murtracker/app/profile \">robocenter.net/murtracker</a>", this)),
    m_applyButton(new QPushButton("Apply")),
    m_cancelButton(new QPushButton("Cancel")),
    m_tokenEdit(new QLineEdit(this))
{
    setModal(true);
    createLayout();
    createConntetions();
    m_tokenEdit->setPlaceholderText("Put API token here.");
}

QString ApiTokenDialog::getToken()
{
    return m_token;
}

QByteArray ApiTokenDialog::getBase64Token()
{
    return m_token.toUtf8().toBase64();
}

void ApiTokenDialog::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_dilogTextLabel.data());
    layout->addWidget(m_tokenEdit.data());

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_applyButton.data());
    buttonLayout->addWidget(m_cancelButton.data());

    layout->addLayout(buttonLayout);
    setLayout(layout);
}

void ApiTokenDialog::createConntetions()
{
    connect(m_applyButton.data(), &QPushButton::clicked, [this]{
        m_token = m_tokenEdit->text().trimmed();
        if (m_token.size() != 32) {
            QMessageBox::warning(nullptr, "API Token size" ,"API Token must be 32 cheracter.", "Ok");
            return;
        }

        setResult(QMessageBox::Apply);
        emit accepted();
        close();
    });

    connect(m_cancelButton.data(), &QPushButton::clicked, [this]{
        setResult(QMessageBox::Cancel);
        emit rejected();
        close();
    });
}

}
