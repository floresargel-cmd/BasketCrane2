/****************************************************************************
** Meta object code from reading C++ file 'autoWdg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane2/autoWdg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'autoWdg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_autoWidgetClass_t {
    QByteArrayData data[11];
    char stringdata[314];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_autoWidgetClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_autoWidgetClass_t qt_meta_stringdata_autoWidgetClass = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 27),
QT_MOC_LITERAL(2, 44, 0),
QT_MOC_LITERAL(3, 45, 42),
QT_MOC_LITERAL(4, 88, 5),
QT_MOC_LITERAL(5, 94, 40),
QT_MOC_LITERAL(6, 135, 40),
QT_MOC_LITERAL(7, 176, 40),
QT_MOC_LITERAL(8, 217, 35),
QT_MOC_LITERAL(9, 253, 23),
QT_MOC_LITERAL(10, 277, 35)
    },
    "autoWidgetClass\0calculateExportGroupsSignal\0"
    "\0exportsToDestackerCheckBtnStateChangedSlot\0"
    "state\0exportsToPackingCheckBtnStateChangedSlot\0"
    "importsFromNorthCheckBtnStateChangedSlot\0"
    "importsFromSouthCheckBtnStateChangedSlot\0"
    "autoExportsCheckBtnStateChangedSlot\0"
    "sendBasketsToCrane1Slot\0"
    "calculateExportGroupsBtnClickedSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_autoWidgetClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    1,   55,    2, 0x0a,
       5,    1,   58,    2, 0x0a,
       6,    1,   61,    2, 0x0a,
       7,    1,   64,    2, 0x0a,
       8,    1,   67,    2, 0x0a,
       9,    0,   70,    2, 0x0a,
      10,    0,   71,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void autoWidgetClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        autoWidgetClass *_t = static_cast<autoWidgetClass *>(_o);
        switch (_id) {
        case 0: _t->calculateExportGroupsSignal(); break;
        case 1: _t->exportsToDestackerCheckBtnStateChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->exportsToPackingCheckBtnStateChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->importsFromNorthCheckBtnStateChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->importsFromSouthCheckBtnStateChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->autoExportsCheckBtnStateChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->sendBasketsToCrane1Slot(); break;
        case 7: _t->calculateExportGroupsBtnClickedSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (autoWidgetClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&autoWidgetClass::calculateExportGroupsSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject autoWidgetClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_autoWidgetClass.data,
      qt_meta_data_autoWidgetClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *autoWidgetClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *autoWidgetClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_autoWidgetClass.stringdata))
        return static_cast<void*>(const_cast< autoWidgetClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int autoWidgetClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void autoWidgetClass::calculateExportGroupsSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
