#include "custom_data_type.h"
#include "debug.h"

#ifndef BUFFERS_H_
#define BUFFERS_H_

#define RX_BUFFER_SIZE		FRAME_LENGTH * 6					// Buffer size in bytes
#define TX_BUFFER_SIZE		FRAME_LENGTH * 2					// Buffer size in bytes

/* buffers module public functions prototyping */
void rbuf_init(rbuf_t *rbuf, uint8_t rbuf_size);
bool rbuf_empty(rbuf_t* rbuf);
void rbuf_write( rbuf_t *rbuf, uint8_t *data, uint8_t numBytes);
error_t rbuf_read( uint8_t *data, rbuf_t *rbuf, uint8_t numBytes);
uint16_t rbuf_data_len(rbuf_t* rbuf);

#endif
