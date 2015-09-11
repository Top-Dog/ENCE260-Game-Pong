/** @file debug.c
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   3 October 2012
 */

#include "debug.h"

#ifdef DEBUG

    #ifdef __linux__
        
        /* PC */
        #pragma message("Defining empty debug_init for PC")
        void debug_init(void) {
        }

    #elif defined  __AVR__

        /* UCFK */
        #include <stdio.h>
        #include "timer0.h"
        #include "ir_uart.h"
        
        #pragma message("Defining ir debug_init for UCFK")
        
        static int debug_putc(char ch, __unused__ FILE *f) {
            ir_uart_putc(ch);
            return 0;
        }
        
        void debug_init(void) {
            system_init();
            ir_uart_init();
            fdevopen(debug_putc, NULL);
        }
    
    #endif

#else

    /* Release mode */
    #pragma message("Defining empty debug_init for release")
    void debug_init(void) {
    }

#endif // DEBUG

#ifdef ENABLE_ASSERT

    #ifdef __linux__

        #pragma message("Defining debug_assertion_failure for PC.")
        void debug_assertion_failure(const char *expr, const char *file, int line) {
            fprintf(stderr, "Assertion \"%s\" failed at %s:%i.\n", expr, file, line);
            exit(1);
        }

    #elif defined __AVR__

        #include <stdio.h>
        #include "system.h"
        #include "pio.h"
        
        #pragma message("Defining debug_assertion_failure for UCFK.")
        void debug_assertion_failure(const char *expr, const char *file, int line) {
            TRACE("Assertion \"%s\" failed at %s:%i.\n", expr, file, line);
            pio_config_set(LED1_PIO, PIO_OUTPUT_HIGH);
        }

    #endif

#endif
