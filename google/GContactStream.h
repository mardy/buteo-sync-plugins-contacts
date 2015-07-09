﻿/****************************************************************************
 **
 ** Copyright (C) 2013-2014 Jolla Ltd. and/or its subsidiary(-ies).
 **               2015 Canonical Ltd.
 **
 ** Contributors: Sateesh Kavuri <sateesh.kavuri@gmail.com>
 **               Mani Chandrasekar <maninc@gmail.com>
 **               Chris Adams <chris.adams@jolla.com>
 **               Renato Araujo Oliveira Filho <renato.filho@canonical.com>
 **
 ** This program/library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public License
 ** version 2.1 as published by the Free Software Foundation.
 **
 ** This program/library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this program/library; if not, write to the Free
 ** Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 ** 02110-1301 USA
 **
 ****************************************************************************/

#ifndef GOOGLECONTACTSTREAM_H
#define GOOGLECONTACTSTREAM_H

#include "GContactAtom.h"

#include <QObject>
#include <QMap>

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include <QContact>
#include <QContactDetail>
#include <QContactSyncTarget>
#include <QContactName>
#include <QContactAddress>
#include <QContactAnniversary>
#include <QContactEmailAddress>
#include <QContactPhoneNumber>
#include <QContactNote>
#include <QContactOrganization>
#include <QContactGender>
#include <QContactFamily>
#include <QContactUrl>
#include <QContactOnlineAccount>
#include <QContactHobby>
#include <QContactAvatar>
#include <QContactBirthday>
#include <QContactGuid>
#include <QContactNickname>
#include <QContactDisplayLabel>
#include <QContactTimestamp>
#include <QContactGeoLocation>
#include <QContactFavorite>
#include <QContactExtendedDetail>
#include <QContactRingtone>

QTCONTACTS_USE_NAMESPACE

class GoogleContactStream : public QObject
{
    Q_OBJECT

public:
    enum UpdateType {
        Add,
        Modify,
        Remove
    };

public:
    explicit GoogleContactStream(bool response, const QString &accountEmail = QString(), QObject* parent = 0);
    ~GoogleContactStream();

    QByteArray encode(const QMultiMap<GoogleContactStream::UpdateType, QPair<QContact, QStringList> > &updates);
    GoogleContactAtom* parse(const QByteArray &xmlBuffer);

signals:
    void parseDone(bool);

// Decoding XML stream to QContacts
private:
    void initAtomFunctionMap();
    void initResponseFunctionMap();
    void initFunctionMap();

    // Atom feed elements handler methods
    void handleAtomUpdated();
    void handleAtomCategory();
    void handleAtomAuthor();
    void handleAtomOpenSearch();
    void handleAtomLink();
    void handleAtomEntry();

    // Following are for the response received from the server in case of failures
    void handleEntryBatchStatus(GoogleContactAtom::BatchOperationResponse *response);
    void handleEntryBatchOperation(GoogleContactAtom::BatchOperationResponse *response);
    void handleEntryBatchId(GoogleContactAtom::BatchOperationResponse *response);

    // gContact:xxx schema handler methods
    QContactDetail handleEntryBirthday();
    QContactDetail handleEntryGender();
    QContactDetail handleEntryHobby();
    QContactDetail handleEntryNickname();
    QContactDetail handleEntryOccupation();
    QContactDetail handleEntryWebsite();
    QContactDetail handleEntryJot();
    QContactDetail handleEntryEmail();
    QContactDetail handleEntryIm();
    QContactDetail handleEntryName();
    QContactDetail handleEntryOrganization();
    QContactDetail handleEntryPhoneNumber();
    QContactDetail handleEntryStructuredPostalAddress();
    QContactDetail handleEntryUpdated();
    QContactDetail handleEntryGroup();
    QContactDetail handleEntryExtendedProperty();
    QContactDetail handleEntryEvent();

    // handle the id specially
    QContactDetail handleEntryId(QString *rawId);

    // unknown / unsupported element handler methods
    QString handleEntryLink(QContactAvatar *avatar, bool *isAvatar, QString *etag);
    QString handleEntryUnknownElement();

    // generic context parse
    QList<int> handleContext(const QString &rel) const;

    typedef void (GoogleContactStream::*Handler)();
    typedef QContactDetail (GoogleContactStream::*DetailHandler)();

    QMap<QString, GoogleContactStream::Handler> mAtomFunctionMap;
    QMap<QString, GoogleContactStream::DetailHandler> mContactFunctionMap;
    QXmlStreamReader *mXmlReader;
    GoogleContactAtom *mAtom;

// Encoding QContacts to XML stream
private:
    void encodeContactUpdate(const QContact &qContact,
                             const QStringList &unsupportedElements,
                             const UpdateType updateType,
                             const bool batch);
    void startBatchFeed();
    void endBatchFeed();
    void encodeBatchTag(const UpdateType updateType, const QString &batchElementId);
    void encodeId(const QContact &qContact, bool isUpdate = false);
    void encodeUpdatedTimestamp(const QContact &qContact);
    void encodeEtag(const QContact &qContact, bool needed);
    void encodeCategory();
    void encodeName(const QContactName &name);
    void encodePhoneNumber(const QContactPhoneNumber &phoneNumber);
    void encodeEmailAddress(const QContactEmailAddress &emailAddress);
    void encodeAddress(const QContactAddress &address);
    void encodeUrl(const QContactUrl &url);
    void encodeBirthday(const QContactBirthday &birthday);
    void encodeNote(const QContactNote &note);
    void encodeHobby(const QContactHobby &hobby);
    void encodeGeoLocation(const QContactGeoLocation &geolocation);
    void encodeOrganization(const QContactOrganization &organization);
    void encodeAvatar(const QContactAvatar &avatar, const QContact &qContact);
    void encodeGender(const QContactGender &gender);
    void encodeNickname(const QContactNickname &nickname);
    void encodeAnniversary(const QContactAnniversary &anniversary);
    void encodeOnlineAccount(const QContactOnlineAccount &onlineAccount);
    void encodeFamily(const QContactFamily &family);
    void encodeDisplayLabel(const QContactDisplayLabel &displayLabel);
    void encodeGroup(const QContactExtendedDetail &group);
    void encodeFavorite(const QContactFavorite &favorite);
    void encodeExtendedProperty(const QContactExtendedDetail &detail, bool *isGroup);
    void encodeRingTone(const QContactRingtone &ringTone);

    void encodeUnknownElements(const QStringList &unknownElements);
    QString encodeContext(const QList<int> context) const;

    QXmlStreamWriter *mXmlWriter;
    QList<QContactId> mEncodedContactsWithAvatars;
    QString mAccountEmail;
};

#endif // GOOGLECONTACTSTREAM_H
