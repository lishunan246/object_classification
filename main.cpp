#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QFileDialog>
#include <QQmlContext>
#include <QDebug>

#include "ImageLoader.h"

int main(int argc, char* argv[])
{
	QGuiApplication app(argc, argv);
	auto image_provider = new ImageProvider; //will be deleted by engine
	ImageLoader image_loader(image_provider);
	QQmlApplicationEngine engine;

	engine.rootContext()->setContextProperty("image_loader", &image_loader);
	engine.addImageProvider("ip", image_provider);
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
