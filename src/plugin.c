#ifdef _WIN32
#pragma warning (disable : 4100) 
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "public_errors.h"
#include "public_errors_rare.h"
#include "public_definitions.h"
#include "public_rare_definitions.h"
#include "ts3_functions.h"
#include "plugin.h"
#include <libnotify/notify.h>

static struct TS3Functions ts3Functions;

#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s
#else
#define _strcpy(dest, destSize, src) { strncpy(dest, src, destSize-1); (dest)[destSize-1] = '\0'; }
#endif

#define PLUGIN_API_VERSION 20

#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

static char* pluginID = NULL;

#ifdef _WIN32
static int wcharToUtf8(const wchar_t* str, char** result) {
	int outlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
	*result = (char*)malloc(outlen);
	if(WideCharToMultiByte(CP_UTF8, 0, str, -1, *result, outlen, 0, 0) == 0) {
		*result = NULL;
		return -1;
	}
	return 0;
}
#endif
const char* ts3plugin_name() {
#ifdef _WIN32
	static char* result = NULL;
	if(!result) {
		const wchar_t* name = L"Lib-notify plugin";
		if(wcharToUtf8(name, &result) == -1) {
			result = "Lib-notify plugin";
		}
	}
	return result;
#else
	return "Lib-notify plugin";
#endif
}
const char* ts3plugin_version() {
    return "0.0.1";
}
int ts3plugin_apiVersion() {
	return PLUGIN_API_VERSION;
}
const char* ts3plugin_author() {
    return "Alberto Coscia";
}
const char* ts3plugin_description() {
    return "This plugin enable lib-notify notifications for linux";
}

void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
    ts3Functions = funcs;
}
int ts3plugin_init() {
    char appPath[PATH_BUFSIZE];
    char resourcesPath[PATH_BUFSIZE];
    char configPath[PATH_BUFSIZE];
	char pluginPath[PATH_BUFSIZE];
    //printf("PLUGIN: init\n");
    ts3Functions.getAppPath(appPath, PATH_BUFSIZE);
    ts3Functions.getResourcesPath(resourcesPath, PATH_BUFSIZE);
    ts3Functions.getConfigPath(configPath, PATH_BUFSIZE);
	ts3Functions.getPluginPath(pluginPath, PATH_BUFSIZE);

	//printf("PLUGIN: App path: %s\nResources path: %s\nConfig path: %s\nPlugin path: %s\n", appPath, resourcesPath, configPath, pluginPath);

    return 0;
}

void ts3plugin_shutdown() {
    //printf("PLUGIN: shutdown\n");
	if(pluginID) {
		free(pluginID);
		pluginID = NULL;
	}
}
void ts3plugin_onConnectStatusChangeEvent(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber) {

}


int ts3plugin_onTextMessageEvent(uint64 serverConnectionHandlerID, anyID targetMode, anyID toID, anyID fromID, const char* fromName, const char* fromUniqueIdentifier, const char* message, int ffIgnored) {
    //printf("PLUGIN: onTextMessageEvent %llu %d %d %s %s %d\n", (long long unsigned int)serverConnectionHandlerID, targetMode, fromID, fromName, message, ffIgnored);
	send_notification(serverConnectionHandlerID, fromName, message);
    return 0; 
}

void send_notification(uint64 serverConnectionHandlerID, const char* event_name, const char* message) {
	notify_init(event_name);
	NotifyNotification * notification = notify_notification_new(event_name, message, "/usr/share/icons/Numix/256x256/actions/mail-mark-read.svg");
	notify_notification_set_timeout(notification, NOTIFICATION_DURATION);
	if(!notify_notification_show(notification, NULL)) {
		ts3Functions.logMessage("libnotify error", LogLevel_ERROR, "Plugin", serverConnectionHandlerID);
	}
	
	g_object_unref(G_OBJECT(notification));
}

int ts3plugin_onClientPokeEvent(uint64 serverConnectionHandlerID, anyID fromClientID, const char* pokerName, const char* pokerUniqueIdentity, const char* message, int ffIgnored) {
    anyID myID;	
	send_notification(serverConnectionHandlerID, pokerName, message);

    return 0; 
}
