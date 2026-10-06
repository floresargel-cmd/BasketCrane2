/****************************************************************************
** Meta object code from reading C++ file 'gView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../exe/apex/basketCrane2/gView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_myQGraphicsViewClass_t {
    QByteArrayData data[5];
    char stringdata[57];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_myQGraphicsViewClass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_myQGraphicsViewClass_t qt_meta_stringdata_myQGraphicsViewClass = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 16),
QT_MOC_LITERAL(2, 38, 0),
QT_MOC_LITERAL(3, 39, 10),
QT_MOC_LITERAL(4, 50, 5)
    },
    "myQGraphicsViewClass\0keyPressedSignal\0"
    "\0QKeyEvent*\0keyEv\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_myQGraphicsViewClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x05,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void myQGraphicsViewClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        myQGraphicsViewClass *_t = static_cast<myQGraphicsViewClass *>(_o);
        switch (_id) {
        case 0: _t->keyPressedSignal((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (myQGraphicsViewClass::*_t)(QKeyEvent * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&myQGraphicsViewClass::keyPressedSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject myQGraphicsViewClass::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_myQGraphicsViewClass.data,
      qt_meta_data_myQGraphicsViewClass,  qt_static_metacall, 0, 0}
};


const QMetaObject *myQGraphicsViewClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *myQGraphicsViewClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_myQGraphicsViewClass.stringdata))
        return static_cast<void*>(const_cast< myQGraphicsViewClass*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int myQGraphicsViewClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void myQGraphicsViewClass::keyPressedSignal(QKeyEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
