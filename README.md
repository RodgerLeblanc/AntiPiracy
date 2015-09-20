Anti Piracy (beginner)
--------------
BB10 sample app that shows how to prevent your app from being pirated and used without being purchased from BlackBerry World (ie: sideloading a bar file). It might even protect from copycat developer to reuse your so file.

If the app was sideloaded, a SystemPrompt will show asking to enter an unlock key. Providing an incorrect unlock key will close the app.

Note that if an unlock key was already provided before, the code will fake like it was installed from BlackBerry World and the app will launch without asking anything.

Giving out the unlock key is not a good idea anyway, as the unlock key can be shared to other users who would be able to unlock the app. Only use unlock key for yourself or with persons you really trust. A better method would be to whitelist the user using the online method, just use something unique to that user, like his wifi mac address or something else (you'll need to update the code as it's set to whitelist an app version right now).

You can also set a bypass method using an online document. An example is provided in this sample app, it can be used to whitelist a special version of the app while it's in review process.

This code was barely tested, I can't assure that it will work in all environments and all conditions. I can't be held responsible if this code doesn't work for your app, use at your own risk.



Here's what needs to be added to your project :

1) Copy "AntiPiracy" folder in your "src" folder

2) Add this to your ".pro" file :
```sh
LIBS += -lbb -lbbsystem
QT += network
```

3) In "applicationui.hpp", add this :

```sh
#include "src/AntiPiracy/AntiPiracy.h"
// ...

private:
    AntiPiracy* m_antiPiracy;
```

4) In "applicationui.cpp", add this to your constructor :

```sh
    //...

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
```



LICENSE
--------------
The MIT License (MIT)

Copyright (c) 2015 Roger Leblanc

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


(Translation: You can altere, copy, modify, or do whatever you want with this code and you don't need to thank me or give any credits)
