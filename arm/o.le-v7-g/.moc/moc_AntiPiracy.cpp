/****************************************************************************
** Meta object code from reading C++ file 'AntiPiracy.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/AntiPiracy/AntiPiracy.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AntiPiracy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AntiPiracy[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      33,   11,   11,   11, 0x08,
      87,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AntiPiracy[] = {
    "AntiPiracy\0\0errorString(QString)\0"
    "onKeyPromptFinished(bb::system::SystemUiResult::Type)\0"
    "onReplyFinished()\0"
};

void AntiPiracy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AntiPiracy *_t = static_cast<AntiPiracy *>(_o);
        switch (_id) {
        case 0: _t->errorString((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->onKeyPromptFinished((*reinterpret_cast< bb::system::SystemUiResult::Type(*)>(_a[1]))); break;
        case 2: _t->onReplyFinished(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AntiPiracy::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AntiPiracy::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AntiPiracy,
      qt_meta_data_AntiPiracy, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AntiPiracy::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AntiPiracy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AntiPiracy::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AntiPiracy))
        return static_cast<void*>(const_cast< AntiPiracy*>(this));
    return QObject::qt_metacast(_clname);
}

int AntiPiracy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void AntiPiracy::errorString(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
