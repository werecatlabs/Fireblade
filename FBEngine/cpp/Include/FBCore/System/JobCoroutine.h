#ifndef JobCoroutine_h__
#define JobCoroutine_h__

// Declare start of protothread (use at start of Run() implementation).
#define JOB_BEGIN() \
    bool ptYielded = true; \
    switch( rYield->getLineNumber() ) \
    { \
    case 0: \
    {
// Stop protothread and end it (use at end of Run() implementation).
#define JOB_END() \
    } \
default: \
{ \
}; \
    } \
    rYield->stop(); \
    return;
#define JOB_FUNCTION_END() \
    } \
default: \
{ \
}; \
    } \
    return;

//#define JOB_YIELD() \
//		} do {  \
//			ptYielded = false;  \
//			rYield.setLineNumber(__FUNCTION__, __LINE__);  \
//	case __LINE__: {  \
//		if (!ptYielded)  \
//			return;  \
//		} while (0)

#define JOB_YIELD() \
    } \
    do \
    { \
        ptYielded = false; \
        rYield->setLineNumber( __LINE__ ); \
    case __LINE__: \
        if( !ptYielded ) \
            return; \
    } while( 0 ); \
    {
//#define JOB_YIELD_UNTIL(condition) \
//		} do { ptYielded = false; rYield.setLineNumber(__FUNCTION__, __LINE__);  \
//		case __LINE__:   \
//				if (!ptYielded || !(condition)) \
//					return;  \
//		} while (0)

#define JOB_YIELD_UNTIL_FALSE( condition ) \
    } \
    do \
    { \
        ptYielded = false; \
        rYield->setLineNumber( __LINE__ ); \
    case __LINE__: \
        if( !ptYielded || ( condition ) ) \
            return; \
    } while( 0 ); \
    {
#define JOB_YIELD_UNTIL_TRUE( condition ) \
    } \
    do \
    { \
        ptYielded = false; \
        rYield->setLineNumber( __LINE__ ); \
    case __LINE__: \
        if( !ptYielded || ( !( condition ) ) ) \
            return; \
    } while( 0 ); \
    {
#endif  // JobCoroutine_h__
