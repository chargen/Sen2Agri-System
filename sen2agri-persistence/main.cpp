#include <stdexcept>

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>
#include <QDBusError>

#include "persistencemanager.hpp"
#include "persistencemanager_adaptor.h"
#include "settings.hpp"

using std::runtime_error;
using std::exception;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const auto &settings = Settings::readSettings(QStringLiteral("/etc/sen2agri.conf"));

    PersistenceManager::registerMetaTypes();

    QDBusConnection connection = QDBusConnection::systemBus();
    PersistenceManager persistenceManager(connection, settings);

    try {
        new PersistenceManagerAdaptor(&persistenceManager);

        if (!connection.registerObject("/", &persistenceManager)) {
            throw runtime_error(connection.lastError().message().toStdString());
        }

        if (!connection.registerService("org.esa.sen2agri.persistenceManager")) {
            throw runtime_error(connection.lastError().message().toStdString());
        }
    } catch (const exception &e) {
        qDebug() << QStringLiteral("Error registering to the system bus: %1").arg(e.what());
    }

    return app.exec();
}
