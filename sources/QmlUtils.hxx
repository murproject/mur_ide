#pragma once

#include <QList>
#include <QtQml>

namespace Ide::qml {

//TODO(Vlad): Register composite type.

QList<std::function<void()>> &getControllersInitializersList();
void InitializeControllers();

template<class T>
struct RegisterType
{
    RegisterType()
    {
        auto initializer = []() {
            qmlRegisterType<T>();
            T::Create();
        };
        getControllersInitializersList().append(initializer);
    }
};

} // namespace ide::qml
