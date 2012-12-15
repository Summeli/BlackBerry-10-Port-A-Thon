#ifndef FLOOD_PLUGIN_H
#define FLOOD_PLUGIN_H

#include <QtDeclarative/QDeclarativeExtensionPlugin>

class FloodPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri);
};

#endif // FLOOD_PLUGIN_H

