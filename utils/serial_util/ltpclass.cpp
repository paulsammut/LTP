#include "LtpClass.h"
#include "SerialClass.h"

        //serialPortLTP.serialRead(packetBuffer, 128, true);

        int packetLength = serialPortLTP.serialGetPacket(packetBuffer,0x00);
        if(packetLength) {
            printf("\r\nPacket received: ");
            //int i;
            //for (i = 0; i < packetLength; i++)
            //    printf(" 0x%02X ", packetBuffer[i]);

            struct LTPSample curSample;
            if(decodePacket(packetBuffer, packetLength,&curSample))
                printf("Angle: %04d distance: %04d\r\n", curSample.angle, curSample.distance);
            fflush(stdout);
        }
        //serialPortLTP.serialRead(packetBuffer, 128, true);

        int packetLength = serialPortLTP.serialGetPacket(packetBuffer,0x00);
        if(packetLength) {
            printf("\r\nPacket received: ");
            //int i;
            //for (i = 0; i < packetLength; i++)
            //    printf(" 0x%02X ", packetBuffer[i]);

            struct LTPSample curSample;
            if(decodePacket(packetBuffer, packetLength,&curSample))
                printf("Angle: %04d distance: %04d\r\n", curSample.angle, curSample.distance);
            fflush(stdout);
        }
