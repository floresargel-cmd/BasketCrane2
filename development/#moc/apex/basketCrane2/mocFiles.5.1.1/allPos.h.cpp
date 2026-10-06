/****************************************************************************
** Meta object code from reading C++ file 'allPos.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane2/allPos.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'allPos.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_allPossClass_t {
    QByteArrayData data[14];
    char stringdata[225];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_allPossClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_allPossClass_t qt_meta_stringdata_allPossClass = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 23),
QT_MOC_LITERAL(2, 37, 0),
QT_MOC_LITERAL(3, 38, 25),
QT_MOC_LITERAL(4, 64, 13),
QT_MOC_LITERAL(5, 78, 20),
QT_MOC_LITERAL(6, 99, 9),
QT_MOC_LITERAL(7, 109, 25),
QT_MOC_LITERAL(8, 135, 21),
QT_MOC_LITERAL(9, 157, 3),
QT_MOC_LITERAL(10, 161, 26),
QT_MOC_LITERAL(11, 188, 10),
QT_MOC_LITERAL(12, 199, 2),
QT_MOC_LITERAL(13, 202, 21)
    },
    "allPossClass\0exportListChangedSignal\0"
    "\0sendBasketsToCrane1Signal\0mainTimerSlot\0"
    "posMouseReleasedSlot\0posClass*\0"
    "QGraphicsSceneMouseEvent*\0"
    "userChangedBasketSlot\0pos\0"
    "graphicsViewKeyPressedSlot\0QKeyEvent*\0"
    "ev\0exportListChangedSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_allPossClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x05,
       3,    0,   50,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       4,    0,   51,    2, 0x0a,
       5,    2,   52,    2, 0x0a,
       8,    1,   57,    2, 0x0a,
      10,    1,   60,    2, 0x0a,
      13,    0,   63,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 7,    2,    2,
    QMetaType::Void, 0x80000000 | 6,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,

       0        // eod
};

void allPossClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        allPossClass *_t = static_cast<allPossClass *>(_o);
        switch (_id) {
        case 0: _t->exportListChangedSignal(); break;
        case 1: _t->sendBasketsToCrane1Signal(); break;
        case 2: _t->mainTimerSlot(); break;
        case 3: _t->posMouseReleasedSlot((*reinterpret_cast< posClass*(*)>(_a[1])),(*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[2]))); break;
        case 4: _t->userChangedBasketSlot((*reinterpret_cast< posClass*(*)>(_a[1]))); break;
        case 5: _t->graphicsViewKeyPressedSlot((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 6: _t->exportListChangedSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< posClass* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< posClass* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (allPossClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&allPossClass::exportListChangedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (allPossClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&allPossClass::sendBasketsToCrane1Signal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject allPossClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_allPossClass.data,
      qt_meta_data_allPossClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *allPossClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *allPossClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_allPossClass.stringdata))
        return static_cast<void*>(const_cast< allPossClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int allPossClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void allPossClass::exportListChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void allPossClass::sendBasketsToCrane1Signal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
