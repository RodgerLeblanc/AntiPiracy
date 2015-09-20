/*
 * AntiPiracy.h
 *
 *  Created on: 2015-09-08
 *      Author: Roger
 */

#ifndef ANTIPIRACY_H_
#define ANTIPIRACY_H_

#include <QObject>
#include <bb/cascades/AbstractPane>
#include <bb/system/SystemPrompt>
#include <bb/system/SystemToast>

class AntiPiracy : public QObject
{
    Q_OBJECT

public:
    /*
     * Constructor
     *
     * unlockKeyHash is mandatory, it is the hash version of the unlock key that you can
     * retrieve using printToConsoleHashOfThisKey() function, make sure to only use this
     * function for yourself and remove the call to this function before releasing.
     *
     * salt is mandatory, it doesn't need to be hidden to a potential thief, but you can
     * try to hide it if you want.
     *
     * _parent isn't mandatory
     *
     */
    AntiPiracy(const QString& unlockKeyHash, const QString& salt, QObject *_parent = 0);

    /*
     * Destructor
     */
    virtual ~AntiPiracy() {}


    /*
     * Check the validity of the app and set the correct scene.
     * If the app is legit, root will be used as scene.
     * If not, a custom Page will be used to ask the user for an unlock key
     */
    void checkValidityAndSetScene(bb::cascades::AbstractPane* root, QString urlToBypassHash = "");

    /*
     * This function is only a helper for yourself and should never be called publically (in
     * a released version or an exported as bar file version).
     * It's usually only needed once to know what is the hash of an unlock key you have in your
     * mind. The unlock key is never stored in the app, you have to remember it yourself or
     * comment it somewhere in your code, so that it doesn't appear with an hex editor.
     */
    void printToConsoleHashOfThisKey(const QString& keyToHash);

private Q_SLOTS:
    /*
     * Slot called after the SystemPrompt is finished
     */
    void onKeyPromptFinished(bb::system::SystemUiResult::Type);

private slots:
    /*
     * Slot called when retrieving an online bypass code
     */
    void onReplyFinished();

private:
    /*
     * Check a file at *path* to see if this app install details are found
     */
    bool checkFile(const QString& path);

    /*
     * Return the hash version of the string
     */
    QString hashThis(const QString& toHash);

    /*
     * Object pointers
     */
    bb::cascades::AbstractPane* m_normalRoot;
    bb::system::SystemPrompt* m_keyPrompt;
    bb::system::SystemToast* m_wrongKeyToast;

    /*
     * Variables
     */
    bool m_appInstalledFromBBW;
    QString m_urlToBypassHash;
    QString m_salt;
    QString m_sourceOfDownload;
    QString m_unlockKeyHash;

signals:
    /*
     * Signal used to return error messages
     */
    void errorString(const QString&);
};

#endif /* ANTIPIRACY_H_ */
