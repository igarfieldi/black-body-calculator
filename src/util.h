#ifndef BLACKBODY_UTIL_H_
#define BLACKBODY_UTIL_H_

#ifdef STATIC_SIZE
#error "Another header defines our STATIC_SIZE macro already"
#endif // STATIC_SIZE

// This macro lets us specify array parameter sizes if we compile with C99
#ifdef __STDC_VERSION__
#if __STDC_VERSION__ >= 199901L
#define STATIC_SIZE(x) static x
#endif // __STDC_VERSION__ >= 199901L
#endif // __STDC_VERSION__

#ifndef STATIC_SIZE
#define STATIC_SIZE(x) 
#endif // STATIC_SIZE


#endif // BLACKBODY_UTIL_H_