#include "sys.h"
#include "phy.h"

void radio_init()
{
    transceiver_enable();
    rf_sw_init();
}

/** 
 * @description     Enable the transceiver (set P4.3)
 ----------------------------------------------------------------------------*/
void transceiver_enable()
{
    RADIO_DIR |= ON;            // set the transceiver enable bit to be output
    RADIO_OUT |= ON;            // enable the transceiver
}

/** 
 * @description     disable the transceiver
 ----------------------------------------------------------------------------*/
void transceiver_disable()
{
    RADIO_OUT &= ~ON;            // enable the transceiver
}

/** 
 * @description     set the direction pins for the RF switch (A1, A0) 
 *                  (1,1) => matching circuit (receiving)
 *                  (1,0) => mismatching circuit (backscatter with phase-shift)
 *                  (0,1) => mismatching cirtuit (backscatter)
 ----------------------------------------------------------------------------*/
void rf_sw_init()
{
    RFSW_OUT &= ~(A0 | A1);     // set the output to low
    RFSW_DIR |= (A0 | A1);      //set the two pins as output
}


/**
 * @description     it causes the antenna matching circuit to be mismatched, to backscatter
 * @param           phaseShift indicates with a phase shift is required or not
 ----------------------------------------------------------------------------*/
void backscatter_state(bool phaseShift)
{
    if(phaseShift)
    {
        RFSW_OUT &= ~A0;
        RFSW_OUT |= A1;
    }else{
        RFSW_OUT |= A0;
        RFSW_OUT &= ~A1;
    }
}

/**
 * @description     it causes the antenna matching circuit to be matched, to receive the signal
 ----------------------------------------------------------------------------*/
void receive_state()
{
    RFSW_OUT |= A0 | A1;
   rx_line_init();
}

