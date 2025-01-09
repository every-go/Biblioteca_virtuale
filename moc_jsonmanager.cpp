/****************************************************************************
** Meta object code from reading C++ file 'jsonmanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "JSON/jsonmanager.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jsonmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_JsonManager_t {
    const uint offsetsAndSize[36];
    char stringdata0[173];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_JsonManager_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_JsonManager_t qt_meta_stringdata_JsonManager = {
    {
QT_MOC_LITERAL(0, 11), // "JsonManager"
QT_MOC_LITERAL(12, 20), // "handleObjectCreation"
QT_MOC_LITERAL(33, 0), // ""
QT_MOC_LITERAL(34, 13), // "selectedClass"
QT_MOC_LITERAL(48, 12), // "deleteObject"
QT_MOC_LITERAL(61, 11), // "Biblioteca*"
QT_MOC_LITERAL(73, 6), // "biblio"
QT_MOC_LITERAL(80, 12), // "modifyObject"
QT_MOC_LITERAL(93, 11), // "savePrenota"
QT_MOC_LITERAL(105, 9), // "saveLetto"
QT_MOC_LITERAL(115, 9), // "Cartaceo*"
QT_MOC_LITERAL(125, 5), // "carta"
QT_MOC_LITERAL(131, 13), // "saveAscoltato"
QT_MOC_LITERAL(145, 3), // "Cd*"
QT_MOC_LITERAL(149, 2), // "cd"
QT_MOC_LITERAL(152, 9), // "saveVisto"
QT_MOC_LITERAL(162, 5), // "Film*"
QT_MOC_LITERAL(168, 4) // "film"

    },
    "JsonManager\0handleObjectCreation\0\0"
    "selectedClass\0deleteObject\0Biblioteca*\0"
    "biblio\0modifyObject\0savePrenota\0"
    "saveLetto\0Cartaceo*\0carta\0saveAscoltato\0"
    "Cd*\0cd\0saveVisto\0Film*\0film"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_JsonManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x0a,    1 /* Public */,
       4,    1,   59,    2, 0x0a,    3 /* Public */,
       7,    1,   62,    2, 0x0a,    5 /* Public */,
       8,    1,   65,    2, 0x0a,    7 /* Public */,
       9,    1,   68,    2, 0x0a,    9 /* Public */,
      12,    1,   71,    2, 0x0a,   11 /* Public */,
      15,    1,   74,    2, 0x0a,   13 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,

       0        // eod
};

void JsonManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<JsonManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->handleObjectCreation((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->deleteObject((*reinterpret_cast< std::add_pointer_t<Biblioteca*>>(_a[1]))); break;
        case 2: _t->modifyObject((*reinterpret_cast< std::add_pointer_t<Biblioteca*>>(_a[1]))); break;
        case 3: _t->savePrenota((*reinterpret_cast< std::add_pointer_t<Biblioteca*>>(_a[1]))); break;
        case 4: _t->saveLetto((*reinterpret_cast< std::add_pointer_t<Cartaceo*>>(_a[1]))); break;
        case 5: _t->saveAscoltato((*reinterpret_cast< std::add_pointer_t<Cd*>>(_a[1]))); break;
        case 6: _t->saveVisto((*reinterpret_cast< std::add_pointer_t<Film*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Biblioteca* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Biblioteca* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Biblioteca* >(); break;
            }
            break;
        }
    }
}

const QMetaObject JsonManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_JsonManager.offsetsAndSize,
    qt_meta_data_JsonManager,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_JsonManager_t
, QtPrivate::TypeAndForceComplete<JsonManager, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Biblioteca *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Biblioteca *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Biblioteca *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cartaceo *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cd *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Film *, std::false_type>


>,
    nullptr
} };


const QMetaObject *JsonManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *JsonManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_JsonManager.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int JsonManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
