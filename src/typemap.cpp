/*
 * File:   typemap.cpp
 * Author: Volf Tomas
 *
 * Created on 29. září 2011, 10:54
 */

#include "vtapi_commons.h"

using namespace std;

TypeMap::TypeMap(Connector* connector) {
    this->connector = connector;
    dataloaded = false;
}

TypeMap::~TypeMap() {
    clear();
}

void TypeMap::clear() {
    typesname.clear();
    typesoid.clear();
    reftypes.clear();
    dataloaded = false;
}

bool TypeMap::empty() {
    bool isEmpty = this->typesname.empty() || this->typesoid.empty();
    if (isEmpty) this->clear();
    return isEmpty;
}
int TypeMap::size() {
    return typesoid.size();
}

int TypeMap::toOid(const String typname) {
    if (!dataloaded) loadTypes();
    if (!typesname.count(typname) > 0) return -1;
    return this->typesname[typname].first;
}

String TypeMap::toTypname(const int oid) {
    if (!dataloaded) loadTypes();
    if (!typesoid.count(oid) > 0) return "";
    return this->typesoid[oid].first;
}

void TypeMap::loadTypes() {

    PGresult* res = PQexec(connector->getConn(),
        "SELECT oid, typname, typcategory, typlen, typelem from pg_catalog.pg_type");
    // load data types
    for (int i = 0; i < PQntuples(res); i++) {
        struct typeinfo ti;
        PGint4 oid;
        PGtext typname;
        PQgetf(res, i, "%oid %name %char %int2 %oid",
            0, &oid, 1, &typname, 2, &ti.category, 3, &ti.length, 4, &ti.elemoid);
        typesname[String(typname)] = make_pair(oid, ti);
        typesoid[oid] = make_pair(String(typname), ti);
    }
    // load defined reference types
    loadRefTypes();
    dataloaded = true;
    PQclear(res);
}

// TODO: this is possibly useless?
void TypeMap::loadRefTypes() {
    reftypes.insert("regproc");
    reftypes.insert("regprocedure");
    reftypes.insert("regoper");
    reftypes.insert("regoperator");
    reftypes.insert("regtype");
    reftypes.insert("regclass");
}

void TypeMap::registerTypes() {

    // general types registered at all times
    PGregisterType types_userdef[] =
    {
        {"seqtype", enum_put, enum_get},
        {"inouttype", enum_put, enum_get}//,
        //{"permissions", enum_put, enum_get}, // change 2 to 3 in next command
    };


    if (!PQregisterTypes(connector->getConn(), PQT_USERDEFINED, types_userdef, 2, 0))
        cerr << "Register types: " << PQgeterror() << endl;

    // PostGIS special types
#ifdef POSTGIS
    PGregisterType typespg_userdef[] = {
        {"cube", cube_put, cube_get},
        {"geometry", geometry_put, geometry_get}        
    };
    if (!PQregisterTypes(connector->getConn(), PQT_USERDEFINED, typespg_userdef, 2, 0))
        cerr << "Register types: " << PQgeterror() << endl;
#endif

    // OpenCV special types
#ifdef __OPENCV_CORE_HPP__
    PGregisterType typescv_comp[] = {
        {"cvmat", NULL, NULL}
    };
    if (!PQregisterTypes(connector->getConn(), PQT_COMPOSITE, typescv_comp, 1, 0))
        cerr << "Register types: " << PQgeterror() << endl;
#endif
}

bool TypeMap::isRefType(String name) {
    if (!dataloaded) loadTypes();
    return (reftypes.count(name) > 0);
}
bool TypeMap::isEnumType(String name) {
    if (!dataloaded) loadTypes();
    return (typesname.count(name) > 0) ? (typesname[name].second.category == 'E') : false;
}

char TypeMap::getCategory (String name) {
    if (!dataloaded) loadTypes();
    return typesname[name].second.category;
}
char TypeMap::getCategory (int oid) {
    if (!dataloaded) loadTypes();
    return typesoid[oid].second.category;
}
short TypeMap::getLength (String name) {
    if (!dataloaded) loadTypes();
    return typesname[name].second.length;
}
short TypeMap::getLength (int oid) {
    if (!dataloaded) loadTypes();
    return typesoid[oid].second.length;
}
int TypeMap::getElemOID (String name) {
    if (!dataloaded) loadTypes();
    return typesname[name].second.elemoid;
}
int TypeMap::getElemOID (int oid) {
    if (!dataloaded) loadTypes();
    return typesoid[oid].second.elemoid;
}