/****************************************************************************
** Meta object code from reading C++ file 'mW.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane2/mW.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mW.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_mainWindowClass_t {
    QByteArrayData data[23];
    char stringdata[434];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_mainWindowClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_mainWindowClass_t qt_meta_stringdata_mainWindowClass = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 13),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 20),
QT_MOC_LITERAL(4, 52, 2),
QT_MOC_LITERAL(5, 55, 5),
QT_MOC_LITERAL(6, 61, 5),
QT_MOC_LITERAL(7, 67, 5),
QT_MOC_LITERAL(8, 73, 14),
QT_MOC_LITERAL(9, 88, 19),
QT_MOC_LITERAL(10, 108, 30),
QT_MOC_LITERAL(11, 139, 35),
QT_MOC_LITERAL(12, 175, 11),
QT_MOC_LITERAL(13, 187, 8),
QT_MOC_LITERAL(14, 196, 21),
QT_MOC_LITERAL(15, 218, 28),
QT_MOC_LITERAL(16, 247, 26),
QT_MOC_LITERAL(17, 274, 1),
QT_MOC_LITERAL(18, 276, 30),
QT_MOC_LITERAL(19, 307, 28),
QT_MOC_LITERAL(20, 336, 29),
QT_MOC_LITERAL(21, 366, 25),
QT_MOC_LITERAL(22, 392, 40)
    },
    "mainWindowClass\0mainTimerSlot\0\0"
    "dataInPlcChangedSlot\0ip\0table\0index\0"
    "value\0showServerSlot\0showUsageDialogSlot\0"
    "showConVDlgActionTriggeredSlot\0"
    "showDestackerDlgActionTriggeredSlot\0"
    "qtAboutSlot\0stopSlot\0resetGraphicsViewZoom\0"
    "hmiDlgBasicButtonClickedSlot\0"
    "handShakeCrane2ChangedSlot\0s\0"
    "missionsRadioButtonClickedSlot\0"
    "modifyRadioButtonClickedSlot\0"
    "exportsRadioButtonClickedSlot\0"
    "toolBoxCurrentChangedSlot\0"
    "writeBasketToReturningConveyorActionSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mainWindowClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x0a,
       3,    4,   95,    2, 0x0a,
       8,    0,  104,    2, 0x0a,
       9,    0,  105,    2, 0x0a,
      10,    0,  106,    2, 0x0a,
      11,    0,  107,    2, 0x0a,
      12,    0,  108,    2, 0x0a,
      13,    0,  109,    2, 0x0a,
      14,    0,  110,    2, 0x0a,
      15,    0,  111,    2, 0x0a,
      16,    1,  112,    2, 0x0a,
      18,    1,  115,    2, 0x0a,
      19,    1,  118,    2, 0x0a,
      20,    1,  121,    2, 0x0a,
      21,    1,  124,    2, 0x0a,
      22,    0,  127,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::QVariant,    4,    5,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,

       0        // eod
};

void mainWindowClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        mainWindowClass *_t = static_cast<mainWindowClass *>(_o);
        switch (_id) {
        case 0: _t->mainTimerSlot(); break;
        case 1: _t->dataInPlcChangedSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QVariant(*)>(_a[4]))); break;
        case 2: _t->showServerSlot(); break;
        case 3: _t->showUsageDialogSlot(); break;
        case 4: _t->showConVDlgActionTriggeredSlot(); break;
        case 5: _t->showDestackerDlgActionTriggeredSlot(); break;
        case 6: _t->qtAboutSlot(); break;
        case 7: _t->stopSlot(); break;
        case 8: _t->resetGraphicsViewZoom(); break;
        case 9: _t->hmiDlgBasicButtonClickedSlot(); break;
        case 10: _t->handShakeCrane2ChangedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->missionsRadioButtonClickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->modifyRadioButtonClickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->exportsRadioButtonClickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->toolBoxCurrentChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->writeBasketToReturningConveyorActionSlot(); break;
        default: ;
        }
    }
}

const QMetaObject mainWindowClass::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_mainWindowClass.data,
      qt_meta_data_mainWindowClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *mainWindowClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mainWindowClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mainWindowClass.stringdata))
        return static_cast<void*>(const_cast< mainWindowClass*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int mainWindowClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
