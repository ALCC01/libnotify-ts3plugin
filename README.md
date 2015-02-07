TeamSpeak 3 Libnotify
============================

Libnotify + TeamSpeak = nice Ubuntu notifications.
Numix Project's icons are required (if you don't want to edit the icon location in the source code).
I recommend to also install NotifyOSD Configuration for dismissable notifications and custom timeout.

Original work from Joachrica's repository.

**Build:**

	./build.sh
	cp libnotifyplugin.so /path/to/ts3/plugins/

Building in ubuntu requires the packages libnotify-dev and libgtk2.0-dev
	
	sudo apt-get install libnotify-dev libgtk2.0-dev

**Enable plugin**

In TeamSpeak3, go to Settings->Plugins and select the Lib-Notify plugin.
