/*
 * halcogen_can.c
 *
 *  Created on: Nov 15, 2019
 *      Author: arrooney
 */
#include "FreeRTOS.h"
#include <stdint.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
//#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "os_semphr.h"
#include "os_task.h"

#include "can.h"
#include <csp/csp.h>
#include <csp/interfaces/csp_if_can.h>
#include "csp/drivers/can.h"

// the following function definitions are as defined by CSP. This file converts their functionality
// to that defined by CSP

int can_send(can_id_t id, uint8_t * data, uint8_t dlc); // The CSP definition of sending a CAN frame
int can_init(uint32_t id, uint32_t mask, struct csp_can_config *conf);

// This sends data byte by byte, dlc should always be 8
int can_send(can_id_t id, uint8_t* data, uint8_t dlc) {

    switch(dlc){
        case 8:
            canUpdateID(canREG2, canMESSAGE_BOX2, id);
            canTransmit(canREG2, canMESSAGE_BOX2, data);
            break;
        case 7:
            canUpdateID(canREG2, canMESSAGE_BOX4, id);
            canTransmit(canREG2, canMESSAGE_BOX4, data);
            break;
        case 6:
            canUpdateID(canREG2, canMESSAGE_BOX6, id);
            canTransmit(canREG2, canMESSAGE_BOX6, data);
            break;
        case 5:
            canUpdateID(canREG2, canMESSAGE_BOX8, id);
            canTransmit(canREG2, canMESSAGE_BOX8, data);
            break;
        case 4:
            canUpdateID(canREG2, canMESSAGE_BOX10, id);
            canTransmit(canREG2, canMESSAGE_BOX10, data);
            break;
        case 3:
            canUpdateID(canREG2, canMESSAGE_BOX12, id);
            canTransmit(canREG2, canMESSAGE_BOX12, data);
            break;
        case 2:
            canUpdateID(canREG2, canMESSAGE_BOX14, id);
            canTransmit(canREG2, canMESSAGE_BOX14, data);
            break;
        case 1:
            canUpdateID(canREG2, canMESSAGE_BOX16, id);
            canTransmit(canREG2, canMESSAGE_BOX16, data);
            break;
        default:
            return 1;
    }

    return 0;
}


static void can_rx_thread(void * parameters)
{
    can_frame_t frame;
    int nbytes;
    uint8_t dlc;
    uint8_t boxnum;

    // TODO: check which message box it's arriving on and
    // change dlc field depending on that.

    while (1) {
        while(!(canIsRxMessageArrived(canREG2, canMESSAGE_BOX1) ||
                canIsRxMessageArrived(canREG2, canMESSAGE_BOX3) ||
                canIsRxMessageArrived(canREG2, canMESSAGE_BOX5) ||
                canIsRxMessageArrived(canREG2, canMESSAGE_BOX7) ||
                canIsRxMessageArrived(canREG2, canMESSAGE_BOX9) ||
                canIsRxMessageArrived(canREG2, canMESSAGE_BOX11) ||
                canIsRxMessageArrived(canREG2, canMESSAGE_BOX13) ||
                canIsRxMessageArrived(canREG2, canMESSAGE_BOX15))){
            vTaskDelay(10);
        }

        if(canIsRxMessageArrived(canREG2, canMESSAGE_BOX1)){
            dlc = 8;
            boxnum = canMESSAGE_BOX1;
        } else if (canIsRxMessageArrived(canREG2, canMESSAGE_BOX3)){
            dlc = 7;
            boxnum = canMESSAGE_BOX3;
        } else if (canIsRxMessageArrived(canREG2, canMESSAGE_BOX5)){
            dlc = 6;
            boxnum = canMESSAGE_BOX5;
        } else if (canIsRxMessageArrived(canREG2, canMESSAGE_BOX7)){
            dlc = 5;
            boxnum = canMESSAGE_BOX7;
        } else if (canIsRxMessageArrived(canREG2, canMESSAGE_BOX9)){
            dlc = 4;
            boxnum = canMESSAGE_BOX9;
        } else if (canIsRxMessageArrived(canREG2, canMESSAGE_BOX11)){
            dlc = 3;
            boxnum = canMESSAGE_BOX11;
        } else if (canIsRxMessageArrived(canREG2, canMESSAGE_BOX13)){
            dlc = 2;
            boxnum = canMESSAGE_BOX13;
        } else if (canIsRxMessageArrived(canREG2, canMESSAGE_BOX15)){
            dlc = 1;
            boxnum = canMESSAGE_BOX15;
        }


        //uint8_t * rx_data = (uint8_t *)pvPortMalloc(8*sizeof(uint8_t));
        uint8_t rx_data[8] = {0};                           //change this to dynamic memory in future
        canGetData(canREG2, boxnum, rx_data);               //should be good
        frame.data[0] = rx_data[0];                         //TODO: does this have to be element-wise?
        frame.data[1] = rx_data[1];
        frame.data[2] = rx_data[2];
        frame.data[3] = rx_data[3];
        frame.data[4] = rx_data[4];
        frame.data[5] = rx_data[5];
        frame.data[6] = rx_data[6];
        frame.data[7] = rx_data[7];
        frame.id = (can_id_t) canGetID(canREG2, boxnum);    //should be good
        frame.dlc = dlc;                                    //should be good

//        if (nbytes < 0) {
//            csp_log_error("read: %s", strerror(errno));
//            continue;
//        }
//
//        if (nbytes != sizeof(frame)) {
//            csp_log_warn("Read incomplete CAN frame");
//            continue;
//        }
//
//        /* Frame type */
//        if (frame.can_id & (CAN_ERR_FLAG | CAN_RTR_FLAG) || !(frame.can_id & CAN_EFF_FLAG)) {
//            /* Drop error and remote frames */
//            csp_log_warn("Discarding ERR/RTR/SFF frame");
//            continue;
//        }

//        /* Strip flags */
        //frame.id &= CAN_EFF_MASK;
//
//        /* Call RX callback */
        csp_can_rx_frame((can_frame_t *)&frame, NULL);
    }

    return 0;
}

int can_init(uint32_t id, uint32_t mask, struct csp_can_config *conf) {
    // must init the can reg.
    // TODO: figure out how to configure halcogen CAN on the fly
    canInit(); // the halcogen call takes no parameters, all configurations are done in the halcogen GUI
    xTaskHandle can_rx;
    xTaskCreate(can_rx_thread, "RX_CAN", 1000, ( void * ) NULL, 0, &can_rx );

    return 0;
}


