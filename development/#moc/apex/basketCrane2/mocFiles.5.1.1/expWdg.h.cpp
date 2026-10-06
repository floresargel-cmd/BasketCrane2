/****************************************************************************
** Meta object code from reading C++ file 'expWdg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane2/expWdg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'expWdg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_exportStationWidgetClass_t {
    QByteArrayData data[14];
    char stringdata[205];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_exportStationWidgetClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_exportStationWidgetClass_t qt_meta_stringdata_exportStationWidgetClass = {
    {
QT_MOC_LITERAL(0, 0, 24),
QT_MOC_LITERAL(1, 25, 17),
QT_MOC_LITERAL(2, 43, 0),
QT_MOC_LITERAL(3, 44, 17),
QT_MOC_LITERAL(4, 62, 31),
QT_MOC_LITERAL(5, 94, 16),
QT_MOC_LITERAL(6, 111, 8),
QT_MOC_LITERAL(7, 120, 10),
QT_MOC_LITERAL(8, 131, 12),
QT_MOC_LITERAL(9, 144, 14),
QT_MOC_LITERAL(10, 159, 16),
QT_MOC_LITERAL(11, 176, 11),
QT_MOC_LITERAL(12, 188, 13),
QT_MOC_LITERAL(13, 202, 1)
    },
    "exportStationWidgetClass\0listChangedSignal\0"
    "\0dbUpdateTimerSlot\0exportsListSelectionChangedSlot\0"
    "QListWidgetItem*\0selected\0deselected\0"
    "basketUpSlot\0basketDownSlot\0"
    "basketRemoveSlot\0resizeEvent\0QResizeEvent*\0"
    "e\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_exportStationWidgetClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x0a,
       4,    2,   51,    2, 0x0a,
       8,    0,   56,    2, 0x0a,
       9,    0,   57,    2, 0x0a,
      10,    0,   58,    2, 0x0a,
      11,    1,   59,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 5,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,

       0        // eod
};

void exportStationWidgetClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        exportStationWidgetClass *_t = static_cast<exportStationWidgetClass *>(_o);
        switch (_id) {
        case 0: _t->listChangedSignal(); break;
        case 1: _t->dbUpdateTimerSlot(); break;
        case 2: _t->exportsListSelectionChangedSlot((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 3: _t->basketUpSlot(); break;
        case 4: _t->basketDownSlot(); break;
        case 5: _t->basketRemoveSlot(); break;
        case 6: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (exportStationWidgetClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&exportStationWidgetClass::listChangedSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject exportStationWidgetClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_exportStationWidgetClass.data,
      qt_meta_data_exportStationWidgetClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *exportStationWidgetClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *exportStationWidgetClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_exportStationWidgetClass.stringdata))
        return static_cast<void*>(const_cast< exportStationWidgetClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int exportStationWidgetClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void exportStationWidgetClass::listChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
