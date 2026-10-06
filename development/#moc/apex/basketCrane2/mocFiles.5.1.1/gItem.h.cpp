/****************************************************************************
** Meta object code from reading C++ file 'gItem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane2/gItem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_gItemClass_t {
    QByteArrayData data[7];
    char stringdata[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_gItemClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_gItemClass_t qt_meta_stringdata_gItemClass = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 16),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 16),
QT_MOC_LITERAL(4, 46, 18),
QT_MOC_LITERAL(5, 65, 25),
QT_MOC_LITERAL(6, 91, 19)
    },
    "gItemClass\0hoverLeaveSignal\0\0"
    "hoverEnterSignal\0mousePressedSignal\0"
    "QGraphicsSceneMouseEvent*\0mouseReleasedSignal\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_gItemClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x05,
       3,    0,   35,    2, 0x05,
       4,    1,   36,    2, 0x05,
       6,    1,   39,    2, 0x05,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 5,    2,

       0        // eod
};

void gItemClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        gItemClass *_t = static_cast<gItemClass *>(_o);
        switch (_id) {
        case 0: _t->hoverLeaveSignal(); break;
        case 1: _t->hoverEnterSignal(); break;
        case 2: _t->mousePressedSignal((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->mouseReleasedSignal((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (gItemClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&gItemClass::hoverLeaveSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (gItemClass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&gItemClass::hoverEnterSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (gItemClass::*_t)(QGraphicsSceneMouseEvent * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&gItemClass::mousePressedSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (gItemClass::*_t)(QGraphicsSceneMouseEvent * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&gItemClass::mouseReleasedSignal)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject gItemClass::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_gItemClass.data,
      qt_meta_data_gItemClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *gItemClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *gItemClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_gItemClass.stringdata))
        return static_cast<void*>(const_cast< gItemClass*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< gItemClass*>(this));
    return QObject::qt_metacast(_clname);
}

int gItemClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void gItemClass::hoverLeaveSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void gItemClass::hoverEnterSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void gItemClass::mousePressedSignal(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void gItemClass::mouseReleasedSignal(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
