/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: sdmessage.proto */

#ifndef PROTOBUF_C_sdmessage_2eproto__INCLUDED
#define PROTOBUF_C_sdmessage_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1004000 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct MessageT MessageT;
typedef struct MessageT__Entry MessageT__Entry;


/* --- enums --- */

typedef enum _MessageT__Opcode {
  MESSAGE_T__OPCODE__OP_BAD = 0,
  MESSAGE_T__OPCODE__OP_SIZE = 10,
  MESSAGE_T__OPCODE__OP_DEL = 20,
  MESSAGE_T__OPCODE__OP_GET = 30,
  MESSAGE_T__OPCODE__OP_PUT = 40,
  MESSAGE_T__OPCODE__OP_GETKEYS = 50,
  MESSAGE_T__OPCODE__OP_PRINT = 60,
  MESSAGE_T__OPCODE__OP_STATS = 70,
  MESSAGE_T__OPCODE__OP_ERROR = 99
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(MESSAGE_T__OPCODE)
} MessageT__Opcode;
typedef enum _MessageT__CType {
  MESSAGE_T__C_TYPE__CT_BAD = 0,
  MESSAGE_T__C_TYPE__CT_KEY = 10,
  MESSAGE_T__C_TYPE__CT_VALUE = 20,
  MESSAGE_T__C_TYPE__CT_ENTRY = 30,
  MESSAGE_T__C_TYPE__CT_KEYS = 40,
  MESSAGE_T__C_TYPE__CT_RESULT = 50,
  MESSAGE_T__C_TYPE__CT_TABLE = 60,
  MESSAGE_T__C_TYPE__CT_NONE = 70,
  MESSAGE_T__C_TYPE__CT_STATS = 80
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(MESSAGE_T__C_TYPE)
} MessageT__CType;

/* --- messages --- */

struct  MessageT__Entry
{
  ProtobufCMessage base;
  char *key;
  ProtobufCBinaryData data;
};
#define MESSAGE_T__ENTRY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&message_t__entry__descriptor) \
    , (char *)protobuf_c_empty_string, {0,NULL} }


struct  MessageT
{
  ProtobufCMessage base;
  MessageT__Opcode opcode;
  MessageT__CType c_type;
  size_t n_keys;
  char **keys;
  ProtobufCBinaryData data;
  int32_t result;
  size_t n_entries;
  MessageT__Entry **entries;
  size_t n_stats;
  int32_t *stats;
};
#define MESSAGE_T__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&message_t__descriptor) \
    , MESSAGE_T__OPCODE__OP_BAD, MESSAGE_T__C_TYPE__CT_BAD, 0,NULL, {0,NULL}, 0, 0,NULL, 0,NULL }


/* MessageT__Entry methods */
void   message_t__entry__init
                     (MessageT__Entry         *message);
/* MessageT methods */
void   message_t__init
                     (MessageT         *message);
size_t message_t__get_packed_size
                     (const MessageT   *message);
size_t message_t__pack
                     (const MessageT   *message,
                      uint8_t             *out);
size_t message_t__pack_to_buffer
                     (const MessageT   *message,
                      ProtobufCBuffer     *buffer);
MessageT *
       message_t__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   message_t__free_unpacked
                     (MessageT *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*MessageT__Entry_Closure)
                 (const MessageT__Entry *message,
                  void *closure_data);
typedef void (*MessageT_Closure)
                 (const MessageT *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor message_t__descriptor;
extern const ProtobufCMessageDescriptor message_t__entry__descriptor;
extern const ProtobufCEnumDescriptor    message_t__opcode__descriptor;
extern const ProtobufCEnumDescriptor    message_t__c_type__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_sdmessage_2eproto__INCLUDED */
