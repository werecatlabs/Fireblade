#ifndef SQLiteObfuscateCodecInterface_h__
#define SQLiteObfuscateCodecInterface_h__

#ifdef __cplusplus
extern "C"
#endif
    void
    InitializeBotan();

#ifdef __cplusplus
extern "C"
#endif
    void *
    InitializeNewCodec( void *db );

#ifdef __cplusplus
extern "C"
#endif
    void *
    InitializeFromOtherCodec( const void *otherCodec, void *db );

#ifdef __cplusplus
extern "C"
#endif
    void
    GenerateWriteKey( void *codec, const char *userPassword, int passwordLength );

#ifdef __cplusplus
extern "C"
#endif
    void
    DropWriteKey( void *codec );

#ifdef __cplusplus
extern "C"
#endif
    void
    SetWriteIsRead( void *codec );

#ifdef __cplusplus
extern "C"
#endif
    void
    SetReadIsWrite( void *codec );

#ifdef __cplusplus
extern "C"
#endif
    unsigned char *
    Encrypt( void *codec, int page, unsigned char *data, unsigned char useWriteKey );

#ifdef __cplusplus
extern "C"
#endif
    void
    Decrypt( void *codec, int page, unsigned char *data );

#ifdef __cplusplus
extern "C"
#endif
    void
    SetPageSize( void *codec, int pageSize );

#ifdef __cplusplus
extern "C"
#endif
    unsigned char
    HasReadKey( void *codec );

#ifdef __cplusplus
extern "C"
#endif
    unsigned char
    HasWriteKey( void *codec );

#ifdef __cplusplus
extern "C"
#endif
    void *
    GetDB( void *codec );

#ifdef __cplusplus
extern "C"
#endif
    const char *
    GetAndResetError( void *codec );

#ifdef __cplusplus
extern "C"
#endif
    void
    DeleteCodec( void *codec );

#endif  // SQLiteObfuscateCodec_h__
