Enyo Launcher (enyo-doom)
=========

Frontend for Doom engines

Enyo Launcher is a GUI launcher for Doom engines. It is a relaunch of the gDoomsday project under a different name with more functionality. The source release is usable in any modern GNU/Linux distribution with Qt). The original release used GTK+, but was rewritten to use Qt for ease of maintenance and cross-platform compatibility.

Major development on this project has ended; I will still update and support any bugs, as well as small changes whenever I feel they are necessary.  I have provided details on the history and my decision to end major development of Enyo Launcher [here](https://gitlab.com/sdcofer70/enyo-launcher/-/wikis/Enyo-Launcher:-History-and-Future).

**Requirements**

A working Doom engine that supports Vanilla Doom commandline options, such as Chocolate Doom or zdoom. Even though it is possible to use it with custom commandline options, DEngine/Doomsday is not directly supported by this new release since it does not use standard launch parameters.  Since it now integrates its launcher with the engine, a separate launcher is no longer necessary.

A recent version of Qt6 installed, including development packages (package ending in -dev or -devel in most distro repositories), GCC (g++), GNU make, and cmake to compile from source. 

**Download**

Download the latest version using git -- instructions are shown on gitlab.  

Use this program at your own risk. I cannot claim any responsibility for any damage to your computer, your health, your pets, your plants, the environment, or to the fabric of reality. Enyo Launcher (enyo-doom) is released under the GPL version 3, which means you can do what you wish with it as long as you keep the source free. For the legally-binding description, please take a look at the GPL itself to see what you can and can't do.

**Compiling and installing**

It is recommended that you create an empty build directory under the source directory to compile Enyo Launcher. You then call cmake from this directory, like this (from the enyo-doom source directory):

     mkdir build
     cd build
     cmake ..

Any errors at this point are likely caused by missing Qt libraries and/or headers.

You will need to have the qt6 development headers installed.  I am uncertain what distro requires what package for Qt6 compilation -- Ubuntu used qt5-default for Qt5 so I'm going to guess using qt6-default; if this is not correct, please consult the Ubuntu documentation.  Enyo Launcher no longer supports Qt version 4 or 5.

By default, Enyo Launcher will be installed in /usr/local/bin and its data files in /usr/local/share/enyo-doom. To change this, pass the option -DCMAKE_INSTALL_PREFIX=/usr (or whichever path you prefer) to your cmake command.

Then simply type make to begin compiling Enyo Launcher.

When the build is finished, type make install as root to install. If you are using Ubuntu or a derivative, you will need to use sudo make install; most other systems you will need to do something like su -c 'make install'. The install should create a shortcut in the default menu structure for most desktops under Games; you can manually create a shortcut or run it from the commandline using the "enyo-doom" command.  It is possible to install under your home directory for your user only without access to root; use a path after -DCMAKE_INSTALL_PREFIX= that is in your home directory.  (Installing under ~/.local will usually add enyo-launcher in its menu/launcher, depending on your desktop and configuration, and some distros will add ~/.local/bin to your commandline PATH if it exists on login.)

**Configuration and Running Games**

You will need to point Enyo Launcher to a game WAD (the data file that contain Doom level, sound, graphic, music, etc data). Change the dropdown under the logo to the type of Doom WAD you have and use the "select file" icon after the "Game Data file" entry to locate the proper WAD for that game type. The default game types and the WAD files they use are:

Doom: doom.wad (or doom1.wad for the Shareware version) 

Doom II: Hell on Earth: doom2.wad 

TNT Evilution: tnt.wad 

The Plutonia Experiment: plutonia.wad 


You may add new game profiles using the "add" icon next to the game type drop-down menu. It will ask you the name of your new game profile (which can be anything you want) and if you want to copy the current game profile to the new one, which is useful if you want to create a game profile from an existing one and addons (see below). If your game engine supports Heretic and/or Hexen or other Doom game derivatives, you may add them in this way. Removing an unused or unwanted game can be done with the "remove" icon which will ask for confirmation.  You can use the icons to the right to move the game up and down the list and rename the currently selected game.

Extra patch WADs (pwads) or any other compatible file type (for example, pk3 files for ZDoom and others) that add levels, sounds, music, etc., can be enabled by Enyo Launcher by using the "add" icons beside the Add-ons box in the Launcher tab and removed by highlighting (clicking) the added file and clicking the "remove" icon. To return to having no extra WADs load, click the "clear" icon. In some cases, the order the PWADs are given to the engine is important for their correct functionality; you can move them up and down the list with the "move up/down" icons.  Each WAD set is specific to the game type you choose. Note that Doom 1's patch WADs will not work with Doom Shareware (doom1.wad).

The "Commandline options" input is an advanced option for each game type that allows you to specify custom commandline options to the game. This may use standard Doom options that Enyo Launcher does not provide, or engine-specific options. If you do not know what this does, it is best to leave it blank.

By default, Enyo Launcher is configured with three engines to choose from on the General tab: Chocolate Doom, prBoom, and GZDoom. The executable is the default name for each engine's executable, and is assumed to be in your system's PATH. If the name is different, or is not in a default PATH, you may change it using the "file select" icon  beside the "Engine Executable" setting. Other engines may be added using the "add" icon, and removed using the "remove" icon.

Note that since modern engines manage fullscreen and sound settings on their own gracefully now, Enyo Launcher no longer has options to set these per-engine since these options didn't always work and were useless more often than not.  Please check the documentation for the Doom engine you are using to enable/disable fullscreen or sound.

If you have a game type that requires the use of only one engine, the "Override default engine" option at the bottom of the Game tab allows you to use only the engine you select to run that game type. If the option is unchecked, the currently selected game in the Settings tab will be used.

To run a Doom engine game, select which configured game you want in the Select game profile drop-down, then click the Run button. If you have any problems running, make sure the output window is enabled (under Global Options) and view the output of the engine; it may provide a clue about what is not configured correctly. For Chocolate Doom, it is recommended that you run the chocolate-setup before you use Enyo Launcher to run it, since the majority of that engine's configuration is done there. To eliminate the information screen that pops up after you exit Chocolate Doom, untick Show ENDOOM screen in Display Configuration in chocolate-setup. In some engines there is no way to disable this option, and a jumble of control characters and text will show up at the end of the output. You can just safely ignore this.  

**Enyo?**

Enyo is the Greek goddess of war, known as Bellona in Roman mythology. She is the companion to Ares, the god of war (known as Mars in Roman mythology), and his two sons Phobos and Deimos, linking Enyo to the setting of Doom (the Martian moons of Phobos and Deimos, and Mars itself). (Information from the Wikipedia article on Enyo.) 
