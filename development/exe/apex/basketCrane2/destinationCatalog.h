#pragma once

#include <QString>
#include <QVector>

namespace basket {

enum class DestinationId : int {
    None = 0,
    Packing = 1,
    Destacker = 9,
    InFromOut = 6,
    InFromDestacker = 7,
    PackingOverLength = 11,
    PackingDestacker = 12
};

struct Destination {
    DestinationId id;
    QString title;
};

class DestinationCatalog final {
public:
    static const QVector<Destination>& Entries()
    {
        static const QVector<Destination> entries = QVector<Destination>()
            << Destination{DestinationId::None, "None"}
            << Destination{DestinationId::Packing, "Out"}
            << Destination{DestinationId::Destacker, "Destacker"}
            << Destination{DestinationId::InFromOut, "In from out"}
            << Destination{DestinationId::InFromDestacker, "In from Destacker"}
            << Destination{DestinationId::PackingOverLength, "Out overlength"}
            << Destination{DestinationId::PackingDestacker, "Packing Destacker"};
        return entries;
    }

    static int PlcValue(DestinationId id) { return static_cast<int>(id); }

private:
    DestinationCatalog() = delete;
};

} // namespace basket
