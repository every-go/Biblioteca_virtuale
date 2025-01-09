/****************************************************************************
** Meta object code from reading C++ file 'adminarea.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "interfaccia_grafica/adminarea.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'adminarea.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AdminArea_t {
    const uint offsetsAndSize[24];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_AdminArea_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_AdminArea_t qt_meta_stringdata_AdminArea = {
    {
QT_MOC_LITERAL(0, 9), // "AdminArea"
QT_MOC_LITERAL(10, 23), // "objectCreationRequested"
QT_MOC_LITERAL(34, 0), // ""
QT_MOC_LITERAL(35, 13), // "selectedClass"
QT_MOC_LITERAL(49, 12), // "removeObject"
QT_MOC_LITERAL(62, 11), // "Biblioteca*"
QT_MOC_LITERAL(74, 6), // "biblio"
QT_MOC_LITERAL(81, 12), // "modifyObject"
QT_MOC_LITERAL(94, 13), // "tornaIndietro"
QT_MOC_LITERAL(108, 8), // "showUser"
QT_MOC_LITERAL(117, 8), // "aggiungi"
QT_MOC_LITERAL(126, 6) // "chiudi"

    },
    "AdminArea\0objectCreationRequested\0\0"
    "selectedClass\0removeObject\0Biblioteca*\0"
    "biblio\0modifyObject\0tornaIndietro\0"
    "showUser\0aggiungi\0chiudi"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AdminArea[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    1 /* Public */,
       4,    1,   59,    2, 0x06,    3 /* Public */,
       7,    1,   62,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   65,    2, 0x08,    7 /* Private */,
       9,    0,   66,    2, 0x08,    8 /* Private */,
      10,    0,   67,    2, 0x08,    9 /* Private */,
      11,    0,   68,    2, 0x08,   10 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AdminArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AdminArea *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->objectCreationRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->removeObject((*reinterpret_cast< std::add_pointer_t<Biblioteca*>>(_a[1]))); break;
        case 2: _t->modifyObject((*reinterpret_cast< std::add_pointer_t<Biblioteca*>>(_a[1]))); break;
        case 3: _t->tornaIndietro(); break;
        case 4: _t->showUser(); break;
        case 5: _t->aggiungi(); break;
        case 6: _t->chiudi(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AdminArea::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AdminArea::objectCreationRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (AdminArea::*)(Biblioteca * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AdminArea::removeObject)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (AdminArea::*)(Biblioteca * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AdminArea::modifyObject)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AdminArea::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_AdminArea.offsetsAndSize,
    qt_meta_data_AdminArea,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_AdminArea_t
, QtPrivate::TypeAndForceComplete<AdminArea, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Biblioteca *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Biblioteca *, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *AdminArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AdminArea::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AdminArea.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "JsonObserver"))
        return static_cast< JsonObserver*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int AdminArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void AdminArea::objectCreationRequested(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AdminArea::removeObject(Biblioteca * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AdminArea::modifyObject(Biblioteca * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
