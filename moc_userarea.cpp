/****************************************************************************
** Meta object code from reading C++ file 'userarea.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "interfaccia_grafica/userarea.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userarea.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UserArea_t {
    const uint offsetsAndSize[34];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_UserArea_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_UserArea_t qt_meta_stringdata_UserArea = {
    {
QT_MOC_LITERAL(0, 8), // "UserArea"
QT_MOC_LITERAL(9, 7), // "prenota"
QT_MOC_LITERAL(17, 0), // ""
QT_MOC_LITERAL(18, 11), // "Biblioteca*"
QT_MOC_LITERAL(30, 6), // "biblio"
QT_MOC_LITERAL(37, 5), // "letto"
QT_MOC_LITERAL(43, 9), // "Cartaceo*"
QT_MOC_LITERAL(53, 5), // "carta"
QT_MOC_LITERAL(59, 9), // "ascoltato"
QT_MOC_LITERAL(69, 3), // "Cd*"
QT_MOC_LITERAL(73, 2), // "cd"
QT_MOC_LITERAL(76, 5), // "visto"
QT_MOC_LITERAL(82, 5), // "Film*"
QT_MOC_LITERAL(88, 4), // "film"
QT_MOC_LITERAL(93, 13), // "tornaIndietro"
QT_MOC_LITERAL(107, 6), // "chiudi"
QT_MOC_LITERAL(114, 5) // "admin"

    },
    "UserArea\0prenota\0\0Biblioteca*\0biblio\0"
    "letto\0Cartaceo*\0carta\0ascoltato\0Cd*\0"
    "cd\0visto\0Film*\0film\0tornaIndietro\0"
    "chiudi\0admin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserArea[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    1 /* Public */,
       5,    1,   59,    2, 0x06,    3 /* Public */,
       8,    1,   62,    2, 0x06,    5 /* Public */,
      11,    1,   65,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      14,    0,   68,    2, 0x08,    9 /* Private */,
      15,    0,   69,    2, 0x08,   10 /* Private */,
      16,    0,   70,    2, 0x08,   11 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 12,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UserArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UserArea *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->prenota((*reinterpret_cast< std::add_pointer_t<Biblioteca*>>(_a[1]))); break;
        case 1: _t->letto((*reinterpret_cast< std::add_pointer_t<Cartaceo*>>(_a[1]))); break;
        case 2: _t->ascoltato((*reinterpret_cast< std::add_pointer_t<Cd*>>(_a[1]))); break;
        case 3: _t->visto((*reinterpret_cast< std::add_pointer_t<Film*>>(_a[1]))); break;
        case 4: _t->tornaIndietro(); break;
        case 5: _t->chiudi(); break;
        case 6: _t->admin(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Biblioteca* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UserArea::*)(Biblioteca * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserArea::prenota)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UserArea::*)(Cartaceo * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserArea::letto)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UserArea::*)(Cd * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserArea::ascoltato)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UserArea::*)(Film * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserArea::visto)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject UserArea::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_UserArea.offsetsAndSize,
    qt_meta_data_UserArea,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_UserArea_t
, QtPrivate::TypeAndForceComplete<UserArea, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Biblioteca *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cartaceo *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cd *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Film *, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *UserArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserArea::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UserArea.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "JsonObserver"))
        return static_cast< JsonObserver*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int UserArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void UserArea::prenota(Biblioteca * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UserArea::letto(Cartaceo * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UserArea::ascoltato(Cd * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UserArea::visto(Film * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
