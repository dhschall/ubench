

/**
 * Conditional fatal macro that checks the supplied condition and only causes a
 * fatal error if the condition is true and allows the programmer to specify
 * diagnostic printout.  Useful to replace if + fatal, or if + print + assert,
 * etc.
 *
 * @param cond Condition that is checked; if true -> fatal
 * @param ...  Printf-based format string with arguments, extends printout.
 *
 * \def fatal_if(...)
 *
 * @ingroup api_logger
 */
#define fatal_if(cond, ...)                                     \
    do {                                                        \
        if (cond) {                                        \
            std::cerr << # cond << " " << __VA_ARGS__;         \
        }                                                       \
    } while (0)



#endif // __ERROR_HH__