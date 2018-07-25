#include "mac.h"

// Private functions prototype
static void receive();
static void* transmit();
inline static void channel_assessment();


void mac_init()
{
    // initialize the buffers
    rbuf_init(&tx_buf, TX_BUFFER_SIZE);
    rbuf_init(&rx_buf, RX_BUFFER_SIZE);
    rbuf_init(&crc_buf, CRC_BUFFER_SIZE);
    // initialize a buffer to save the valid data
    rbuf_init(&rx_data_buf, RX_BUFFER_SIZE);
}

uint8_t end_mac_flag = false;

inline static void channel_assessment()
{
    set_p3_5();
    start_capture(); 
    slow_timer_delay( (uint16_t) MAC_PREAMBLE_SAMPLING_INTERVAL );
    stop_capture();
    clear_p3_5();
}

static void receive()
{
    start_capture();
    set_p3_6();
    mac_down_cntr((uint16_t) MAC_RX_TIMEOUT_ACLK);
    uint16_t prev_data_len = rbuf_data_len(&rx_data_buf);

    // this while loop must be broken on a frame reception or on a timeout
    while( (rbuf_data_len(&rx_data_buf) - prev_data_len == 0) && (mac_timeout == false) )
    {
//        frame_validation(wait_frame_state);
    }

// Process anything left
    stop_capture();
    while(! rbuf_empty(&rx_buf))
    {
        frame_validation(wait_frame_state);
    }
    clear_p3_6();
//    return preamble_sampling;
}


static void* transmit()
{
    set_p4_2();
    // disable capturing
    stop_capture();
    //check if the transmit buffer is not empty
    if( !rbuf_empty(&tx_buf) )
    {
        //send_frame should return an error code
        backscatter_frame();
    }
    clear_p4_2();
    return preamble_sampling;
}

void* preamble_sampling()
{
    transceiver_enable();
    channel_assessment();
    if(preamble_detected == true)
    {
        mac_down_cntr((uint16_t) LONG_PREAMBLE_INTERVAL);

        while((channel_busy == false) && (mac_timeout == false) )
        {
            channel_assessment();
            __no_operation();
        }
    }

    preamble_detected = false;

    //if the channel is busy
    if(channel_busy == true)
    {
        channel_busy = false;
        // to speedup the transition to the receive state
        receive();
        // to not let all the rely nodes to estimate the channel and backscatter at the same time
        // introduce a random delay
        slow_timer_delay( rand() % 180); // channel assessment should be (180/32768) + (8/10000) = 6.3 msec

        return preamble_sampling;
    }
//    transceiver_disable();
    return transmit;
}

void mac_fsm(func_ptr mac_func)
{
   while(1)
   {
       receive_state();
       // go through mac fsm  until you reach a transmit state
       // upon entering a transmit state set end_mac_flag
       // After leaving the transmit state break the mac fsm
       // do computation and restart the mac fsm
       if(mac_func == transmit)
           {
               end_mac_flag = true;
           }

       mac_func = (func_ptr) (*mac_func)();

       if(end_mac_flag)
           {
               end_mac_flag = false;
               break;
           }
   }
}





































