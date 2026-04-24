#ifndef STD_TYPES_H
#define STD_TYPES_H

/* Fixed-width unsigned integers */
typedef unsigned char      uint8 ;
typedef unsigned short int uint16;
typedef unsigned int       uint32;

/* Fixed-width signed integers */
typedef signed char      sint8 ;
typedef signed short int sint16;
typedef signed int       sint32;

/*Boolean Data type*/
typedef uint8 boolean;
#define TRUE ((boolean)1)
#define FALSE ((boolean)0)

/*Standard Return Type*/
typedef uint8 std_returntype;
#define E_OK ((std_returntype)0)
#define E_NOK ((std_returntype)1)

/*Generic Pointer*/
typedef void* GenPtr;


#endif