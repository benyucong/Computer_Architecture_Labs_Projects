#include <stdio.h>
#include "bit_ops.h"

/* Returns the Nth bit of X. Assumes 0 <= N <= 31. */
unsigned get_bit(unsigned x, unsigned n) {
    /* YOUR CODE HERE */
    unsigned mask = 1 << n;
    unsigned val = mask & x;
    return val >> n;
}

/* Set the nth bit of the value of x to v. Assumes 0 <= N <= 31, and V is 0 or 1 */
void set_bit(unsigned *x, unsigned n, unsigned v) {
    /* YOUR CODE HERE */
    // Quite simple. (First, clear the nth bit, and set nth bit to 1 if y is 1.)
    *x = (*x & (~(1 << n))) | (v << n);
    return; 
}

/* Flips the Nth bit in X. Assumes 0 <= N <= 31.*/
void flip_bit(unsigned *x, unsigned n) {
    /* YOUR CODE HERE */
    *x ^= (1 << n);
}

