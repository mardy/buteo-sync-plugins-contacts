/*
 * This file is part of buteo-gcontact-plugin package
 *
 * Copyright (C) 2015 Canonical Ltd.
 *
 * Contributors: Renato Araujo Oliveira Filho <renato.filho@canonical.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include "config-tests.h"
#include "GContactStream.h"
#include "GContactAtom.h"
#include "UContactsCustomDetail.h"

#include <QtContacts>
#include <QtCore>
#include <QtTest>

QTCONTACTS_USE_NAMESPACE

class GoogleContactParserTest : public QObject
{
    Q_OBJECT
private slots:
    void testParseFromGoogleXml()
    {
        QFile xml(TEST_DATA_DIR + QStringLiteral("google_single_entry.txt"));
        QVERIFY(xml.open(QIODevice::ReadOnly));

        GoogleContactStream parser(false);
        GoogleContactAtom *atom = parser.parse(xml.readAll());
        QVERIFY(atom);

        QList<QContact> entries = atom->deletedEntryContacts();
        QCOMPARE(entries.size(), 1);

        QContact contact = entries.at(0);
        QVERIFY(!contact.isEmpty());

        // check each contat details
        // name
        QContactName name = contact.detail<QContactName>();
        QCOMPARE(name.firstName(), QStringLiteral("Aaron"));
        QCOMPARE(name.lastName(), QStringLiteral("Rossler"));

        // TODO: implement display label support
        // QContactDisplayLabel label = contact.detail<QContactDisplayLabel>();
        // QCOMPARE(label.label(), QStringLiteral("Aaron Rossler"));

        // email
        QList<QContactEmailAddress> emails = contact.details<QContactEmailAddress>();
        QCOMPARE(emails.size(), 3);
        QCOMPARE(emails.at(0).emailAddress(), QStringLiteral("rossler@other.info"));
        QCOMPARE(emails.at(0).contexts().size(), 1);
        QVERIFY(emails.at(0).contexts().contains(QContactDetail::ContextOther));

        QCOMPARE(emails.at(1).emailAddress(), QStringLiteral("rossler@home.info"));
        QCOMPARE(emails.at(1).contexts().size(), 1);
        QVERIFY(emails.at(1).contexts().contains(QContactDetail::ContextHome));

        QCOMPARE(emails.at(2).emailAddress(), QStringLiteral("rossler@work.info"));
        QCOMPARE(emails.at(2).contexts().size(), 1);
        QVERIFY(emails.at(2).contexts().contains(QContactDetail::ContextWork));

        // phone number
        QList<QContactPhoneNumber> phones = contact.details<QContactPhoneNumber>();
        QCOMPARE(phones.size(), 7);

        // <gd:phoneNumber rel="http://schemas.google.com/g/2005#other">+55-8704-0000</gd:phoneNumber>
        QCOMPARE(phones.at(0).number(), QStringLiteral("+55-8704-0000"));
        QCOMPARE(phones.at(0).contexts().size(), 1);
        QVERIFY(phones.at(0).contexts().contains(QContactDetail::ContextOther));
        QCOMPARE(phones.at(0).subTypes().size(), 1);
        QVERIFY(phones.at(0).subTypes().contains(QContactPhoneNumber::SubTypeVoice));

        // <gd:phoneNumber rel="http://schemas.google.com/g/2005#home">+55-8704-0001</gd:phoneNumber>
        QCOMPARE(phones.at(1).number(), QStringLiteral("+55-8704-0001"));
        QCOMPARE(phones.at(1).contexts().size(), 1);
        QVERIFY(phones.at(1).contexts().contains(QContactDetail::ContextHome));
        QCOMPARE(phones.at(1).subTypes().size(), 1);
        QVERIFY(phones.at(1).subTypes().contains(QContactPhoneNumber::SubTypeVoice));

        // <gd:phoneNumber rel="http://schemas.google.com/g/2005#work">+55-8704-0002</gd:phoneNumber>
        QCOMPARE(phones.at(2).number(), QStringLiteral("+55-8704-0002"));
        QCOMPARE(phones.at(2).contexts().size(), 1);
        QVERIFY(phones.at(2).contexts().contains(QContactDetail::ContextWork));
        QCOMPARE(phones.at(2).subTypes().size(), 1);
        QVERIFY(phones.at(2).subTypes().contains(QContactPhoneNumber::SubTypeVoice));

        // <gd:phoneNumber rel="http://schemas.google.com/g/2005#work_fax">+55-8704-0003</gd:phoneNumber>
        QCOMPARE(phones.at(3).number(), QStringLiteral("+55-8704-0003"));
        QCOMPARE(phones.at(3).contexts().size(), 1);
        QVERIFY(phones.at(3).contexts().contains(QContactDetail::ContextWork));
        QCOMPARE(phones.at(3).subTypes().size(), 1);
        QVERIFY(phones.at(3).subTypes().contains(QContactPhoneNumber::SubTypeFax));

        // <gd:phoneNumber rel="http://schemas.google.com/g/2005#home_fax">+55-8704-0004</gd:phoneNumber>
        QCOMPARE(phones.at(4).number(), QStringLiteral("+55-8704-0004"));
        QCOMPARE(phones.at(4).contexts().size(), 1);
        QVERIFY(phones.at(4).contexts().contains(QContactDetail::ContextHome));
        QCOMPARE(phones.at(4).subTypes().size(), 1);
        QVERIFY(phones.at(4).subTypes().contains(QContactPhoneNumber::SubTypeFax));

        // <gd:phoneNumber rel="http://schemas.google.com/g/2005#mobile">+55-8704-0005</gd:phoneNumber>
        QCOMPARE(phones.at(5).number(), QStringLiteral("+55-8704-0005"));
        QCOMPARE(phones.at(5).contexts().size(), 0);
        QCOMPARE(phones.at(5).subTypes().size(), 1);
        QVERIFY(phones.at(5).subTypes().contains(QContactPhoneNumber::SubTypeMobile));

        // <gd:phoneNumber rel="http://schemas.google.com/g/2005#mobile_work">+55-8704-0006</gd:phoneNumber>
        QCOMPARE(phones.at(6).number(), QStringLiteral("+55-8704-0006"));
        QCOMPARE(phones.at(6).contexts().size(), 1);
        QVERIFY(phones.at(6).contexts().contains(QContactDetail::ContextWork));
        QCOMPARE(phones.at(6).subTypes().size(), 1);
        QVERIFY(phones.at(6).subTypes().contains(QContactPhoneNumber::SubTypeMobile));

        // Address
        QList<QContactAddress> address = contact.details<QContactAddress>();
        QCOMPARE(address.size(), 3);

        //<gd:structuredPostalAddress rel="http://schemas.google.com/g/2005#other">
        // <gd:formattedAddress>Other street 00
        //00000
        //Brazil</gd:formattedAddress>
        // <gd:street>Other street 00</gd:street>
        // <gd:postcode>00000</gd:postcode>
        // <gd:country>Brazil</gd:country>
        //  <gd:region>PE</gd:region>
        //  <gd:neighborhood>Other</gd:neighborhood>
        //  <gd:pobox>POBOX0</gd:pobox>
        //</gd:structuredPostalAddress>
        QCOMPARE(address.at(0).street(), QStringLiteral("Other street 00"));
        QCOMPARE(address.at(0).locality(), QStringLiteral("Other"));
        QCOMPARE(address.at(0).region(), QStringLiteral("PE"));
        QCOMPARE(address.at(0).postcode(), QStringLiteral("00000"));
        QCOMPARE(address.at(0).country(), QStringLiteral("Brazil"));
        QCOMPARE(address.at(0).postOfficeBox(), QStringLiteral("POBOX0"));
        QCOMPARE(address.at(0).contexts().size(), 1);
        QVERIFY(address.at(0).contexts().contains(QContactDetail::ContextOther));

        //<gd:structuredPostalAddress rel="http://schemas.google.com/g/2005#work">
        //  <gd:formattedAddress>Other street 01
        //11111
        //Brazil</gd:formattedAddress>
        //  <gd:street>Work street 01</gd:street>
        //  <gd:postcode>11111</gd:postcode>
        //  <gd:country>Brazil</gd:country>
        //  <gd:region>MA</gd:region>
        //  <gd:neighborhood>Work</gd:neighborhood>
        //  <gd:pobox>POBOX1</gd:pobox>
        // </gd:structuredPostalAddress>
        QCOMPARE(address.at(1).street(), QStringLiteral("Work street 01"));
        QCOMPARE(address.at(1).locality(), QStringLiteral("Work"));
        QCOMPARE(address.at(1).region(), QStringLiteral("MA"));
        QCOMPARE(address.at(1).postcode(), QStringLiteral("11111"));
        QCOMPARE(address.at(1).country(), QStringLiteral("Brazil"));
        QCOMPARE(address.at(1).postOfficeBox(), QStringLiteral("POBOX1"));
        QCOMPARE(address.at(1).contexts().size(), 1);
        QVERIFY(address.at(1).contexts().contains(QContactDetail::ContextWork));

        //<gd:structuredPostalAddress rel="http://schemas.google.com/g/2005#home">
        // <gd:formattedAddress>Other street 02
        //22222
        //Brazil</gd:formattedAddress>
        // <gd:street>Home street 02</gd:street>
        // <gd:postcode>22222</gd:postcode>
        // <gd:country>Brazil</gd:country>
        //  <gd:region>MG</gd:region>
        //  <gd:neighborhood>Home</gd:neighborhood>
        //  <gd:pobox>POBOX2</gd:pobox>
        //</gd:structuredPostalAddress>
        QCOMPARE(address.at(2).street(), QStringLiteral("Home street 02"));
        QCOMPARE(address.at(2).locality(), QStringLiteral("Home"));
        QCOMPARE(address.at(2).region(), QStringLiteral("MG"));
        QCOMPARE(address.at(2).postcode(), QStringLiteral("22222"));
        QCOMPARE(address.at(2).country(), QStringLiteral("Brazil"));
        QCOMPARE(address.at(2).postOfficeBox(), QStringLiteral("POBOX2"));
        QCOMPARE(address.at(2).contexts().size(), 1);
        QVERIFY(address.at(2).contexts().contains(QContactDetail::ContextHome));

        // Online accounts
        QList<QContactOnlineAccount> accounts = contact.details<QContactOnlineAccount>();
        QCOMPARE(accounts.size(), 10);

        //<gd:im address='Aaron@gmail.com'
        //  protocol='http://schemas.google.com/g/2005#GOOGLE_TALK'
        //  primary='true'
        //  rel='http://schemas.google.com/g/2005#home'/>
        QCOMPARE(accounts.at(0).accountUri(), QStringLiteral("Aaron@gmail.com"));
        QCOMPARE(accounts.at(0).serviceProvider(), QStringLiteral(""));
        QCOMPARE(accounts.at(0).protocol(), QContactOnlineAccount::ProtocolJabber);
        QVERIFY(accounts.at(0).capabilities().isEmpty());
        QVERIFY(accounts.at(0).subTypes().isEmpty());
        QCOMPARE(accounts.at(0).contexts().size(), 1);
        QVERIFY(accounts.at(0).contexts().contains(QContactDetail::ContextHome));


        //<gd:im address='Aaron@aim.com'
        //  protocol='http://schemas.google.com/g/2005#AIM'
        //  rel='http://schemas.google.com/g/2005#work'/>
        QCOMPARE(accounts.at(1).accountUri(), QStringLiteral("Aaron@aim.com"));
        QCOMPARE(accounts.at(1).serviceProvider(), QStringLiteral(""));
        QCOMPARE(accounts.at(1).protocol(), QContactOnlineAccount::ProtocolAim);
        QVERIFY(accounts.at(1).capabilities().isEmpty());
        QVERIFY(accounts.at(1).subTypes().isEmpty());
        QCOMPARE(accounts.at(1).contexts().size(), 1);
        QVERIFY(accounts.at(1).contexts().contains(QContactDetail::ContextWork));

        //<gd:im address='Aaron@msn.com'
        //  protocol='http://schemas.google.com/g/2005#MSN'
        //  rel='http://schemas.google.com/g/2005#work'/>
        QCOMPARE(accounts.at(2).accountUri(), QStringLiteral("Aaron@msn.com"));
        QCOMPARE(accounts.at(2).serviceProvider(), QStringLiteral(""));
        QCOMPARE(accounts.at(2).protocol(), QContactOnlineAccount::ProtocolMsn);
        QVERIFY(accounts.at(2).capabilities().isEmpty());
        QVERIFY(accounts.at(2).subTypes().isEmpty());
        QCOMPARE(accounts.at(2).contexts().size(), 1);
        QVERIFY(accounts.at(2).contexts().contains(QContactDetail::ContextWork));

        //<gd:im address='Aaron@yahoo.com'
        //  protocol='http://schemas.google.com/g/2005#YAHOO'
        //  rel='http://schemas.google.com/g/2005#other'/>
        QCOMPARE(accounts.at(3).accountUri(), QStringLiteral("Aaron@yahoo.com"));
        QCOMPARE(accounts.at(3).serviceProvider(), QStringLiteral(""));
        QCOMPARE(accounts.at(3).protocol(), QContactOnlineAccount::ProtocolYahoo);
        QVERIFY(accounts.at(3).capabilities().isEmpty());
        QVERIFY(accounts.at(3).subTypes().isEmpty());
        QCOMPARE(accounts.at(3).contexts().size(), 1);
        QVERIFY(accounts.at(3).contexts().contains(QContactDetail::ContextOther));

        //<gd:im address='Aaron@skype.com'
        //  protocol='http://schemas.google.com/g/2005#SKYPE'
        //  rel='http://schemas.google.com/g/2005#other'/>
        QCOMPARE(accounts.at(4).accountUri(), QStringLiteral("Aaron@skype.com"));
        QCOMPARE(accounts.at(4).serviceProvider(), QStringLiteral(""));
        QCOMPARE(accounts.at(4).protocol(), QContactOnlineAccount::ProtocolSkype);
        QVERIFY(accounts.at(4).capabilities().isEmpty());
        QVERIFY(accounts.at(4).subTypes().isEmpty());
        QCOMPARE(accounts.at(4).contexts().size(), 1);
        QVERIFY(accounts.at(4).contexts().contains(QContactDetail::ContextOther));

        //<gd:im address='Aaron@qq.com'
        //  protocol='http://schemas.google.com/g/2005#QQ'
        //  rel='http://schemas.google.com/g/2005#other'/>
        QCOMPARE(accounts.at(5).accountUri(), QStringLiteral("Aaron@qq.com"));
        QCOMPARE(accounts.at(5).serviceProvider(), QStringLiteral(""));
        QCOMPARE(accounts.at(5).protocol(), QContactOnlineAccount::ProtocolQq);
        QVERIFY(accounts.at(5).capabilities().isEmpty());
        QVERIFY(accounts.at(5).subTypes().isEmpty());
        QCOMPARE(accounts.at(5).contexts().size(), 1);
        QVERIFY(accounts.at(5).contexts().contains(QContactDetail::ContextOther));

        //<gd:im address='Aaron@icq.com'
        //  protocol='http://schemas.google.com/g/2005#ICQ'
        //  rel='http://schemas.google.com/g/2005#other'/>
        QCOMPARE(accounts.at(6).accountUri(), QStringLiteral("Aaron@icq.com"));
        QCOMPARE(accounts.at(6).serviceProvider(), QStringLiteral(""));
        QCOMPARE(accounts.at(6).protocol(), QContactOnlineAccount::ProtocolIcq);
        QVERIFY(accounts.at(6).capabilities().isEmpty());
        QVERIFY(accounts.at(6).subTypes().isEmpty());
        QCOMPARE(accounts.at(6).contexts().size(), 1);
        QVERIFY(accounts.at(6).contexts().contains(QContactDetail::ContextOther));

        //<gd:im address='Aaron@jabber.com'
        //  protocol='http://schemas.google.com/g/2005#JABBER'
        //  rel='http://schemas.google.com/g/2005#home'/>
        QCOMPARE(accounts.at(7).accountUri(), QStringLiteral("Aaron@jabber.com"));
        QCOMPARE(accounts.at(7).serviceProvider(), QStringLiteral(""));
        QCOMPARE(accounts.at(7).protocol(), QContactOnlineAccount::ProtocolJabber);
        QVERIFY(accounts.at(7).capabilities().isEmpty());
        QVERIFY(accounts.at(7).subTypes().isEmpty());
        QCOMPARE(accounts.at(7).contexts().size(), 1);
        QVERIFY(accounts.at(7).contexts().contains(QContactDetail::ContextHome));

        //<gd:im address='Aaron@irc.com'
        //  protocol='http://schemas.google.com/g/2005#IRC'
        //  rel='http://schemas.google.com/g/2005#home'/>
        QCOMPARE(accounts.at(8).accountUri(), QStringLiteral("Aaron@irc.com"));
        QCOMPARE(accounts.at(8).serviceProvider(), QStringLiteral(""));
        QCOMPARE(accounts.at(8).protocol(), QContactOnlineAccount::ProtocolIrc);
        QVERIFY(accounts.at(8).capabilities().isEmpty());
        QVERIFY(accounts.at(8).subTypes().isEmpty());
        QCOMPARE(accounts.at(8).contexts().size(), 1);
        QVERIFY(accounts.at(8).contexts().contains(QContactDetail::ContextHome));

        //<gd:im address='Aaron@telegram.com'
        //  protocol='http://schemas.google.com/g/2005#TELEGRAM'
        //  rel='http://schemas.google.com/g/2005#home'/>
        QCOMPARE(accounts.at(9).accountUri(), QStringLiteral("Aaron@telegram.com"));
        QCOMPARE(accounts.at(9).serviceProvider(), QStringLiteral("TELEGRAM"));
        QCOMPARE(accounts.at(9).protocol(), QContactOnlineAccount::ProtocolUnknown);
        QVERIFY(accounts.at(9).capabilities().isEmpty());
        QVERIFY(accounts.at(9).subTypes().isEmpty());
        QCOMPARE(accounts.at(9).contexts().size(), 1);
        QVERIFY(accounts.at(9).contexts().contains(QContactDetail::ContextHome));

        // QContactOrganization
        QList<QContactOrganization> orgs = contact.details<QContactOrganization>();
        QCOMPARE(orgs.size(), 2);
        //<gd:organization rel="http://schemas.google.com/g/2005#work" label="Work" primary="true"/>
        //  <gd:orgName>Google, Inc.</gd:orgName>
        //  <gd:orgTitle>Tech Writer</gd:orgTitle>
        //  <gd:orgJobDescription>Writes documentation</gd:orgJobDescription>
        //  <gd:orgDepartment>Software Development</gd:orgDepartment>
        //  <gd:orgSymbol>GOOG</gd:orgSymbol>
        //</gd:organization>
        QCOMPARE(orgs.at(0).name(), QStringLiteral("Google, Inc."));
        QCOMPARE(orgs.at(0).logoUrl().toString(), QStringLiteral("GOOG"));
        QCOMPARE(orgs.at(0).department().size(), 1);
        QCOMPARE(orgs.at(0).department().first(), QStringLiteral("Software Development"));
        QCOMPARE(orgs.at(0).location(), QStringLiteral(""));
        QCOMPARE(orgs.at(0).role(), QStringLiteral("Writes documentation"));
        QCOMPARE(orgs.at(0).title(), QStringLiteral("Tech Writer"));
        QCOMPARE(orgs.at(0).assistantName(), QStringLiteral(""));
        QCOMPARE(orgs.at(0).contexts().size(), 1);
        QVERIFY(orgs.at(0).contexts().contains(QContactDetail::ContextWork));

        //<gd:organization rel="http://schemas.google.com/g/2005#home" label="Home""/>
        //  <gd:orgName>My house</gd:orgName>
        //  <gd:orgTitle>cleaner</gd:orgTitle>
        //  <gd:orgJobDescription>Clean the house</gd:orgJobDescription>
        //  <gd:orgDepartment>Room</gd:orgDepartment>
        //  <gd:orgSymbol>HOUSE</gd:orgSymbol>
        //</gd:organization>
        QCOMPARE(orgs.at(1).name(), QStringLiteral("My house"));
        QCOMPARE(orgs.at(1).logoUrl().toString(), QStringLiteral("HOUSE"));
        QCOMPARE(orgs.at(1).department().size(), 1);
        QCOMPARE(orgs.at(1).department().first(), QStringLiteral("Room"));
        QCOMPARE(orgs.at(1).location(), QStringLiteral(""));
        QCOMPARE(orgs.at(1).role(), QStringLiteral("Clean the house"));
        QCOMPARE(orgs.at(1).title(), QStringLiteral("cleaner"));
        QCOMPARE(orgs.at(1).assistantName(), QStringLiteral(""));
        QCOMPARE(orgs.at(1).contexts().size(), 1);
        QVERIFY(orgs.at(1).contexts().contains(QContactDetail::ContextHome));

        //<link rel="http://schemas.google.com/contacts/2008/rel#photo" type="image/*" href="https://www.google.com/m8/feeds/photos/media/test%40gmail.com/948ac108f03db45"/>
        QList<QContactAvatar> avatars = contact.details<QContactAvatar>();
        QString avatarETag = UContactsCustomDetail::getCustomField(contact, UContactsCustomDetail::FieldContactAvatarETag).data().toString();
        QCOMPARE(avatars.size(), 1);
        QCOMPARE(avatars.at(0).imageUrl(), QUrl("https://www.google.com/m8/feeds/photos/media/test%40gmail.com/948ac108f03db45"));
        QCOMPARE(avatarETag, QStringLiteral("\"eh19bkUofCt7I2BaHVJTdw9tEH0ifl8lcyg.\""));

        //<gd:extendedProperty name="X-FAVORITE" value="true"/>
        QList<QContactFavorite> favorites = contact.details<QContactFavorite>();
        QCOMPARE(favorites.size(), 1);
        QCOMPARE(favorites.at(0).isFavorite(), true);

        //<gd:extendedProperty name="X-PROPERTY" value="property-value"/>
        QContactExtendedDetail xProperty = UContactsCustomDetail::getCustomField(contact,
                                                                                 QStringLiteral("X-PROPERTY"));
        QCOMPARE(xProperty.name(), QStringLiteral("X-PROPERTY"));
        QCOMPARE(xProperty.data().toString(), QStringLiteral("property-value"));

        //<gd:deleted/>
        QContactExtendedDetail ex = UContactsCustomDetail::getCustomField(contact, UContactsCustomDetail::FieldDeletedAt);
        QVERIFY(ex.data().toDateTime().isValid());

        //TODO: check for missing fields
        //TypeAnniversary,
        //TypeBirthday,
        //TypeFamily,
        //TypeGender,
        //TypeGeoLocation,
        //TypeGlobalPresence,
        //TypeGuid,
        //TypeHobby,
        //TypeNickname,
        //TypeNote,
        //TypePresence,
        //TypeRingtone,
        //TypeSyncTarget,
        //TypeTag,
        //TypeType,
        //TypeUrl,
    }

    void testParseToGoogleXml()
    {
        QStringList expectedXML;
        QContact contact;

        // name
        QContactName cName;
        cName.setFirstName(QStringLiteral("Aaron"));
        cName.setLastName(QStringLiteral("Rossler"));
        contact.saveDetail(&cName);
        expectedXML << QStringLiteral("<gd:name><gd:givenName>Aaron</gd:givenName><gd:familyName>Rossler</gd:familyName></gd:name>");

        // email
        QContactEmailAddress email;
        email.setEmailAddress(QStringLiteral("rossler@other.info"));
        email.setContexts(QList<int>() << QContactDetail::ContextOther);
        contact.saveDetail(&email);
        expectedXML << QStringLiteral("<gd:email rel=\"http://schemas.google.com/g/2005#other\" address=\"rossler@other.info\"/>");

        email = QContactEmailAddress();
        email.setEmailAddress(QStringLiteral("rossler@home.info"));
        email.setContexts(QList<int>() << QContactDetail::ContextHome);
        contact.saveDetail(&email);
        expectedXML << QStringLiteral("<gd:email rel=\"http://schemas.google.com/g/2005#home\" address=\"rossler@home.info\"/>");

        email = QContactEmailAddress();
        email.setEmailAddress(QStringLiteral("rossler@work.info"));
        email.setContexts(QList<int>() << QContactDetail::ContextWork);
        contact.saveDetail(&email);
        expectedXML << QStringLiteral("<gd:email rel=\"http://schemas.google.com/g/2005#work\" address=\"rossler@work.info\"/>");

        // phone number
        QContactPhoneNumber phone;

        phone.setNumber(QStringLiteral("+55-8704-0000"));
        phone.setContexts(QList<int>() << QContactDetail::ContextOther);
        phone.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeVoice);
        contact.saveDetail(&phone);
        expectedXML << QStringLiteral("<gd:phoneNumber rel=\"http://schemas.google.com/g/2005#other\">+55-8704-0000</gd:phoneNumber>");

        phone = QContactPhoneNumber();
        phone.setNumber(QStringLiteral("+55-8704-0001"));
        phone.setContexts(QList<int>() << QContactDetail::ContextHome);
        phone.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeVoice);
        contact.saveDetail(&phone);
        expectedXML << QStringLiteral("<gd:phoneNumber rel=\"http://schemas.google.com/g/2005#home\">+55-8704-0001</gd:phoneNumber>");

        phone = QContactPhoneNumber();
        phone.setNumber(QStringLiteral("+55-8704-0002"));
        phone.setContexts(QList<int>() << QContactDetail::ContextWork);
        phone.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeVoice);
        contact.saveDetail(&phone);
        expectedXML << QStringLiteral("<gd:phoneNumber rel=\"http://schemas.google.com/g/2005#work\">+55-8704-0002</gd:phoneNumber>");

        phone = QContactPhoneNumber();
        phone.setNumber(QStringLiteral("+55-8704-0003"));
        phone.setContexts(QList<int>() << QContactDetail::ContextWork);
        phone.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeFax);
        contact.saveDetail(&phone);
        expectedXML << QStringLiteral("<gd:phoneNumber rel=\"http://schemas.google.com/g/2005#work_fax\">+55-8704-0003</gd:phoneNumber>");

        phone = QContactPhoneNumber();
        phone.setNumber(QStringLiteral("+55-8704-0004"));
        phone.setContexts(QList<int>() << QContactDetail::ContextHome);
        phone.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeFax);
        contact.saveDetail(&phone);
        expectedXML << QStringLiteral("<gd:phoneNumber rel=\"http://schemas.google.com/g/2005#home_fax\">+55-8704-0004</gd:phoneNumber>");

        phone = QContactPhoneNumber();
        phone.setNumber(QStringLiteral("+55-8704-0005"));
        phone.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeMobile);
        contact.saveDetail(&phone);
        expectedXML << QStringLiteral("<gd:phoneNumber rel=\"http://schemas.google.com/g/2005#mobile\">+55-8704-0005</gd:phoneNumber>");

        phone = QContactPhoneNumber();
        phone.setNumber(QStringLiteral("+55-8704-0006"));
        phone.setContexts(QList<int>() << QContactDetail::ContextWork);
        phone.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeMobile);
        contact.saveDetail(&phone);
        expectedXML << QStringLiteral("<gd:phoneNumber rel=\"http://schemas.google.com/g/2005#work_mobile\">+55-8704-0006</gd:phoneNumber>");

        // Address
        QContactAddress address;

        address.setStreet(QStringLiteral("Other street 00"));
        address.setLocality(QStringLiteral("Other"));
        address.setRegion(QStringLiteral("PE"));
        address.setPostcode(QStringLiteral("00000"));
        address.setCountry(QStringLiteral("Brazil"));
        address.setPostOfficeBox(QStringLiteral("POBOX0"));
        contact.saveDetail(&address);
        expectedXML << QStringLiteral("<gd:structuredPostalAddress rel=\"http://schemas.google.com/g/2005#other\">"
                                      "<gd:street>Other street 00</gd:street>"
                                      "<gd:neighborhood>Other</gd:neighborhood>"
                                      "<gd:pobox>POBOX0</gd:pobox>"
                                      "<gd:region>PE</gd:region>"
                                      "<gd:postcode>00000</gd:postcode>"
                                      "<gd:country>Brazil</gd:country>"
                                      "</gd:structuredPostalAddress>");


        address = QContactAddress();
        address.setStreet(QStringLiteral("Work street 01"));
        address.setLocality(QStringLiteral("Work"));
        address.setRegion(QStringLiteral("MA"));
        address.setPostcode(QStringLiteral("11111"));
        address.setCountry(QStringLiteral("Brazil"));
        address.setPostOfficeBox(QStringLiteral("POBOX1"));
        address.setContexts(QList<int>() << QContactDetail::ContextWork);
        contact.saveDetail(&address);
        expectedXML << QStringLiteral("<gd:structuredPostalAddress rel=\"http://schemas.google.com/g/2005#work\">"
                                      "<gd:street>Work street 01</gd:street>"
                                      "<gd:neighborhood>Work</gd:neighborhood>"
                                      "<gd:pobox>POBOX1</gd:pobox>"
                                      "<gd:region>MA</gd:region>"
                                      "<gd:postcode>11111</gd:postcode>"
                                      "<gd:country>Brazil</gd:country>"
                                      "</gd:structuredPostalAddress>");


        address = QContactAddress();
        address.setStreet(QStringLiteral("Home street 02"));
        address.setLocality(QStringLiteral("Home"));
        address.setRegion(QStringLiteral("MG"));
        address.setPostcode(QStringLiteral("22222"));
        address.setCountry(QStringLiteral("Brazil"));
        address.setPostOfficeBox(QStringLiteral("POBOX2"));
        address.setContexts(QList<int>() << QContactDetail::ContextHome);
        contact.saveDetail(&address);
        expectedXML << QStringLiteral("<gd:structuredPostalAddress rel=\"http://schemas.google.com/g/2005#home\">"
                                      "<gd:street>Home street 02</gd:street>"
                                      "<gd:neighborhood>Home</gd:neighborhood>"
                                      "<gd:pobox>POBOX2</gd:pobox>"
                                      "<gd:region>MG</gd:region>"
                                      "<gd:postcode>22222</gd:postcode>"
                                      "<gd:country>Brazil</gd:country>"
                                      "</gd:structuredPostalAddress>");

        // Online accounts
        QContactOnlineAccount account;

        account.setAccountUri(QStringLiteral("Aaron@gmail.com"));
        account.setProtocol(QContactOnlineAccount::ProtocolJabber);
        account.setContexts(QList<int>() << QContactDetail::ContextHome);
        contact.saveDetail(&account);
        expectedXML << QStringLiteral("<gd:im protocol=\"http://schemas.google.com/g/2005#JABBER\""
                                      " rel=\"http://schemas.google.com/g/2005#home\""
                                      " address=\"Aaron@gmail.com\"/>");

        account = QContactOnlineAccount();
        account.setAccountUri(QStringLiteral("Aaron@aim.com"));
        account.setProtocol(QContactOnlineAccount::ProtocolAim);
        account.setContexts(QList<int>() << QContactDetail::ContextWork);
        contact.saveDetail(&account);
        expectedXML << QStringLiteral("<gd:im protocol=\"http://schemas.google.com/g/2005#AIM\""
                                      " rel=\"http://schemas.google.com/g/2005#work\""
                                      " address=\"Aaron@aim.com\"/>");

        account = QContactOnlineAccount();
        account.setAccountUri(QStringLiteral("Aaron@msn.com"));
        account.setProtocol(QContactOnlineAccount::ProtocolMsn);
        account.setContexts(QList<int>() << QContactDetail::ContextWork);
        contact.saveDetail(&account);
        expectedXML << QStringLiteral("<gd:im protocol=\"http://schemas.google.com/g/2005#MSN\""
                                      " rel=\"http://schemas.google.com/g/2005#work\""
                                      " address=\"Aaron@msn.com\"/>");

        account = QContactOnlineAccount();
        account.setAccountUri(QStringLiteral("Aaron@yahoo.com"));
        account.setProtocol(QContactOnlineAccount::ProtocolYahoo);
        account.setContexts(QList<int>() << QContactDetail::ContextOther);
        contact.saveDetail(&account);
        expectedXML << QStringLiteral("<gd:im protocol=\"http://schemas.google.com/g/2005#YAHOO\""
                                      " rel=\"http://schemas.google.com/g/2005#other\""
                                      " address=\"Aaron@yahoo.com\"/>");

        account = QContactOnlineAccount();
        account.setAccountUri(QStringLiteral("Aaron@skype.com"));
        account.setProtocol(QContactOnlineAccount::ProtocolSkype);
        account.setContexts(QList<int>() << QContactDetail::ContextOther);
        contact.saveDetail(&account);
        expectedXML << QStringLiteral("<gd:im protocol=\"http://schemas.google.com/g/2005#SKYPE\""
                                      " rel=\"http://schemas.google.com/g/2005#other\""
                                      " address=\"Aaron@skype.com\"/>");

        account = QContactOnlineAccount();
        account.setAccountUri(QStringLiteral("Aaron@qq.com"));
        account.setProtocol(QContactOnlineAccount::ProtocolQq);
        account.setContexts(QList<int>() << QContactDetail::ContextOther);
        contact.saveDetail(&account);
        expectedXML << QStringLiteral("<gd:im protocol=\"http://schemas.google.com/g/2005#QQ\""
                                      " rel=\"http://schemas.google.com/g/2005#other\""
                                      " address=\"Aaron@qq.com\"/>");

        account = QContactOnlineAccount();
        account.setAccountUri(QStringLiteral("Aaron@icq.com"));
        account.setProtocol(QContactOnlineAccount::ProtocolIcq);
        account.setContexts(QList<int>() << QContactDetail::ContextOther);
        contact.saveDetail(&account);
        expectedXML << QStringLiteral("<gd:im protocol=\"http://schemas.google.com/g/2005#ICQ\""
                                      " rel=\"http://schemas.google.com/g/2005#other\""
                                      " address=\"Aaron@icq.com\"/>");

        account = QContactOnlineAccount();
        account.setAccountUri(QStringLiteral("Aaron@jabber.com"));
        account.setProtocol(QContactOnlineAccount::ProtocolJabber);
        account.setContexts(QList<int>() << QContactDetail::ContextHome);
        contact.saveDetail(&account);
        expectedXML << QStringLiteral("<gd:im protocol=\"http://schemas.google.com/g/2005#JABBER\""
                                      " rel=\"http://schemas.google.com/g/2005#home\""
                                      " address=\"Aaron@jabber.com\"/>");

        account = QContactOnlineAccount();
        account.setAccountUri(QStringLiteral("Aaron@irc.com"));
        account.setProtocol(QContactOnlineAccount::ProtocolIrc);
        account.setContexts(QList<int>() << QContactDetail::ContextHome);
        contact.saveDetail(&account);
        expectedXML << QStringLiteral("<gd:im protocol=\"http://schemas.google.com/g/2005#IRC\""
                                      " rel=\"http://schemas.google.com/g/2005#home\""
                                      " address=\"Aaron@irc.com\"/>");

        account = QContactOnlineAccount();
        account.setAccountUri(QStringLiteral("Aaron@telegram.com"));
        account.setProtocol(QContactOnlineAccount::ProtocolUnknown);
        account.setContexts(QList<int>() << QContactDetail::ContextHome);
        account.setServiceProvider(QStringLiteral("TELEGRAM"));
        contact.saveDetail(&account);
        expectedXML << QStringLiteral("<gd:im protocol=\"http://schemas.google.com/g/2005#TELEGRAM\""
                                      " rel=\"http://schemas.google.com/g/2005#home\""
                                      " address=\"Aaron@telegram.com\"/>");

        // Organization
        QContactOrganization  org;

        org.setName(QStringLiteral("Google, Inc."));
        org.setDepartment(QStringList() <<  QStringLiteral("Software Development"));
        org.setRole(QStringLiteral("Writes documentation"));
        org.setTitle(QStringLiteral("Tech Writer"));
        org.setContexts(QList<int>() << QContactDetail::ContextWork);
        contact.saveDetail(&org);
        expectedXML << QStringLiteral("<gd:organization rel=\"http://schemas.google.com/g/2005#work\">"
                                      "<gd:orgTitle>Tech Writer</gd:orgTitle>"
                                      "<gd:orgName>Google, Inc.</gd:orgName>"
                                      "<gd:orgDepartment>Software Development</gd:orgDepartment>"
                                      "</gd:organization>");

        org = QContactOrganization();
        org.setName(QStringLiteral("My house"));
        org.setDepartment(QStringList() <<  QStringLiteral("Room"));
        org.setRole(QStringLiteral("Clean the house"));
        org.setTitle(QStringLiteral("cleaner"));
        org.setContexts(QList<int>() << QContactDetail::ContextHome);
        contact.saveDetail(&org);
        expectedXML << QStringLiteral("<gd:organization rel=\"http://schemas.google.com/g/2005#home\">"
                                      "<gd:orgTitle>cleaner</gd:orgTitle>"
                                      "<gd:orgName>My house</gd:orgName>"
                                      "<gd:orgDepartment>Room</gd:orgDepartment>"
                                      "</gd:organization>");

        // Extended Details
        QContactExtendedDetail xDet;
        xDet.setName("X-PROPERTY");
        xDet.setData("x-property-value");
        contact.saveDetail(&xDet);
        expectedXML << QStringLiteral("<gd:extendedProperty name=\"X-PROPERTY\" value=\"x-property-value\"/>");

        // Favorite
        QContactFavorite fav;
        fav.setFavorite(true);
        contact.saveDetail(&fav);
        expectedXML << QStringLiteral("<gd:extendedProperty name=\"X-FAVORITE\" value=\"true\"/>");

        // Anniversay
        QContactAnniversary aniversary;
        aniversary.setOriginalDateTime(QDateTime(QDate(2015, 10, 4), QTime(12, 30)));
        aniversary.setSubType(QContactAnniversary::SubTypeWedding);
        aniversary.setEvent("My wedding");
        contact.saveDetail(&aniversary);
        expectedXML << QStringLiteral("<gContact:event rel=\"anniversary\" label=\"wedding\"><gd:when startTime=\"2015-10-04T12:30:00\"/>");

        aniversary = QContactAnniversary();
        aniversary.setOriginalDate(QDate(2015, 3, 20));
        aniversary.setSubType(QContactAnniversary::SubTypeHouse);
        aniversary.setEvent("My house event");
        contact.saveDetail(&aniversary);
        expectedXML << QStringLiteral("<gContact:event rel=\"anniversary\" label=\"house\"><gd:when startTime=\"2015-03-20T00:00:00\"/>");

        // Birthday
        QContactBirthday birthday;
        birthday.setDateTime(QDateTime(QDate(2015, 11, 4), QTime(12, 30)));
        contact.saveDetail(&birthday);
        expectedXML << QStringLiteral("<gContact:birthday when=\"2015-11-04\"/>");

// FIXME: QContacts api does not support birthday without year
//        birthday = QContactBirthday();
//        birthday.setDate(QDate(0, 4, 23));
//        contact.saveDetail(&birthday);
//        expectedXML << QStringLiteral("<gContact:birthday when=\"--04-23\"/>");

        // Gender
        QContactGender gender;
        gender.setGender(QContactGender::GenderMale);
        contact.saveDetail(&gender);
        expectedXML << QStringLiteral("<gContact:gender value=\"male\"/>");

        // Note
        QContactNote note;
        note.setNote("This is a note");
        contact.saveDetail(&note);
        expectedXML << QStringLiteral("<gContact:jot>This is a note</gContact:jot>");


        //TODO: check for missing fields
        //TypeAvatar,
        //TypeFamily,
        //TypeGeoLocation,
        //TypeGlobalPresence,
        //TypeGuid,
        //TypeHobby,
        //TypeNickname,
        //TypeNote,
        //TypePresence,
        //TypeRingtone,
        //TypeSyncTarget,
        //TypeTag,
        //TypeTimestamp,
        //TypeType,
        //TypeUrl,
        //TypeVersion

        QMultiMap<GoogleContactStream::UpdateType, QPair<QContact, QStringList> > batchPage;
        batchPage.insertMulti(GoogleContactStream::Add,
                              qMakePair(contact, QStringList()));

        GoogleContactStream encoder(false, QStringLiteral("test@gmail.com"));
        QByteArray xml = encoder.encode(batchPage);
        qDebug() << "XML" << xml;
        foreach(QString line, expectedXML) {
            QVERIFY2(xml.contains(line.toUtf8()), qPrintable("Invalid parse for:" + line));
        }
    }
};

QTEST_MAIN(GoogleContactParserTest)

#include "TestGoogleContactParser.moc"
