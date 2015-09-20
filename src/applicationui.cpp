/*
 * Copyright (c) 2011-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

using namespace bb::cascades;

/*
 * Don't forget to add this to your pro file :
 *
 * LIBS += -lbb -lbbsystem
 * QT += network
 *
 */

ApplicationUI::ApplicationUI() :
        QObject()
{
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    // Application::instance()->setScene(root);    <---- *** This need to be commented or removed ***

    m_antiPiracy = new AntiPiracy("3c0585fdf3ac91ed8efaca040b35c9f5", "mySalt", this);

    /*
    * Uncomment if you want to know the hash to be used in the previous constructor call
    * Comment it back as soon as you have copied your hash to your constructor call.
    */
    // m_antiPiracy->printToConsoleHashOfThisKey("CellNinja");

    /*
    * Uncomment if you want to know the hash to be used in the online bypass document set in
    * the next call.
    * Comment it back as soon as you have copied your hash to your online document.
    */
    // m_antiPiracy->printToConsoleHashOfThisKey(QCoreApplication::applicationVersion());

    m_antiPiracy->checkValidityAndSetScene(root, "http://RodgerLeblanc.github.io/CellNinjaBundleWhitelistedHash.txt");
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("PiracyRemover_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}
