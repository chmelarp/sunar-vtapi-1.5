/* 
 * File:   Sequence.cpp
 * Author: chmelarp
 * 
 * Created on 29. září 2011, 10:53
 */

#include "VTApi.h"

Sequence::Sequence(const KeyValues& orig) : KeyValues(orig) {
    // FIXME: tohle by se melo poslat KeyValues, at si to zpracuji
    res = PQexecf(getConnector()->getConnection(), String("SELECT * FROM "+ parent->getString("dsname") +".sequences;").c_str());
}

Sequence::Sequence(const Sequence& orig) : KeyValues(orig) {
}

Sequence::~Sequence() {
}


String Sequence::getName() {
    PGtext text;                        // char* .. not necessarily initialized
    if(res) {
        PQgetf(res, position, "#text", "seqname", &text); // get formated text value from tuple at position
    }
    return String(text);
}

String Sequence::getLocation() {
    PGtext text;                        // char* .. not necessarily initialized
    if(res) {
        PQgetf(res, position, "#text", "imglocation", &text); // get formated text value from tuple at position
    }
    return String(text);
}

Interval* Sequence::newInterval() {
    return new Interval(*this);
}