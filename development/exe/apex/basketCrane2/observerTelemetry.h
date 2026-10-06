#ifndef BASKET_OBSERVER_TELEMETRY_H
#define BASKET_OBSERVER_TELEMETRY_H
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>
#include <functional>
#include <atomic>
#include <limits>
#include <memory>
#include "config.h"
#include "tuxClass.h"

// Owns the sole production connection in Observer. No control widget receives
// this connection, and this class calls only the communication library's reads.
class ObserverTelemetry : public QThread {
    struct Snapshot { QVector<float> floats; QVector<int> integers; QString error; };
    QMutex mutex;
    Snapshot pending;
    bool ready = false;
    std::atomic<bool> stopping{false};
    QByteArray address;
    int interval;
public:
    ObserverTelemetry(QObject *parent, const std::function<void(const QVector<float>&, const QVector<int>&, const QString&)>& consume)
        : QThread(parent), address(basketCraneConfig().text("Plc/Crane2Ip").toLatin1()),
          interval(basketCraneConfig().number("Plc/PollIntervalMs")) {
        QTimer *delivery = new QTimer(this);
        QObject::connect(delivery, &QTimer::timeout, this, [this, consume]() {
            Snapshot snapshot;
            { QMutexLocker lock(&mutex); if (!ready) return; snapshot = pending; ready = false; }
            consume(snapshot.floats, snapshot.integers, snapshot.error);
        });
        delivery->start(interval);
        start();
    }
    ~ObserverTelemetry() { stopping = true; wait(); }
protected:
    void run() override {
        std::unique_ptr<tuxipLgxConnectionClass> connection;
        while (!stopping) {
            Snapshot snapshot;
            try {
                if (!connection) connection.reset(new tuxipLgxConnectionClass(int(QCoreApplication::applicationPid()), address.data(), false));
                if (!connection->getIsConnected()) snapshot.error = "Crane PLC telemetry unavailable";
                else {
                    snapshot.floats.fill(std::numeric_limits<float>::quiet_NaN(), 51);
                    snapshot.integers.fill(INT_MIN, 76);
                    char floatTag[] = "toPcF[0]";
                    char intTag[] = "toPcI[0]";
                    _cip_errno = 0;
                    connection->tuxipReadFloat(floatTag, snapshot.floats.data(), snapshot.floats.size());
                    if (_cip_errno) snapshot.error = "Crane PLC float read failed";
                    _cip_errno = 0;
                    if (snapshot.error.isEmpty()) connection->tuxipReadInt(intTag, snapshot.integers.data(), snapshot.integers.size());
                    if (_cip_errno) snapshot.error = "Crane PLC integer read failed";
                    for (float value : snapshot.floats) if (!qIsFinite(value)) snapshot.error = "Incomplete crane PLC telemetry";
                    for (int value : snapshot.integers) if (value == INT_MIN) snapshot.error = "Incomplete crane PLC telemetry";
                }
            } catch (...) { snapshot.error = "Crane PLC telemetry read failed"; }
            if (!snapshot.error.isEmpty()) connection.reset();
            { QMutexLocker lock(&mutex); pending = snapshot; ready = true; }
            for (int elapsed = 0; elapsed < interval && !stopping; elapsed += 50) msleep(50);
        }
    }
};
#endif
