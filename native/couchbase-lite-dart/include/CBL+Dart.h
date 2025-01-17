
#pragma once

#include "Fleece+Dart.h"
#ifdef CBL_FRAMEWORK_HEADERS
#include <CouchbaseLite/CouchbaseLite.h>
#else
#include "cbl/CouchbaseLite.h"
#endif
#include "CBLDart_Export.h"
#include "dart/dart_api_dl.h"

/**
 * This is a compatibility layer to allow Dart code to use the Couchbase Lite C
 * API. Some method signatures are incompatible with Dart's FFI capabilities.
 *
 * This layer is also where memory management of objects from the Couchbase Lite
 * C API is integrated with the garbage collection of Dart objects.
 */

extern "C" {

/**
 * Initializes the native libraries.
 *
 * This function can be called multiple times and is thread save. The
 * libraries are only initialized by the first call and subsequent calls are
 * NOOPs.
 */
CBLDART_EXPORT
bool CBLDart_Initialize(void *dartInitializeDlData, void *cblInitContext,
                        CBLError *errorOut);

// === Dart Native ============================================================

// === Async Callbacks

typedef struct _CBLDart_AsyncCallback *CBLDart_AsyncCallback;

CBLDART_EXPORT
CBLDart_AsyncCallback CBLDart_AsyncCallback_New(uint32_t id, Dart_Handle object,
                                                Dart_Port sendPort, bool debug);

CBLDART_EXPORT
void CBLDart_AsyncCallback_Close(CBLDart_AsyncCallback callback);

CBLDART_EXPORT
void CBLDart_AsyncCallback_CallForTest(CBLDart_AsyncCallback callback,
                                       int64_t argument);

// === Dart Finalizer

CBLDART_EXPORT
void CBLDart_RegisterDartFinalizer(Dart_Handle object, Dart_Port registry,
                                   int64_t token);

// === Couchbase Lite =========================================================

// === Base

CBLDART_EXPORT
CBLDart_FLStringResult CBLDart_CBLError_Message(CBLError *error);

/**
 * Binds a CBLRefCounted to a Dart objects lifetime.
 *
 * If \p retain is true the ref counted object will be retained. Otherwise
 * it will only be released once the Dart object is garbage collected.
 */
CBLDART_EXPORT
void CBLDart_BindCBLRefCountedToDartObject(Dart_Handle object,
                                           CBLRefCounted *refCounted,
                                           bool retain, char *debugName);

/**
 * Sets whether information to debug CBLRefCounted is printed.
 *
 * This features is only functional in debug builds.
 */
CBLDART_EXPORT
void CBLDart_SetDebugRefCounted(bool enabled);

// === Log

CBLDART_EXPORT
void CBLDart_CBL_LogMessage(CBLLogDomain domain, CBLLogLevel level,
                            CBLDart_FLString message);

CBLDART_EXPORT
bool CBLDart_CBLLog_SetCallback(CBLDart_AsyncCallback callback);

CBLDART_EXPORT
void CBLDart_CBLLog_SetCallbackLevel(CBLLogLevel level);

typedef struct {
  CBLLogLevel level;
  CBLDart_FLString directory;
  uint32_t maxRotateCount;
  uint64_t maxSize;
  bool usePlaintext;
} CBLDart_CBLLogFileConfiguration;

CBLDART_EXPORT
bool CBLDart_CBLLog_SetFileConfig(CBLDart_CBLLogFileConfiguration *config,
                                  CBLError *errorOut);

CBLDART_EXPORT
CBLDart_CBLLogFileConfiguration *CBLDart_CBLLog_GetFileConfig();

CBLDART_EXPORT
bool CBLDart_CBLLog_SetSentryBreadcrumbs(bool enabled);

// === Document

CBLDART_EXPORT
CBLDart_FLString CBLDart_CBLDocument_ID(CBLDocument *doc);

CBLDART_EXPORT
CBLDart_FLString CBLDart_CBLDocument_RevisionID(CBLDocument *doc);

CBLDART_EXPORT
CBLDart_FLStringResult CBLDart_CBLDocument_CreateJSON(CBLDocument *doc);

CBLDART_EXPORT
CBLDocument *CBLDart_CBLDocument_CreateWithID(CBLDart_FLString docID);

CBLDART_EXPORT
int8_t CBLDart_CBLDocument_SetJSON(CBLDocument *doc, CBLDart_FLString json,
                                   CBLError *errorOut);

// === Database

#ifdef COUCHBASE_ENTERPRISE
CBLDART_EXPORT
bool CBLDart_CBLEncryptionKey_FromPassword(CBLEncryptionKey *key,
                                           CBLDart_FLString password);
#endif

struct CBLDart_CBLDatabaseConfiguration {
  uint32_t _padding;
  CBLDart_FLString directory;
#ifdef COUCHBASE_ENTERPRISE
  CBLEncryptionKey encryptionKey;
#endif
};

CBLDART_EXPORT
CBLDart_CBLDatabaseConfiguration CBLDart_CBLDatabaseConfiguration_Default();

CBLDART_EXPORT
bool CBLDart_CBL_DatabaseExists(CBLDart_FLString name,
                                CBLDart_FLString inDirectory);

CBLDART_EXPORT
bool CBLDart_CBL_CopyDatabase(CBLDart_FLString fromPath,
                              CBLDart_FLString toName,
                              CBLDart_CBLDatabaseConfiguration *config,
                              CBLError *errorOut);

CBLDART_EXPORT
bool CBLDart_CBL_DeleteDatabase(CBLDart_FLString name,
                                CBLDart_FLString inDirectory,
                                CBLError *errorOut);

CBLDART_EXPORT
CBLDatabase *CBLDart_CBLDatabase_Open(CBLDart_FLString name,
                                      CBLDart_CBLDatabaseConfiguration *config,
                                      CBLError *errorOut);

CBLDART_EXPORT
void CBLDart_BindDatabaseToDartObject(Dart_Handle object, CBLDatabase *database,
                                      char *debugName);

CBLDART_EXPORT
bool CBLDart_CBLDatabase_Close(CBLDatabase *database, bool andDelete,
                               CBLError *errorOut);

CBLDART_EXPORT CBLDart_FLString CBLDart_CBLDatabase_Name(CBLDatabase *db);

CBLDART_EXPORT
CBLDart_FLStringResult CBLDart_CBLDatabase_Path(CBLDatabase *db);

CBLDART_EXPORT
CBLDart_CBLDatabaseConfiguration CBLDart_CBLDatabase_Config(CBLDatabase *db);

CBLDART_EXPORT
const CBLDocument *CBLDart_CBLDatabase_GetDocument(CBLDatabase *database,
                                                   CBLDart_FLString docID,
                                                   CBLError *errorOut);

CBLDART_EXPORT
CBLDocument *CBLDart_CBLDatabase_GetMutableDocument(CBLDatabase *database,
                                                    CBLDart_FLString docID,
                                                    CBLError *errorOut);

CBLDART_EXPORT
bool CBLDart_CBLDatabase_SaveDocumentWithConcurrencyControl(
    CBLDatabase *db, CBLDocument *doc, CBLConcurrencyControl concurrency,
    CBLError *errorOut);

CBLDART_EXPORT
bool CBLDart_CBLDatabase_PurgeDocumentByID(CBLDatabase *database,
                                           CBLDart_FLString docID,
                                           CBLError *errorOut);

CBLDART_EXPORT
CBLTimestamp CBLDart_CBLDatabase_GetDocumentExpiration(CBLDatabase *db,
                                                       CBLDart_FLSlice docID,
                                                       CBLError *errorOut);

CBLDART_EXPORT
bool CBLDart_CBLDatabase_SetDocumentExpiration(CBLDatabase *db,
                                               CBLDart_FLSlice docID,
                                               CBLTimestamp expiration,
                                               CBLError *errorOut);

CBLDART_EXPORT
void CBLDart_CBLDatabase_AddDocumentChangeListener(
    const CBLDatabase *db, const CBLDart_FLString docID,
    CBLDart_AsyncCallback listener);

CBLDART_EXPORT
void CBLDart_CBLDatabase_AddChangeListener(const CBLDatabase *db,
                                           CBLDart_AsyncCallback listener);

typedef enum : uint8_t {
  kCBLDart_IndexTypeValue,
  kCBLDart_IndexTypeFullText,
} CBLDart_IndexType;

struct CBLDart_CBLIndexSpec {
  CBLDart_IndexType type;
  CBLQueryLanguage expressionLanguage;
  CBLDart_FLString expressions;
  bool ignoreAccents;
  CBLDart_FLString language;
};

CBLDART_EXPORT
bool CBLDart_CBLDatabase_CreateIndex(CBLDatabase *db, CBLDart_FLString name,
                                     CBLDart_CBLIndexSpec indexSpec,
                                     CBLError *errorOut);

CBLDART_EXPORT
bool CBLDart_CBLDatabase_DeleteIndex(CBLDatabase *db, CBLDart_FLString name,
                                     CBLError *errorOut);

// === Query

CBLDART_EXPORT
CBLQuery *CBLDart_CBLDatabase_CreateQuery(CBLDatabase *db,
                                          CBLQueryLanguage language,
                                          CBLDart_FLString queryString,
                                          int *errorPosOut, CBLError *errorOut);

CBLDART_EXPORT
CBLDart_FLStringResult CBLDart_CBLQuery_Explain(const CBLQuery *query);

CBLDART_EXPORT
CBLDart_FLString CBLDart_CBLQuery_ColumnName(const CBLQuery *query,
                                             unsigned columnIndex);

CBLDART_EXPORT
FLValue CBLDart_CBLResultSet_ValueForKey(CBLResultSet *rs,
                                         CBLDart_FLString key);

CBLDART_EXPORT
CBLListenerToken *CBLDart_CBLQuery_AddChangeListener(
    CBLQuery *query, CBLDart_AsyncCallback listener);

// === Blob

CBLDART_EXPORT
CBLDart_FLString CBLDart_CBLBlob_Digest(CBLBlob *blob);

CBLDART_EXPORT
CBLDart_FLString CBLDart_CBLBlob_ContentType(CBLBlob *blob);

CBLDART_EXPORT
CBLDart_FLSliceResult CBLDart_CBLBlob_Content(const CBLBlob *blob,
                                              CBLError *errorOut);

CBLDART_EXPORT
void CBLDart_BindBlobReadStreamToDartObject(Dart_Handle object,
                                            CBLBlobReadStream *stream);

CBLDART_EXPORT
CBLDart_FLSliceResult CBLDart_CBLBlobReader_Read(CBLBlobReadStream *stream,
                                                 uint64_t bufferSize,
                                                 CBLError *outError);

CBLDART_EXPORT
CBLBlob *CBLDart_CBLBlob_CreateWithData(CBLDart_FLString contentType,
                                        CBLDart_FLSlice contents);

CBLDART_EXPORT CBLBlob *CBLDart_CBLBlob_CreateWithStream(
    CBLDart_FLString contentType, CBLBlobWriteStream *writer);

// === Replicator

CBLDART_EXPORT
CBLEndpoint *CBLDart_CBLEndpoint_CreateWithURL(CBLDart_FLString url,
                                               CBLError *errorOut);

CBLDART_EXPORT
CBLAuthenticator *CBLDart_CBLAuth_CreatePassword(CBLDart_FLString username,
                                                 CBLDart_FLString password);

CBLDART_EXPORT
CBLAuthenticator *CBLDart_CBLAuth_CreateSession(CBLDart_FLString sessionID,
                                                CBLDart_FLString cookieName);

struct CBLDart_CBLProxySettings {
  CBLProxyType type;
  CBLDart_FLString hostname;
  uint16_t port;
  CBLDart_FLString username;
  CBLDart_FLString password;
};

struct CBLDart_ReplicatorConfiguration {
  CBLDatabase *database;
  CBLEndpoint *endpoint;
  CBLReplicatorType replicatorType;
  bool continuous;
  bool disableAutoPurge;
  uint32_t maxAttempts;
  uint32_t maxAttemptWaitTime;
  uint32_t heartbeat;
  CBLAuthenticator *authenticator;
  CBLDart_CBLProxySettings *proxy;
  FLDict headers;
  CBLDart_FLSlice *pinnedServerCertificate;
  CBLDart_FLSlice *trustedRootCertificates;
  FLArray channels;
  FLArray documentIDs;
  CBLDart_AsyncCallback pushFilter;
  CBLDart_AsyncCallback pullFilter;
  CBLDart_AsyncCallback conflictResolver;
};

CBLDART_EXPORT
CBLReplicator *CBLDart_CBLReplicator_Create(
    CBLDart_ReplicatorConfiguration *config, CBLError *errorOut);

CBLDART_EXPORT
void CBLDart_BindReplicatorToDartObject(Dart_Handle object,
                                        CBLReplicator *replicator,
                                        char *debugName);

CBLDART_EXPORT
bool CBLDart_CBLReplicator_IsDocumentPending(CBLReplicator *replicator,
                                             CBLDart_FLString docId,
                                             CBLError *errorOut);
CBLDART_EXPORT
void CBLDart_CBLReplicator_AddChangeListener(CBLReplicator *replicator,
                                             CBLDart_AsyncCallback listenerId);

CBLDART_EXPORT
void CBLDart_CBLReplicator_AddDocumentReplicationListener(
    CBLReplicator *replicator, CBLDart_AsyncCallback listenerId);
}
