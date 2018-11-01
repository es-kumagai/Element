/*
    Settings.cpp - This file is part of Element
    Copyright (C) 2014  Kushview, LLC.  All rights reserved.
*/

#include "Settings.h"

namespace Element {

const char* Settings::checkForUpdatesKey        = "checkForUpdates";
const char* Settings::pluginFormatsKey          = "pluginFormatsKey";
const char* Settings::pluginWindowOnTopDefault  = "pluginWindowOnTopDefault";
const char* Settings::scanForPluginsOnStartKey  = "scanForPluginsOnStart";
const char* Settings::showPluginWindowsKey      = "showPluginWindows";
const char* Settings::openLastUsedSessionKey    = "openLastUsedSession";
const char* Settings::askToSaveSessionKey       = "askToSaveSession";
const char* Settings::defaultNewSessionFile     = "defaultNewSessionFile";

#if JUCE_32BIT
 #if JUCE_MAC
  const char* Settings::lastPluginScanPathPrefix = "pluginScanPath32_";
  const char* Settings::pluginListKey            = "pluginList32";
 
 #else
  const char* Settings::lastPluginScanPathPrefix = "pluginScanPath_";   // TODO: migrate this
  const char* Settings::pluginListKey            = "plugin-list";       // TODO: migrate this
 #endif

#else // 64bit keys
 #if JUCE_MAC
  const char* Settings::lastPluginScanPathPrefix = "pluginScanPath_";   // TODO: migrate this
  const char* Settings::pluginListKey            = "plugin-list";       // TODO: migrate this
 
 #else
  const char* Settings::lastPluginScanPathPrefix = "pluginScanPath64_";
  const char* Settings::pluginListKey            = "pluginList64";
 #endif
#endif
    
Settings::Settings()
{
    PropertiesFile::Options opts;
    opts.applicationName     = "Element";
    opts.filenameSuffix      = "conf";
    opts.osxLibrarySubFolder = "Application Support";
    opts.storageFormat       = PropertiesFile::storeAsCompressedBinary;

   #if JUCE_DEBUG
    opts.applicationName     = "ElementDebug";
    opts.storageFormat       = PropertiesFile::storeAsXML;
   #endif
    
   #if JUCE_LINUX
    opts.folderName          = ".config/Element";
   #else
    opts.folderName          = "Element";
   #endif

    setStorageParameters (opts);
}

Settings::~Settings() { }

    
bool Settings::checkForUpdates() const
{
    if (auto* props = getProps())
        return props->getBoolValue (checkForUpdatesKey, true);
    return false;
}

void Settings::setCheckForUpdates (const bool shouldCheck)
{
    if (shouldCheck == checkForUpdates())
        return;
    if (auto* p = getProps())
        p->setValue (checkForUpdatesKey, shouldCheck);
}
    
PropertiesFile* Settings::getProps() const
{
    return (const_cast<Settings*> (this))->getUserSettings();
}

    
XmlElement* Settings::getLastGraph() const
{
    if (auto* p = getProps())
        return p->getXmlValue ("lastGraph");
    return nullptr;
}

void Settings::setLastGraph (const ValueTree& data)
{
    jassert (data.hasType (Tags::node));
    if (! data.hasType (Tags::node))
        return;
    if (auto* p = getProps())
        if (ScopedXml xml = data.createXml())
            p->setValue ("lastGraph", xml);
}
    

bool Settings::scanForPluginsOnStartup() const
{
    if (auto* p = getProps())
        return p->getBoolValue (scanForPluginsOnStartKey, false);
    return false;
}
    
void Settings::setScanForPluginsOnStartup (const bool shouldScan)
{
    if (shouldScan == scanForPluginsOnStartup())
        return;
    if (auto* p = getProps())
        p->setValue (scanForPluginsOnStartKey, shouldScan);
}

bool Settings::showPluginWindowsWhenAdded() const
{
    if (auto* p = getProps())
        return p->getBoolValue (showPluginWindowsKey, true);
    return true;
}

void Settings::setShowPluginWindowsWhenAdded (const bool shouldShow)
{
    if (shouldShow == showPluginWindowsWhenAdded())
        return;
    if (auto* p = getProps())
        p->setValue (showPluginWindowsKey, shouldShow);
}

bool Settings::openLastUsedSession() const
{
    if (auto* p = getProps())
        return p->getBoolValue (openLastUsedSessionKey, true);
    return true;
}

void Settings::setOpenLastUsedSession (const bool shouldOpen)
{
    if (shouldOpen == openLastUsedSession())
        return;
    if (auto* p = getProps())
        p->setValue (openLastUsedSessionKey, shouldOpen);
}

bool Settings::pluginWindowsOnTop() const
{
    if (auto* p = getProps())
        return p->getBoolValue (pluginWindowOnTopDefault, true);
    return false;
}

bool Settings::askToSaveSession()
{
    if (auto* props = getProps())
        return props->getBoolValue (askToSaveSessionKey, true);
    return false;
}

void Settings::setAskToSaveSession (const bool value)
{
    if (auto* props = getProps())
        props->setValue (askToSaveSessionKey, value);
}

void Settings::setPluginWindowsOnTop (const bool onTop)
{
    if (onTop == pluginWindowsOnTop())
        return;
    if (auto* p = getProps())
        p->setValue (pluginWindowOnTopDefault, onTop);
}

const File Settings::getDefaultNewSessionFile() const
{
    if (auto* p = getProps())
    {
        const auto value = p->getValue (defaultNewSessionFile);
        if (value.isNotEmpty() && File::isAbsolutePath (value))
            return File (value);
    }

    return File();
}

void Settings::setDefaultNewSessionFile (const File& file)
{
    if (auto* p = getProps())
        p->setValue (defaultNewSessionFile, 
            file.existsAsFile() ? file.getFullPathName() : "");
}

}
