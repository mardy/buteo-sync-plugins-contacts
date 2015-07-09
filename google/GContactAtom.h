/****************************************************************************
 **
 ** Copyright (C) 2013-2014 Jolla Ltd. and/or its subsidiary(-ies).
 ** Contact: Chris Adams <chris.adams@jolla.com>
 **
 ** Contributors: Sateesh Kavuri <sateesh.kavuri@gmail.com>
 **               Chris Adams <chris.adams@jolla.com>
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

#ifndef GOOGLECONTACTATOM_H
#define GOOGLECONTACTATOM_H

#include <QMetaEnum>
#include <QMap>
#include <QList>
#include <QXmlStreamWriter>

#include <QContact>

QTCONTACTS_USE_NAMESPACE

class GoogleContactAtom {
public:
    GoogleContactAtom();

    void setAuthorName(const QString &authorName);
    QString authorName() const;

    void setAuthorEmail(const QString &authorEmail);
    QString authorEmail() const;

    void setId(const QString &id);
    QString id() const;

    void setUpdated(const QString &updated);
    QString updated() const;

    void setCategory(const QString &schema = QStringLiteral("http://schemas.google.com/g/2005#kind"),
                     const QString &term = QStringLiteral("http://schemas.google.com/contact/2008#contact"));
    QString categorySchema() const;
    QString categoryTerm() const;

    void setTitle(const QString &title);
    QString title() const;

    void setContent(const QString &note, const QString &type = QStringLiteral("text"));

    void setGenerator(const QString &name = QStringLiteral("Contacts"),
                      const QString &version = QStringLiteral("1.0"),
                      const QString &uri = QStringLiteral("https://sailfishos.org"));
    QString generatorName() const;
    QString generatorVersion() const;
    QString generatorUri() const;

    void setTotalResults(int totalResults);
    int totalResults() const;

    void setStartIndex(int startIndex);
    int startIndex() const;

    void setItemsPerPage(int itemsPerPage);
    int itemsPerPage() const;

    void addEntryContact(const QContact &contact, const QStringList &unsupportedElements);
    QList<QPair<QContact, QStringList> > entryContacts() const;
    void addDeletedEntryContact(const QContact &contact);
    QList<QContact> deletedEntryContacts() const;

    void addEntrySystemGroup(const QString &systemGroupId, const QString &systemGroupAtomId);
    QMap<QString, QString> entrySystemGroups() const;

    void setNextEntriesUrl(const QString &nextUrl);
    QString nextEntriesUrl() const;

    class BatchOperationResponse {
    public:
        BatchOperationResponse();
        QString operationId;
        QString type;
        QString code;
        QString reason;
        QString reasonDescription;
        QString contactGuid;
        QString eTag;
        bool isError;
    };
    void addBatchOperationResponse(const QString &operationId, BatchOperationResponse response);
    QMap<QString, BatchOperationResponse> batchOperationResponses() const;

private:
    QString mAuthorEmail;
    QString mAuthorName;
    QString mCategory;
    QString mCategorySchema;
    QString mCategoryTerm;
    QString mContributor;
    QString mGeneratorName;
    QString mGeneratorVersion;
    QString mGeneratorUri;
    QString mIcon;
    QString mId;
    QString mLink;
    QString mLogo;
    QString mRights;
    QString mSubtitle;
    QString mTitle;
    QString mUpdated;

    int mTotalResults;
    int mStartIndex;
    int mItemsPerPage;

    QMap<QString, BatchOperationResponse> mBatchOperationResponses;

    QList<QContact> mDeletedContactList;
    QList<QPair<QContact, QStringList> > mContactList;

    QMap<QString, QString> mSystemGroupAtomIds;

    QString mNextEntriesUrl;
};

#endif // GOOGLECONTACTATOM_H
