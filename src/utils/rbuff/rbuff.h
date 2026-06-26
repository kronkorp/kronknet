/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Header file for KRONKNET, a simple ring-buffer library in C.
*/
#ifndef KRONKNET_RBUFF_IMPL_H
    #define KRONKNET_RBUFF_IMPL_H
    #include <stddef.h>
    #include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct  KRONKNET ring buffer structure. Store informations.
 *
 * @note   Instead of doing '%' to know the position on the circular buffer,
 *         we can use '&' since the size will be a power of 2. 
*/
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_ring_buffer_s {

    uint8_t* raw_buffer;  //!< The raw buffer (generic)
    size_t   size;        //!< The size of the buffer
    size_t   mask;        //!< The mask of the buffer to apply the binary and
    size_t   reader;      //!< The index of the reader
    size_t   writer;      //!< The index of the writer

} knRBuff;
///////////////////////////////////////////////////////////////////////////////

#endif /* KRONKNET_RBUFF_IMPL_H */
