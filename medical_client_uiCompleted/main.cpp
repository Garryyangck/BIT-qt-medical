#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include "InitPage.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    InitPage initPage;

    engine.rootContext()->setContextProperty("initPage", &initPage);

    // 在这里添加新的连接的类

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QObject *loginWindow = engine.rootObjects().first(); // 获取当前窗口对象

    QObject::connect(&initPage, &InitPage::loginSuccess, [&]() {
        // 关闭当前窗口
        QQuickWindow *window = qobject_cast<QQuickWindow *>(loginWindow);
        if (window) {
            window->close();
        }

        // 创建并显示新窗口
        QQmlApplicationEngine *newEngine = new QQmlApplicationEngine;
        // 这里可能有bug
        newEngine->rootContext()->setContextProperty("initPage", &initPage);

        newEngine->load(QUrl(QStringLiteral("qrc:/mainPage.qml")));

        if (newEngine->rootObjects().isEmpty())
            return;

        QQuickWindow *newWindow = qobject_cast<QQuickWindow *>(newEngine->rootObjects().first());
        if (newWindow) {
            newWindow->show();
        }
    });

    return app.exec();
}
