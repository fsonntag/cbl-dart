#pragma once

#include "CBLDart_Export.h"
#include "dart/dart_api_dl.h"
#ifdef CBL_FRAMEWORK_HEADERS
#include <CouchbaseLite/Fleece.h>
#else
#include "fleece/Fleece.h"
#endif

extern "C" {

// === Slice ==================================================================

/**
 * See `FLSlice` in Dart code.
 */
struct CBLDart_FLSlice {
  const void *buf;
  uint64_t size;
};

struct CBLDart_FLSliceResult {
  const void *buf;
  uint64_t size;
};

typedef CBLDart_FLSlice CBLDart_FLString;
typedef CBLDart_FLSliceResult CBLDart_FLStringResult;

CBLDART_EXPORT
bool CBLDart_FLSlice_Equal(CBLDart_FLSlice a, CBLDart_FLSlice b);

CBLDART_EXPORT
int64_t CBLDart_FLSlice_Compare(CBLDart_FLSlice a, CBLDart_FLSlice b);

CBLDART_EXPORT
CBLDart_FLSliceResult CBLDart_FLSliceResult_New(uint64_t size);

CBLDART_EXPORT
CBLDart_FLSliceResult CBLDart_FLSlice_Copy(CBLDart_FLSlice slice);

CBLDART_EXPORT
void CBLDart_FLSliceResult_BindToDartObject(Dart_Handle object,
                                            CBLDart_FLSliceResult slice,
                                            bool retain);

CBLDART_EXPORT
void CBLDart_FLSliceResult_Retain(CBLDart_FLSliceResult slice);

CBLDART_EXPORT
void CBLDart_FLSliceResult_Release(CBLDart_FLSliceResult slice);

// === Doc ====================================================================

CBLDART_EXPORT
FLDoc CBLDart_FLDoc_FromResultData(CBLDart_FLSliceResult data, uint8_t trust,
                                   FLSharedKeys sharedKeys,
                                   CBLDart_FLSlice externalData);

CBLDART_EXPORT
FLDoc CBLDart_FLDoc_FromJSON(CBLDart_FLString json, FLError *errorOut);

CBLDART_EXPORT
void CBLDart_FLDoc_BindToDartObject(Dart_Handle object, FLDoc doc);

// === Value ==================================================================

CBLDART_EXPORT
void CBLDart_FLValue_BindToDartObject(Dart_Handle object, FLValue value,
                                      bool retain);

CBLDART_EXPORT
CBLDart_FLString CBLDart_FLValue_AsString(FLValue value);

CBLDART_EXPORT
CBLDart_FLSlice CBLDart_FLValue_AsData(FLValue value);

CBLDART_EXPORT
CBLDart_FLStringResult CBLDart_FLValue_ToString(FLValue value);

CBLDART_EXPORT
CBLDart_FLStringResult CBLDart_FLValue_ToJSONX(FLValue value, bool json5,
                                               bool canonicalForm);

// === Dict ===================================================================

CBLDART_EXPORT
FLValue CBLDart_FLDict_Get(FLDict dict, CBLDart_FLString keyString);

typedef struct {
  FLDictIterator *iterator;
  CBLDart_FLSlice keyString;
  bool done;
} CBLDart_DictIterator;

CBLDART_EXPORT
CBLDart_DictIterator *CBLDart_FLDictIterator_Begin(Dart_Handle object,
                                                   FLDict dict);

CBLDART_EXPORT
void CBLDart_FLDictIterator_Next(CBLDart_DictIterator *iterator);

CBLDART_EXPORT
void CBLDart_FLMutableDict_Remove(FLMutableDict dict, CBLDart_FLString key);

CBLDART_EXPORT
FLSlot CBLDart_FLMutableDict_Set(FLMutableDict dict, CBLDart_FLString key);

CBLDART_EXPORT
void CBLDart_FLSlot_SetString(FLSlot slot, CBLDart_FLString value);

CBLDART_EXPORT
FLMutableArray CBLDart_FLMutableDict_GetMutableArray(FLMutableDict dict,
                                                     CBLDart_FLString key);

CBLDART_EXPORT
FLMutableDict CBLDart_FLMutableDict_GetMutableDict(FLMutableDict dict,
                                                   CBLDart_FLString key);

// === Decoder ================================================================

struct CBLDart_LoadedFLValue {
  bool exists;
  int8_t type;
  bool isInteger;
  uint32_t collectionSize;
  bool asBool;
  int64_t asInt;
  double asDouble;
  CBLDart_FLString asString;
  CBLDart_FLSlice asData;
  FLValue asValue;
};

CBLDART_EXPORT
CBLDart_FLStringResult CBLDart_FLData_Dump(CBLDart_FLSlice data);

CBLDART_EXPORT
void CBLDart_FLValue_FromData(CBLDart_FLSlice data, uint8_t trust,
                              CBLDart_LoadedFLValue *out);

CBLDART_EXPORT
void CBLDart_GetLoadedFLValue(FLValue value, CBLDart_LoadedFLValue *out);

CBLDART_EXPORT
void CBLDart_FLArray_GetLoadedFLValue(FLArray array, uint32_t index,
                                      CBLDart_LoadedFLValue *out);

CBLDART_EXPORT
void CBLDart_FLDict_GetLoadedFLValue(FLDict dict, CBLDart_FLString key,
                                     CBLDart_LoadedFLValue *out);

struct CBLDart_FLDictIterator2 {
  bool isDone;
  CBLDart_FLString *keyOut;
  CBLDart_LoadedFLValue *valueOut;
  FLDictIterator *_iterator;
};

CBLDART_EXPORT
CBLDart_FLDictIterator2 *CBLDart_FLDictIterator2_Begin(
    Dart_Handle object, FLDict dict, CBLDart_FLString *keyOut,
    CBLDart_LoadedFLValue *valueOut);

CBLDART_EXPORT
void CBLDart_FLDictIterator2_Next(CBLDart_FLDictIterator2 *iterator);

// === Encoder ================================================================

CBLDART_EXPORT
void CBLDart_FLEncoder_BindToDartObject(Dart_Handle object, FLEncoder encoder);

CBLDART_EXPORT
FLEncoder CBLDart_FLEncoder_New(uint8_t format, uint64_t reserveSize,
                                bool uniqueStrings);

CBLDART_EXPORT
bool CBLDart_FLEncoder_WriteArrayValue(FLEncoder encoder, FLArray array,
                                       uint32_t index);

CBLDART_EXPORT
bool CBLDart_FLEncoder_WriteString(FLEncoder encoder, CBLDart_FLString value);

CBLDART_EXPORT
bool CBLDart_FLEncoder_WriteData(FLEncoder encoder, CBLDart_FLSlice value);

CBLDART_EXPORT
bool CBLDart_FLEncoder_WriteJSON(FLEncoder encoder, CBLDart_FLString value);

CBLDART_EXPORT
bool CBLDart_FLEncoder_BeginArray(FLEncoder encoder, uint64_t reserveCount);

CBLDART_EXPORT
bool CBLDart_FLEncoder_BeginDict(FLEncoder encoder, uint64_t reserveCount);

CBLDART_EXPORT
bool CBLDart_FLEncoder_WriteKey(FLEncoder encoder, CBLDart_FLString key);

CBLDART_EXPORT
CBLDart_FLSliceResult CBLDart_FLEncoder_Finish(FLEncoder encoder,
                                               FLError *errorOut);
}
