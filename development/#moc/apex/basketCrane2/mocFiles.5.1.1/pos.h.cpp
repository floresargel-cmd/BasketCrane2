/****************************************************************************
** Meta object code from reading C++ file 'pos.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane2/pos.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pos.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_posClass_t {
    QByteArrayData data[14];
    char stringdata[217];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_posClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_posClass_t qt_meta_stringdata_posClass = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 16),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 19),
QT_MOC_LITERAL(4, 47, 9),
QT_MOC_LITERAL(5, 57, 9),
QT_MOC_LITERAL(6, 67, 22),
QT_MOC_LITERAL(7, 90, 9),
QT_MOC_LITERAL(8, 100, 25),
QT_MOC_LITERAL(9, 126, 23),
QT_MOC_LITERAL(10, 150, 3),
QT_MOC_LITERAL(11, 154, 23),
QT_MOC_LITERAL(12, 178, 16),
QT_MOC_LITERAL(13, 195, 20)
    },
    "posClass\0posChangedSignal\0\0"
    "basketChangedSignal\0oldBasket\0newBasket\0"
    "posMouseReleasedSignal\0posClass*\0"
    "QGraphicsSceneMouseEvent*\0"
    "userChangedBasketSignal\0pos\0"
    "showPositionDlgSlotSlot\0mousePressedSlot\0"
    "loadFromDatabaseSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_posClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x05,
       3,    2,   50,    2, 0x05,
       6,    2,   55,    2, 0x05,
       9,    1,   60,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
      11,    0,   63,    2, 0x0a,
      12,    1,   64,    2, 0x0a,
      13,    0,   67,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    5,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 8,    2,    2,
    QMetaType::Void, 0x80000000 | 7,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void,

       0        // eod
};

void posClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        posClass *_t = static_cast<posClass *>(_o);
        switch (_id) {
        case 0: _t->posChangedSignal(); break;
        case 1: _t->basketChangedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->posMouseReleasedSignal((*reinterpret_cast< posClass*(*)>(_a[1])),(*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[2]))); break;
        case 3: _t->userChangedBasketSignal((*reinterpret_cast< posClass*(*)>(_a[1]))); break;
        case 4: _t->showPositionDlgSlotSlot(); break;
        case 5: _t->mousePressedSlot((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 6: _t->loadFromDatabaseSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< posClass* >(); break;
            }
            break;
        case 3:
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
            typedef void (posClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&posClass::posChangedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (posClass::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&posClass::basketChangedSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (posClass::*_t)(posClass * , QGraphicsSceneMouseEvent * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&posClass::posMouseReleasedSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (posClass::*_t)(posClass * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&posClass::userChangedBasketSignal)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject posClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_posClass.data,
      qt_meta_data_posClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *posClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *posClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_posClass.stringdata))
        return static_cast<void*>(const_cast< posClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int posClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void posClass::posChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void posClass::basketChangedSignal(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void posClass::posMouseReleasedSignal(posClass * _t1, QGraphicsSceneMouseEvent * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void posClass::userChangedBasketSignal(posClass * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
