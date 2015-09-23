/*
 * AntiPiracy.cpp
 *
 *  Created on: 2015-09-08
 *      Author: Roger
 */

#include "AntiPiracy.h"

#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/Application>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/Label>
#include <bb/cascades/Page>
#include <bb/device/HardwareInfo>
#include <bb/PackageInfo>
#include <bb/PpsObject>
#include <bb/system/SystemUiResult>

#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QSettings>
#include <QStringList>
#include <QTimer>

#include <QtNetwork>

using namespace bb::cascades;
using namespace bb::device;
using namespace bb::system;

/*
 * Don't forget to add this to your pro file :
 *
 * LIBS += -lbb -lbbdevice -lbbsystem
 * QT += network
 *
 */

AntiPiracy::AntiPiracy(const QString& unlockKeyHash, const QString& salt, QObject *_parent) :
    QObject(),
    m_keyPrompt(NULL),
    m_wrongKeyToast(NULL),
    m_appInstalledFromBetaZone(false),
    m_appInstalledFromBBW(false),
    m_urlToBypassHash(""),
    m_sourceOfDownload(""),
    m_unlockKeyHash(unlockKeyHash)
{
    // Set parent
    if (_parent) { this->setParent(_parent); }

    // If no salt is provided, use the vendor name
    bb::PackageInfo packageInfo;
    m_salt = (salt.isEmpty() ? packageInfo.author() : salt);
}

void AntiPiracy::checkValidityAndSetScene(AbstractPane* root, QString urlToBypassHash) {
    // Save internally for future use
    if (root) { m_normalRoot = root; }
    m_urlToBypassHash = urlToBypassHash;

    // Check if the unlock key was already entered previously
    QSettings settings;
    settings.beginGroup("AntiPiracy");
    if ((settings.contains("unlockKeyEnteredByUser")) && (this->hashThis(settings.value("unlockKeyEnteredByUser", "").toString()) == m_unlockKeyHash)) {
        // User already entered the correct key previously
        m_appInstalledFromBBW = true;
    }
    else {
        // The next code will check if the app was downloaded from BBW
        QDir dir("/pps/system/installer/appdetails/");
        dir.setFilter(QDir::Files | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot | QDir::AllEntries);
        QFileInfoList fileInfoList = dir.entryInfoList();

        for (int i = 0; i < fileInfoList.size(); i++) {
            QString completePath = fileInfoList[i].path() + "/" + fileInfoList[i].fileName() + (fileInfoList[i].isDir() ? "/" : "");
            if (this->checkFile(completePath)) { break; };
        }
    }

    if ((m_appInstalledFromBBW) || (m_appInstalledFromBetaZone)) {
        // App was either installed from BBW/BetaZone or unlock key was previously entered correctly.
        // Set the normal main.qml as Scene
        Application::instance()->setScene(root);

        this->deleteLater();
    }
    else {
        /*
         * Load a custom Page constructed in C++ to make sure it can't be altered
         * The Page is only a Label and an ActivityIndicator, but it's mostly used to be the
         * scene where the unlock key prompt will be shown
         */
        Page *keyPromptPage = new Page(this);

        Container *keyPromptContainer = new Container();
        keyPromptContainer->setLayout(new DockLayout());

        Label *label = new Label();
        label->setText(tr("Checking validity, please wait..."));
        label->setHorizontalAlignment(HorizontalAlignment::Center);

        ActivityIndicator *activityIndicator = new ActivityIndicator();
        activityIndicator->setHorizontalAlignment(HorizontalAlignment::Center);
        activityIndicator->setMinHeight(300);
        activityIndicator->setMinWidth(activityIndicator->minHeight());

        Container *container = new Container();
        container->setHorizontalAlignment(HorizontalAlignment::Center);
        container->setVerticalAlignment(VerticalAlignment::Center);
        container->add(label);
        container->add(activityIndicator);

        keyPromptContainer->add(container);
        keyPromptPage->setContent(keyPromptContainer);

        Application::instance()->setScene(keyPromptPage);

        activityIndicator->start();

        /*
         * Check online for a bypass.
         * This can be used while your app is in review period to avoid having the unlock prompt
         * shown to a BBW reviewer. To construct the bypass hash, use the function
         * printToConsoleHashOfThisKey() and pass the app version you'd like to whitelist
         * as a parameter. Use the hash printed to console and put it online.
         */
        QUrl url(m_urlToBypassHash);
        QNetworkAccessManager *nam = new QNetworkAccessManager(this);
        QNetworkReply *reply = nam->get(QNetworkRequest(url));
        connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
    }
}

bool AntiPiracy::checkFile(const QString& path) {
    /*
     * This code will not be explained. You can read it and try to understand it by yourself.
     * If you want to know more, just paste this url in your mobile browser :
     * file:///pps/system/installer/appdetails/
     * and explore all the files in there.
     *
     * Basically, that's where the installation source for the app can be found.
     */
    bool appFound = false;

    bb::PpsObject ppsObject(path);
    if (ppsObject.open( bb::PpsOpenMode::Subscribe )) {
        bool readOk;
        QByteArray data = ppsObject.read(&readOk);
        if(!readOk) { return appFound; }

        bb::PackageInfo packageInfo;

        if (data.contains(packageInfo.installId().toUtf8())) {
            qDebug() << "App found:" << packageInfo.installId();
            appFound = true;

            bool decodeOk;
            const QVariantMap map = bb::PpsObject::decode(data, &decodeOk);
            if(!decodeOk) { return appFound; }

            const QVariantMap ppsFile = map["@" + path.split("/").last()].toMap();
            if (ppsFile.isEmpty()) { return appFound; }

            QMapIterator<QString, QVariant> j(ppsFile);
            while (j.hasNext()) {
                j.next();

                if (j.key() != packageInfo.installId())
                    continue;

                const QVariantMap thisApp = ppsFile[j.key()].toMap();
                QString extras = thisApp.value("extras").toString();

                QStringList allExtras = extras.split(",");
                for (int i = 0; i < allExtras.size(); i++) {
                    QStringList keyValue = allExtras[i].split("::");
                    if (!keyValue.isEmpty()) {
                        if (keyValue.first() == "source") {
                            qDebug() << keyValue;
                            m_sourceOfDownload = keyValue.last();
                            m_appInstalledFromBBW = (keyValue.last() == "AppWorld");
                            m_appInstalledFromBetaZone = (keyValue.last() == "BetaZone");
                            break;
                        }
                    }
                }
            }
        }
    }
    else {
        if (ppsObject.errorString() != "No such file or directory") {
            qDebug() << "Can't open file: " + ppsObject.errorString();
        }
    }
    ppsObject.close();

    return appFound;
}

QString AntiPiracy::hashThis(const QString& toHash) {
    /*
     * Hash a string.
     * We add a salt, target of the app (com.example.myApp) and vendor name to the hashing method
     */
    bb::PackageInfo packageInfo;
    QString appTarget = packageInfo.installId().left(packageInfo.installId().lastIndexOf("."));
    return QCryptographicHash::hash(QString(toHash + m_salt + appTarget + packageInfo.author()).toAscii(), QCryptographicHash::Md5).toHex();
}

void AntiPiracy::onKeyPromptFinished(SystemUiResult::Type type) {
    /*
     * Save the key entered by user in QSettings. That doesn't mean it's valid, but next time
     * the app is started, if the key is valid, the app will load faster.
     */
    QSettings settings;
    settings.beginGroup("AntiPiracy");
    settings.setValue("unlockKeyEnteredByUser", m_keyPrompt->inputFieldTextEntry());

    // Get the hashed version of the user's input
    QString inputAsHash = this->hashThis(m_keyPrompt->inputFieldTextEntry());

    // Set the toast with an Ok button
    if (!m_wrongKeyToast) { m_wrongKeyToast = new SystemToast(this); }
    m_wrongKeyToast->button()->setLabel(tr("Ok"));

    // Make sure toast and this object are deleted after Ok is pressed
    bool ok = connect(m_wrongKeyToast, SIGNAL(finished(bb::system::SystemUiResult::Type)), m_wrongKeyToast, SLOT(deleteLater()));
    ok = ok && connect(m_wrongKeyToast, SIGNAL(finished(bb::system::SystemUiResult::Type)), this, SLOT(deleteLater()));
    if (!ok) {
        emit errorString("Couldn't connect to SystemToast finished signal");
    }

    if ((type == SystemUiResult::ConfirmButtonSelection) && (inputAsHash == m_unlockKeyHash)) {
        // The user entered the correct key, set the normal root as scene
        if (m_normalRoot) { Application::instance()->setScene(m_normalRoot); }
        else { Application::instance()->quit(); }

        m_wrongKeyToast->setBody(tr("App has been unlocked permanently. Thanks!"));
        m_wrongKeyToast->show();
    }
    else {
        // The user key is incorrect. Prompt the user to contact developer and then force close.
        bool ok = connect(m_wrongKeyToast, SIGNAL(finished(bb::system::SystemUiResult::Type)), Application::instance(), SLOT(quit()));
        if (!ok) {
            emit errorString("Couldn't connect to SystemToast finished signal");
            Application::instance()->quit();
        }

        m_wrongKeyToast->setBody(tr("Wrong unlock key. Contact developer if you want to unlock this app."));
        m_wrongKeyToast->show();
    }

    m_keyPrompt->deleteLater();
}

void AntiPiracy::onReplyFinished() {
    /*
     * This is used to whitelist a special version of an app (ie: while it's under review).
     * You can modify it to your need if you want to whitelist something else (ie: a wifi mac address)
     */
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    /*
     * The online document should contain a list of whitelisted hash, separated by \n.
     */
    QStringList onlineWhitelistedHash;
    if (reply->error() == QNetworkReply::NoError)
        onlineWhitelistedHash = QString(reply->readAll()).split("\n");

    QString applicationVersionHash = this->hashThis(QCoreApplication::applicationVersion());
    bool applicationVersionIsWhitelisted = onlineWhitelistedHash.contains(applicationVersionHash);

    sender()->deleteLater();

    if (applicationVersionIsWhitelisted) {
        /*
         * The online hash matches, set the main.qml as scene
         *
         * We don't want to save to QSettings as the online hash might be temporary (ie: during
         * an app submission process, to make sure a BBW reviewer don't see the prompt)
         */
        if (m_normalRoot) { Application::instance()->setScene(m_normalRoot); }
        else { Application::instance()->quit(); }

        this->deleteLater();
    }
    else {
        // Show a prompt and ask for an unlock key
        if (!m_keyPrompt) { m_keyPrompt = new SystemPrompt(this); }

        bool ok = connect(m_keyPrompt, SIGNAL(finished(bb::system::SystemUiResult::Type)), this, SLOT(onKeyPromptFinished(bb::system::SystemUiResult::Type)));
        Q_ASSERT(ok);
        if (!ok) {
            emit errorString("Couldn't connect to SystemPrompt finished signal");
            this->deleteLater();
            return;
        }

        m_keyPrompt->setTitle(tr("App downloaded from: ") + m_sourceOfDownload);
        m_keyPrompt->setBody(tr("This app wasn't downloaded from BlackBerry World. Enter a key to unlock the app."));
        m_keyPrompt->show();
    }
}

void AntiPiracy::printToConsoleHashOfThisKey(const QString& keyToHash) {
    /*
     * This is only a convenient function to know what would be the hash to include in your
     * constructor call or in your online bypass document. Never let this function being called
     * in a released version or an exported as bar file version.
     */
    qDebug() << "Hash of" << keyToHash << "is" << this->hashThis(keyToHash);
}
