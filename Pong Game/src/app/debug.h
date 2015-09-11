/** @file   debug.h
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   26 September 2012
 * 
 *  @brief Debugging macros.
 *
 *  Define debugging macros. If DEBUG is not defined, these do 
 *  nothing. If ENABLE_ASSERT is defined but DEBUG is not, then ASSERT will do
 *  nothing on PC, and just turn on the blue LED on UCFK.
 * 
 *  TRACE(...): Same arguments as printf. On PC, print a message to 
 *              stderr. On UCFK, disable the IR transmitter and send
 *              the message over the serial connection.
 * 
 *  ASSERT(ex): If the expression evaluates to false, print a message containing
 *              the expression, the file name, and the line number. On UCFK,
 *              also turn on the blue LED.
 *  
 */


#ifndef DEBUG_H
#define DEBUG_H


/** @brief Initialise the module. */
void debug_init(void);


#ifdef DEBUG
    
    #define ENABLE_ASSERT
    
    #ifdef __linux__

        #include <stdio.h>

        /* PC */
        #define TRACE(...) do { fprintf(stderr, __VA_ARGS__); } while (0)

    #else
        #ifdef __AVR__

            /* UCFK */
            #include <stdio.h>
            #include "timer0.h"
            #include "pio.h"

            #define TRACE(...) do {timer0_stop(); pio_output_low(IR_TX_HIGH_PIO); printf(__VA_ARGS__); timer0_start();} while (0)
                    
        #endif /* __AVR__ */
    #endif /* __linux__ */

#else

    /* Release mode */
    #define TRACE(fmt, ...) do {} while (0)

#endif /* DEBUG */

#ifdef ENABLE_ASSERT

    void debug_assertion_failure(const char *expression, const char *file, int line);
    #define ASSERT(expr) do {if (!(expr)) {debug_assertion_failure(#expr, __FILE__, __LINE__);} } while (0)

#else

    /* Do nothing with no warnings. */
    #define ASSERT(expr) do { (void)sizeof(expr); } while(0)

#endif /* ENABLE_ASSERT */

#endif /* DEBUG_H */
