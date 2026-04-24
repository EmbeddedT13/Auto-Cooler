#ifndef BIT_MATH_H
#define BIT_MATH_H


/* Single Bit Operations */
#define SET_BIT(REG, BIT)    ((REG) |= (1U << (BIT)))

#define CLEAR_BIT(REG, BIT)  ((REG) &= ~(1U << (BIT)))

#define TOGGLE_BIT(REG, BIT) ((REG) ^= (1U << (BIT) ))

#define READ_BIT(REG, BIT)   (((REG) >> (BIT)) & 1U )

/* Multiple Bit Operations */

#define SET_MASK(REG, MASK) ( (REG) |= (MASK) )
#define CLEAR_MASK(REG, MASK) ( (REG) &= ~(MASK) )

/* Extract a value from a specific bit-field */
#define READ_BIT_FIELD(REG, MASK, POS) ( ((REG) >> (POS)) & (MASK) )

/* Write a value into a specific bit-field securely */
#define WRITE_BIT_FIELD(REG, MASK, POS, VALUE) \
    ( (REG) = (((REG) & ~((MASK) << (POS))) | (((VALUE) & (MASK)) << (POS))) )



#endif