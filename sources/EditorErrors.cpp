#include "EditorErrors.hxx"

namespace Ide::Ui {

EditorErrors *EditorErrors::instance = nullptr;

qml::RegisterType<EditorErrors> EditorErrors::Register;

EditorErrors::EditorErrors() {
  if (instance) {
    throw std::runtime_error("Instance of editor_errors already exists");
  }
}

void EditorErrors::Create() {
  instance = new EditorErrors();
}

void EditorErrors::add(int line, int column, const QString &message) {
    if (m_preList.count() > 0) {
        for (auto &error : m_preList) {
            if (error.line == line) {
                error.message += "; " + message;
                return;
            }
        }
    }

  EditorError error;
  error.line = line;
  error.column = column;
  error.message = message;
  m_preList.append(error);
}

void EditorErrors::update() {
    for (auto i = 0; i < m_preList.count(); ++i) {
        auto error = m_preList.at(i);
        beginInsertRows(QModelIndex(), i, i);
        m_errors.append(error);
        endInsertRows();
    }

    m_preList.clear();

    emit countChanged();
}

void EditorErrors::clear() {
  beginResetModel();
  m_errors.clear();
  endResetModel();

  emit countChanged();
}

QHash<int, QByteArray> EditorErrors::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[LineRole] = "line";
  roles[ColumnRole] = "column";
  roles[MessageRole] = "message";

  return roles;
}

QVariant EditorErrors::data(const QModelIndex &index, int role) const {
  QVariant var;

  if (index.isValid()) {
    if (role == LineRole) {
      var.setValue(m_errors.at(index.row()).line);
    } else if (role == ColumnRole) {
      var.setValue(m_errors.at(index.row()).column);
    } else if (role == MessageRole) {
      var.setValue(m_errors.at(index.row()).message);
    }
  }

  return var;
}

int EditorErrors::rowCount(const QModelIndex &) const {
  return m_errors.size();
}

auto EditorErrors::get_count() -> int
{
    return m_errors.count();
}
}
