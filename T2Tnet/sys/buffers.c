#include "sys.h"
/*
 * Ring_buffer: utilizes a fixed-size buffer 
 * The ring buffer is a FIFO type of buffer
 * 
 * @author: Amjad Yousef Majid
 * @data  : 26/May/2018

 * TODO:
 * Instead of using while and insert the frame byte by byte, use memcpy
*/

#if DEBUG
uint16_t buffer_debug;
#endif

void rbuf_init(rbuf_t *rbuf, uint8_t rbuf_size)
{
  rbuf->buffer = malloc(rbuf_size);
#if DEBUG
  buffer_debug = (uint16_t) rbuf->buffer;
#endif
  rbuf->size = rbuf_size;
  rbuf->head = 0;
  rbuf->tail = 0;
  rbuf->data_len = 0;
}

/** _advance_idx:
 * It is a helper function that advances an index of a ring buffer and set it to
 * zero when it is equal the buffer size
 *
 * @param *idx   *idx pointer to the head/tail ring buffer indices 
 * @param *size  *size pointer to the size of the buffer
 */
void _advance_idx(size_t *idx, size_t *size)
{
    
    if(++(*idx) == *size)
    {
        *idx= 0;
    }
}

/** rbuf_empty:
 * check if the ring buffer is empty
 *
 * @param   *rbuf   pointer to a ring buffer
 * @return  `true value` if the ring buffer is empty
 */
bool rbuf_empty(rbuf_t* rbuf)
{
    return (rbuf->data_len == 0); 
}

/** rbuf_full:
 * check if the ring buffer is full
 *
 * @param   *rbuf   pointer to a ring buffer
 * @return  `true value` if the ring buffer is full
 */
bool rbuf_full(rbuf_t *rbuf)
{
 return ( rbuf->data_len == rbuf->size); 
}

/** rbuf_put:
 * insert a value into the buffer
 *
 * @param *rbuf     pointer to a ring buffer
 * @param data      a byte to be inserted in to the buffer
 */ 
void rbuf_put( rbuf_t *rbuf, uint8_t data)
{
    // insert the data
    rbuf->buffer[rbuf->head] = data;
    // check if old data is overwritten
    if( rbuf_full(rbuf))
    {
        // advances the tail index to point to the old data
        // buffer size equals (max index + 1) 
        _advance_idx(&rbuf->tail, &rbuf->size);
    }
    else
    {
        // increase the length of the data if you do not overwrite old data
        rbuf->data_len++; 
    }
    // if the head index equals the buffer size set it back to zero 
    _advance_idx(&rbuf->head, &rbuf->size);
}

/** rbuf_get:
 * get a data from the buffer if it is not empty and advance tail pointer
 *
 * @param *rbuf     pointer to a ring buffer
 * @param *data     pointer to a variable to get the data from the buffer
 */
bool rbuf_get(rbuf_t * rbuf, uint8_t *data)
{
    if(rbuf_empty(rbuf))
    {
        return (false);
    }
    // get the data
   *data = rbuf->buffer[rbuf->tail] ;
   // decrease the valid length
    rbuf->data_len--;
    _advance_idx(&rbuf->tail, &rbuf->size);
    return true;
}

#if DEBUG
uint8_t data_debug;
#endif


/** rbuf_write:
 * write many bytes to the buffer
 *
 * @param *rbuf     pointer to a ring buffer
 * @param data      a byte to be inserted in to the buffer
 */ 
void rbuf_write( rbuf_t *rbuf, uint8_t *data, uint8_t numBytes)
{
#if DEBUG
    data_debug = *data;
#endif

    while(numBytes--)
    {
        rbuf_put(rbuf, *data);
        data++;
    }
}

/** rbuf_read:
 * read many bytes to the buffer
 *
 * @param *rbuf     pointer to a ring buffer
 * @param data      a byte to be read from the buffer
 */
error_t rbuf_read( uint8_t *data, rbuf_t *rbuf, uint8_t numBytes)
{
    if(rbuf_empty(rbuf))
       {
        return E_BUFFER_EMPTY;
       }

    while( (numBytes--) && rbuf_get(rbuf, data))
    {
        data++;
    }

    return E_SUCCESS;
}







