enyo-doom
=========

Frontend for Doom engines

![](https://i.imgur.com/2Y6Gaef.png)

Enyo-Doom is a GUI launcher for Doom engines. It is a relaunch of the gDoomsday project under a different name with more functionality. The source release is usable in any modern GNU/Linux distribution with Qt). The original release used GTK+, but was rewritten to use Qt for ease of maintenance and cross-platform compatibility.

[Development on this project has ended; please read this for details.](https://gitlab.com/sdcofer70/enyo-doom/-/wiki_pages/History-and-future)

**Requirements**

A working Doom engine that supports Vanilla Doom commandline options, such as Chocolate Doom or zdoom. Even though it is possible to use it with custom commandline options, DEngine/Doomsday is not directly supported by this new release since it does not use standard launch parameters.  Since it now integrates its launcher with the engine, a separate launcher is no longer necessary.

A recent version of Qt5 installed, including development packages (package ending in -dev or -devel in most distro repositories), GCC (g++), GNU make, and cmake to compile from source. 

**Download**

Download the latest version using git -- instructions are shown on github.  I will not make binary releases of the GNU/Linux version -- ask your distribution's package maintainers if you want a package made for your distribution.

Use any enyo-doom package at your own risk. I cannot claim any responsibility for any damage to your computer, your health, your pets, your plants, the environment, or to the fabric of reality. Enyo-Doom is released under the GPL version 2, which means you can do what you wish with it as long as you keep the source free. For the legally-binding description, please take a look at the GPL itself to see what you can and can't do.

**Compiling and installing**

It is recommended that you create an empty build directory under the source directory to compile enyo-doom. You then call cmake from this directory, like this (from the enyo-doom source directory):

     mkdir build
     cd build
     cmake ..

Any errors at this point are likely caused by missing Qt libraries and/or headers.

A note about Qt5/Qt4: enyo-doom now uses Qt5 exclusively since nearly all distributions have it as the default Qt version.  You will need to have the qt5 development headers installed.  On Ubuntu and possibly other Debian-based distros, you may need to install the package qt5-default from the repositories to select and use the qt5 build environment vs. qt4.  For other distributions, check the documentation regarding building programs using qt5 vs qt4.

By default, enyo-doom will be installed in /usr/local/bin and its data files in /usr/localshare/enyo-doom. To change this, pass the option -DCMAKE_INSTALL_PREFIX=/usr (or whichever path you prefer) to your cmake command.

Then simply type make to begin compiling enyo-doom.

When the build is finished, type make install as root to install enyo-doom. If you are using Ubuntu or a derivative, you will need to use sudo make install; most other systems you will need to do something like su -c 'make install'. The install should create a shortcut in the default menu structure for most desktops under Games; you can manually create a shortcut or run it from the commandline using the command enyo-doom.

**Configuration and Running Games**

You will need to point enyo-doom to a game WAD (the data file that contain Doom level, sound, graphic, music, etc data). Change the dropdown under the logo to the type of Doom WAD you have and use the "select file" icon after the "Game Data file" entry to locate the proper WAD for that game type. The default game types and the WAD files they use are:

Doom: doom.wad (or doom1.wad for the Shareware version) 

Doom II: Hell on Earth: doom2.wad 

TNT Evilution: tnt.wad 

The Plutonia Experiment: plutonia.wad 


You may add new game profiles using the "add" icon next to the game type drop-down menu. It will ask you the name of your new game profile (which can be anything you want) and if you want to copy the current game profile to the new one, which is useful if you want to create a game profile from an existing one and addons (see below). If your game engine supports Heretic and/or Hexen or other Doom game derivatives, you may add them in this way. Removing an unused or unwanted game can be done with the "remove" icon which will ask for confirmation.  You can move a game profile up and down with the "move up/down" icons, and rename it with the "text" icon.

Extra patch WADs (pwads) or any other compatible file type (for example, pk3 files for ZDoom and others) that add levels, sounds, music, etc., can be enabled by enyo-doom by using the "add" icons beside the Add-ons box in the Launcher tab and removed by highlighting (clicking) the added file and clicking the "remove" icon. To return to having no extra WADs load, click the "clear" icon. In some cases, the order the PWADs are given to the engine is important for their correct functionality; you can move them up and down the list with the "move up/down" icons.  Each WAD set is specific to the game type you choose. Note that Doom 1's patch WADs will not work with Doom Shareware (doom1.wad).

The "Commandline options" input is an advanced option for each game type that allows you to specify custom commandline options to the game. This may use standard Doom options that enyo-doom does not provide, or engine-specific options. If you do not know what this does, it is best to leave it blank.

By default, enyo-doom gives you three engines to choose from on the General tab: Chocolate Doom, prBoom, and GZDoom. The executable is the default name for each engine's executable, and is assumed to be in your system's PATH. If the name is different, or is not in a default PATH, you may change it using the "file select" icon  beside the "Engine Executable" setting. Other engines may be added using the "add" icon, and removed using the "remove" icon.

Each engine has an option to disable sound completely (for system compatibility purposes) and an option for fullscreen settings (Use Engine Default, Forced Windowed, and Force Fullscreen).  Some engines may ignore these settings.  NOTE: since there are several engines that ignore these settings, and most engines have the ability to configure them separately, these options may be removed in a future version of enyo-doom.  Please view the Issue on this Gitlab repository and comment if this may be a problem for you.

If you have a game type that requires the use of only one engine, the "Override default engine" option at the bottom of the Game tab allows you to use only the engine you select to run that game type. If the option is unchecked, the currently selected game in the Settings tab will be used.

To run a Doom engine game, select which configured game you want in the Select game profile drop-down, then click the Run button. If you have any problems running, make sure the output window is enabled (under Global Options) and view the output of the engine; it may provide a clue about what is not configured correctly. For Chocolate Doom, it is recommended that you run the chocolate-setup before you use enyo-doom to run it, since the majority of that engine's configuration is done there. To eliminate the information screen that pops up after you exit Chocolate Doom, untick Show ENDOOM screen in Display Configuration in chocolate-setup. In some engines there is no way to disable this option, and a jumble of control characters and text will show up at the end of the output. You can just safely ignore this.  

**Enyo?**

Enyo is the Greek goddess of war, known as Bellona in Roman mythology. She is the companion to Ares, the god of war (known as Mars in Roman mythology), and his two sons Phobos and Deimos, linking Enyo to the setting of Doom (the Martian moons of Phobos and Deimos, and Mars itself). (Information from the Wikipedia article on Enyo.) 
