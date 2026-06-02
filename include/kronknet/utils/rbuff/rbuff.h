/*
** EPITECH PROJECT, 2026
** ISILDUR
** File description:
** Header file for isildur, a simple ring-buffer library in C.
*/

#ifndef KRONKNET_RBUFF_H
    #define KRONKNET_RBUFF_H
    #include <stddef.h>
    #include <stdint.h>
    #include <sys/types.h>

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct  Isildur ring buffer structure. Store informations.
 *
 * @note   Instead of doing '%' to know the position on the circular buffer,
 *         we can use '&' since the size will be a power of 2. 
*/
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_ring_buffer_s {

    uint8_t *raw_buffer;  //!< The raw buffer (generic)
    size_t   size;        //!< The size of the buffer
    size_t   mask;        //!< The mask of the buffer to apply the binary and
    size_t   reader;      //!< The index of the reader
    size_t   writer;      //!< The index of the writer

} knRBuff;
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
* @brief       Create an isildur ring buffer with the given size
*
* @param size  The size of the buffer. /!\Must be a power of 2/!\
* @return      The allocated isdrbuff instance, or NULL if an error occurs.
*/
///////////////////////////////////////////////////////////////////////////////
knRBuff *isdrbuff_create(size_t size);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
* @brief       Initialize an isildur ring buffer
*
* @param buff  The isdrbuff instance to initialize.
* @param size  The size of the buffer. /!\Must be a power of 2/!\
* @return      0 on success, -1 otherwise, and errno set.
*/
///////////////////////////////////////////////////////////////////////////////
int isdrbuff_init(knRBuff *buff, size_t size);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
* @brief       Destroy an allocated isdrbuff instance
*
* @param buff  The instance that will be destroy 
* @return      Returns nothing.
*/
///////////////////////////////////////////////////////////////////////////////
void isdrbuff_destroy(knRBuff *buff);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
* @brief       Free the internal raw_buffer
*
* @param buff  The instance to clean.
* @return      Returns nothing
*/
///////////////////////////////////////////////////////////////////////////////
void isdrbuff_clean(knRBuff *buff);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
* @brief       Get the remaining size in an isdrbuff 
*
* @param buff  The ring buffer to get remaining size
* @return      The remaining size.
*/
///////////////////////////////////////////////////////////////////////////////
size_t isdrbuff_remaining(const knRBuff *buff);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
* @brief       Get the using size in an isdrbuff 
*
* @param buff  The ring buffer to get using size
* @return      The using size.
*/
///////////////////////////////////////////////////////////////////////////////
size_t isdrbuff_usage(const knRBuff *buff);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
* @brief       Push data into isildur ring buffer
*
* @param buff  The ring buffer in which the data will be push
* @param src   The ptr to the memory zone to copy into the ring buffer
* @param size  The size of the data to copy. Must be lower than remaining size
* @return      The size write on success, -1 otherwise
*/
///////////////////////////////////////////////////////////////////////////////
ssize_t isdrbuff_push(knRBuff *buff, const uint8_t *src, size_t size);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
* @brief       Read data from an isildur ring buffer
*
* @param buff  The ring buffer from which the datas will be pop
* @param dest  The ptr to the memory zone to write the data
* @param size  The size of the data to pop.
* @return      The size read on success, -1 otherwise
*/
///////////////////////////////////////////////////////////////////////////////
ssize_t isdrbuff_pop(knRBuff *buff, uint8_t *dest, size_t size);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
* @brief       Peek data from an isildur ring buffer without consuming it
*
* @details     Copies 'size' bytes from the ring buffer into 'dest' without
*              advancing the internal reader pointer. The data remains 
*              available for subsequent read or pop operations.
*
* @param buff  The ring buffer from which the data will be peeked (read-only)
* @param dest  The pointer to the destination memory zone to write the data
* @param size  The size of the data to peek
* @return      The size peeked on success, -1 otherwise (sets errno)
*/
///////////////////////////////////////////////////////////////////////////////
ssize_t isdrbuff_peek(const knRBuff *buff, uint8_t *dest, size_t size);
///////////////////////////////////////////////////////////////////////////////

#endif /* KRONKNET_RBUFF_H */
