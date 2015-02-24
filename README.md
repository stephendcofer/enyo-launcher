enyo-doom
=========

Frontend for Doom engines

Enyo-Doom is a GUI launcher for Doom engines. It is a relaunch of the gDoomsday project under a different name with more functionality. The source release is usable in any modern GNU/Linux distribution with Qt, and I have created a port that runs under Windows as well (see below for details). The original release used GTK+, but was rewritten to use Qt for ease of maintenance and cross-platform compatibility.

Requirements

A working Doom engine that supports Vanilla Doom commandline options, such as Chocolate Doom or zdoom. Even though it is possible to use it with custom commandline options, DEngine/Doomsday is not directly supported by this new release since it does not use standard launch parameters -- use its Snowberry launcher instead. 

A recent version of Qt5 installed, including development packages (package ending in -dev or -devel in most distro repositories), GCC (g++), GNU make, and cmake to compile from source. 

**Qt4 IS NOW DEPRECIATED.**  

Download

Download the latest version using git -- instructions are shown on github.  I will not make binary releases of the GNU/Linux version -- ask your distribution's package maintainers if you want a package made for your distribution.

Use any enyo-doom package at your own risk. I cannot claim any responsibility for any damage to your computer, your health, your pets, your plants, the environment, or to the fabric of reality. Enyo-Doom is released under the GPL version 2, which means you can do what you wish with it as long as you keep the source free. For the legally-binding description, please take a look at the GPL itself to see what you can and can't do.

Compiling and installing

It is recommended that you create an empty build directory under the source directory to compile enyo-doom. You then call cmake from this directory, like this (from the enyo-doom source directory):

     mkdir build
     cd build
     cmake ..

Any errors at this point are likely caused by missing Qt libraries and/or headers.

A note about Qt5/Qt4: enyo-doom now uses Qt5 exclusively since pretty much all distributions now have it as an option.  You will need to have the qt5 development headers installed.  On Ubuntu and possibly other Debian-based distros, you may need to install the package qt5-default from the repositories to select and use the qt5 build environment vs. qt4.  For other distributions, check the documentation regarding building programs using qt5 vs qt4.

By default, enyo-doom will be installed in /usr/local/bin and its data files in /usr/localshare/enyo-doom. To change this, pass the option -DCMAKE_INSTALL_PREFIX=/usr (or whichever path you prefer) to your cmake command.

Then simply type make to begin compiling enyo-doom.

When the build is finished, type make install as root to install enyo-doom. If you are using Ubuntu or a derivative, you will need to use sudo make install; most other systems you will need to do something like su -c 'make install'. The install should create a shortcut in the default menu structure for most desktops under Games; you can manually create a shortcut or run it from the commandline using the command enyo-doom.

Configuration and Running Games

You will need to point enyo-doom to a game WAD (the data file that contain Doom level, sound, graphic, music, etc data). Change the dropdown under the logo to the type of Doom WAD you have and use Select beside the Main WAD entry to locate the proper WAD for that game type. The default game types and the WAD files they use are:

Doom:	doom.wad (or doom1.wad for the Shareware version)

Doom II: Hell on Earth:	doom2.wad

TNT Evilution:	tnt.wad

The Plutonia Experiment:	plutonia.wad


You may add new game types using the Add new... button next to the game type drop-down menu. It will ask you the name of your new game type and if you want to copy the existing game settings to your new game type, which is useful if you want to create a game type from an existing game and addons (see below). If your game engine supports Heretic and/or Hexen or other Doom game derivatives, you may add them in this way. Removing an unused or unwanted engine can be done with the Remove button which will ask for confirmation.

Extra patch WADs (pwads) or any other compatible file type (for example, pk3 files for ZDoom and others) that add levels, sounds, music, etc., can be enabled by enyo-doom by using the Add... button beside the Addons box in the Launcher tab and removed by highlighting (clicking) the added file and clicking Remove. To return to having no extra WADs load, click Clear. Each WAD set is specific to the game type you choose. Note that Doom 1's patch WADs will not work with Doom Shareware (doom1.wad).

The "AdditionaL commandline options" input is an advanced option for each game type that allows you to specify custom commandline options to the game. This may use standard Doom options that Enyo does not provide, or engine-specific options. If you do not know what this does, it is best to leave it blank.

By default, enyo-doom gives you three engines to choose from on the General tab: Chocolate Doom, prBoom, and ZDoom. The executable is the default name for each engine's executable, and is assumed to be in your system's PATH. If the name is different, or is not in a default PATH, you may change it using the Browse button beside the Path to Engine setting. Other engines may be added using the Add engine button, and removed using the Remove Engine button.

Each engine has an option to run in a window and disable sound completely (for system compatibility purposes).  Some engines may ignore one or more of these options.  

If you have a game type that requires the use of only one engine, the "Override default engine" option at the bottom of the Game tab allows you to use only the engine you select to run that game type. Selecting "(None)" (the default option) will use the engine type selected in the General tab.

Everything else should be pretty self-explanatory; just click the Run button at the bottom to start the game. If you have any problems running, make sure the output window is enabled (under Global Options) and view the output of the engine; it may provide a clue about what is not configured correctly. For Chocolate Doom, it is recommended that you run the chocolate-setup before you use enyo-doom to run it, since the majority of that engine's configuration is done there. To eliminate the information screen that pops up after you exit Chocolate Doom, untick Show ENDOOM screen in Display Configuration in chocolate-setup. In some engines there is no way to disable this option, and a jumble of control characters and text will show up at the end of the output. You can just safely ignore this.

Enyo?

Enyo is the Greek goddess of war, known as Bellona in Roman mythology. She is the companion to Ares, the god of war (known as Mars in Roman mythology), and his two sons Phobos and Deimos, linking Enyo to the setting of Doom (the Martian moons of Phobos and Deimos, and Mars itself). (Information from the Wikipedia article on Enyo.) 
