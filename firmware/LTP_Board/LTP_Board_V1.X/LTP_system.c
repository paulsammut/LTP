#include <p24FV16KM204.h>
#include "LTP_system.h"
#include "encoder.h"
#include "lidar.h"
#include "motor.h"
#include "PID.h"
#include "mcc_generated_files/tmr1.h"
#include "sweep.h"

#define _DEBUG
#include "dbg.h"


// we have a 16 bit timer with a 1:256 prescaler on a 32MHz clock cycle
// which means a 16 us timer count. This gives us our PID loop time 
// relative to that timer in counts. 0x139 is 5ms, 0x3F is 1ms.
uint16_t pollPeriod = 0x139;

LTP_MODE *LTP_modePtr;
uint16_t *LTP_anglePtr;
uint16_t *LTP_distancePtr;

void LTP_system_init(void) {
    // any system initializations for the LTP

    //All TRIS pins set for ALL peripherals
    TRISBbits.TRISB6 = 0; //output - motor direction
    TRISCbits.TRISC3 = 0; //output - DEBUG_RED led
    TRISAbits.TRISA9 = 0; //output - DEBUG_GREEN led
    TRISCbits.TRISC2 = 0; //output - DP1 port
    TRISCbits.TRISC1 = 0; //output - DP2 port
    TRISBbits.TRISB12 = 0; //output - Slave select of pulse counter
    TRISBbits.TRISB5 = 0; //output - LIDAR Power enable 

    SS_ENCODER = 1; //idle high
    LIDAR1_PE = 1; // turn on lidar
    DEBUG_GREEN = 0;
    DEBUG_RED = 0;

    encoder_init();
    LIDAR_init();
    motor_init();
    PID_init();

}

void LTP_setPtrs(LTP_MODE *_modePtr, uint16_t *_anglePtr, uint16_t *_distancePtr) {
    LTP_modePtr = _modePtr;
    LTP_anglePtr = _anglePtr;
    LTP_distancePtr = _distancePtr;
}

void LTP_sampleAndSend(void) {
    encoder_updateAngle();
    LIDAR_updateDistance();
    dbg_printf("Angle is: %u, and distance is: %u\r\n", *LTP_anglePtr, *LTP_distancePtr);
}

void LTP_poll(void) {
    
    if (TMR1 >= pollPeriod) {
        // we run the loop!

        // Here we just reset the timer flag.
        IFS0bits.T1IF = false;
        
        switch(*LTP_modePtr){
            case IDLE   :
                // do nothing!
                break;
            case SPIN   :
                // not much to do here either, we are spinning
                break;
            case SETPOINT :
                // ok now something for us! SETPOINT mode means we just run the 
                // PID_cycle function. We are confident that whoever put us in setpoint
                // mode also set the setpoint. haha!
                PID_cycle();
                break;
            case SWEEP:
                // almost same deal as SETPOINT, with the exception that we have a master
                // sweep control loop that sets the setpoint that we also have to run. Cool!
                sweep_cycle();
                PID_cycle();
                break;
               
        }
        TMR1 = 0x0000;
    }
}
