/**
 * @file
 *
 * @brief Source for typedispatcher plugin
 *
 * @copyright BSD License (see doc/LICENSE.md or http://www.libelektra.org)
 *
 */

#ifndef ELEKTRA_PLUGIN_TYPEDISPATCHER_HELPER_H
#define ELEKTRA_PLUGIN_TYPEDISPATCHER_HELPER_H

#include <kdbplugin.h>

#define DEVBUILD
#define VERBOSEBUILD

// helper for readability
typedef enum {
	ERROR = -1,
	SUCCESS = 1,

} RC;

typedef enum {
	SKEL,
	SUBTYPE,
	SUMTYPE,
} TypeType;

typedef enum {
	FAIL,
	IGNORE,
	DROPKEY,
} OnError;

typedef enum {
	ONCHECK = (1 << 0),
	ONTYPE = (1 << 1),
	ONBOTH = ONCHECK | ONTYPE,
} DispatchOn;

// configuration of a defined type
typedef struct
{
	TypeType type;
	Key * scope;     // name of the key where the type was defined
	KeySet * checks; // holds keys with the check metadata for each type check
	KeySet * types;  // references to the keys with supertypes
	KeySet * params; // parameter names + types
} TypeConfig;

// helpertype for pointers to validateKey functions
typedef int (*ValidateFunction) (Key *, Key *);

// helper struct for loaded plugins
typedef struct
{
	Plugin * plugin;    // address of loaded plugin
	ValidateFunction f; // pointer to the validateKey function of the plugin
} PluginConfig;

// holds data needed by most functions
typedef struct
{
	OnError onError;
	DispatchOn dispatchOn;

	KeySet * modules;    // modules keyset for PluginOpen/PluginClose
	KeySet * plugins;    // all loaded plugins. keyname == plugin name
			     // value == PluginConfig
	KeySet * types;      // all defined types. keyname == name of type
			     // value == TypeConfig
	KeySet * pluginMeta; // Metadata + the plugins providing it
} DispatchConfig;


// splits type string into type and arguments
typedef struct
{
	char * type;   // type name
	KeySet * args; // a key for each argument, keynames are array indices,
		       // values the arguments
} ArgumentConfig;

// helpers.c functions
DispatchConfig * initDispatchConfig ();
void closeDispatchConfig (Plugin *);
KeySet * getAllKeysBelow (const Key *, KeySet *);
KeySet * getKeysDirectBelow (const Key *, KeySet *);
TypeConfig * newTypeConfig ();
void setTypeType (TypeConfig *, const Key *);
TypeType getTypeType (TypeConfig *);
Key * getTypeKey (DispatchConfig *, const char *);
TypeConfig * getType (DispatchConfig *, const char *);
ArgumentConfig * parseTypeString (DispatchConfig *, const char *);
void freeArgumentConfig (ArgumentConfig *);
KeySet * makeParamKS (KeySet *, ArgumentConfig *);
int isWithinScope (const TypeConfig *, const Key *);
char * replaceParametersWithArguments (const Key *, KeySet *);
void parseMetadata (const Key *, KeySet *, KeySet *);

// typereader.c
int getTypeDefinitions (Key *, DispatchConfig *, Key *);

// typecheck.c
int validateTypeKey (Key *, KeySet *, DispatchConfig *, Key *);
int validateCheckKey (Key *, KeySet *, DispatchConfig *, Key *);

// pluginhelper.c
ValidateFunction getValidateFunction (DispatchConfig *, const char *);

#endif
