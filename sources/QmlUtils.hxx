#pragma once

#include <QList>
#include <QtQml>

namespace Ide::qml {

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

} 
