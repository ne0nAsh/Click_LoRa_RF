/****************************************************************************
* Title                 :   LORA RF CLICK ENGINE
* Filename              :   lora_rf_engine.h
* Author                :   MSV
* Origin Date           :   24/11/2015
* Notes                 :   None
*****************************************************************************/
/**************************CHANGE LIST **************************************
*
*    Date    Software Version    Initials   Description
*  24/11/15         2.0            MSV       Interface Created.
*
*****************************************************************************/
/**
 * @file lora.h
 * @brief  HW library
 *
 * @par
 * Hardware and communication control.
 */

/**
 * @page SCH Schematic
 * ![The Schematic](sch.jpg)
 */

/**
 * @page LIB_INFO Library Info
 * @date            28 Jan 2016
 * @author          Milos Vidojevic
 * @copyright       GNU Public License
 * @version         2.1.0
 */

/**
 * @page TEST_CFG Test Configurations
 * Test configuration 1 :
 *
 * @par
 * - <b>MCU</b> :             STM32F107VC
 * - <b>Dev.Board</b> :       EasyMx Pro v7
 * - <b>Oscillator</b> :      72 Mhz internal
 * - <b>Ext. Modules</b> :    LoRa RF Click
 * - <b>SW</b> :              MikroC PRO for ARM 4.9.1
 *
 * Test configuration 2 :
 *
 * @par
 * - <b>MCU</b> :             PIC32MX795F512L</li>
 * - <b>Dev.Board</b> :       EasyPic Fusion v7</li>
 * - <b>Oscillator</b> :      80 Mhz internal</li>
 * - <b>Ext. Modules</b> :    LoRa RF Click</li>
 * - <b>SW</b> :              MikroC PRO for PIC 6.6.2</li>
 */

/**
 * @mainpage
 * ![The Click](lora_rf_click.jpg)
 * ### General Description ###
 *
 * @par
 * LoRa click carries Microchip's RN2483 fully certified LoRa Sub-GHz,
 * 433/868 MHz European R&TTE Directive Assessed Radio Modem.
 * Two antenna connectors allow you to choose which of the two frequency bands
 * will be employed.
 *
 * @par
 * This module handles the LoRaWAN Class A protocol and provides an
 * optimized text command/response interface to the host system. The required
 * configuration for accessing a LoRa technology network is minimal and can
 * be stored in the module’s EEPROM, allowing for factory configuration of these
 * parameters, lowering the requirements for the host system while also
 * increasing system security.
 *
 * @par
 * LoRa RF click communicates with the target board MCU through the mikroBUS
 * UART interface (CTS, RTS, TXD, RXD), with the addition of a Reset pin (RST).
 * The board is designed to use either a 3.3V or a 5V power supply.
 *
 * ### Features ###
 *
 * @par
 * -  LoRaWAN Class A protocol compliance
 * -  Integrated FSK, GFSK and LoRa technology transceiver allowing the
 *      user to transmit custom packets using these protocols
 * -  Globally unique 64-bit identifier (EUI-64)
 * -  Intelligent Low-Power mode with programmable/on-demand wake-up
 * -  Bootloader for firmware upgrade
 * -  All configuration and control done over UART using simple ASCII
 *      commands
 */

/**
 * @page CONF_PAGE Module Communication
 * ### Module Communication ###
 *
 * @par
 * In order to communicate with a LoRa network, a specific number of
 * parameters need to be configured. Since two distinctive methods are offered
 * for a device to become part of the network, each of these requires different
 * parameters:
 * -  Over-the-Air Activation (OTAA), where a device negotiates network
 *      encryption keys at the time it joins the network. For this, the device EUI,
 *      application EUI and application key need to be configured and then the OTAA
 *      procedure can start.
 *
 * -  Activation by Personalization (ABP) where the device already contains
 *      the network keys and can directly start communication with the network.
 *      Configuring the device address, network session key and application session
 *      key is sufficient for this type of initialization.
 *
 * @par
 * All of the RN2483 module’s settings and commands are transmitted over UART
 * using the ASCII interface. he default settings for the UART interface are
 * 57600 bps, 8 bits, no parity, 1 Stop bit, no flow control.
 *
 * ### MikroBUS pins usage ###
 * ![The Pins](mikrobus.png)
 */

#ifndef LORA_RF_ENGINE_H_
#define LORA_RF_ENGINE_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

/**
 * Timer Limit ( ms ) */
#define TIMER_EXPIRED                                       20000
/**
 * Number of Response Indexes */
#define RESP_COUNT                                          19
/**
 * Response String Max Size */
#define MAX_RSP_SIZE                                        20
/**
 * Command String Max Size */
#define MAX_CMD_SIZE                                        64
/**
 * Data String Max Size */
#define MAX_DATA_SIZE                                       256

#define MAX_TRANSFER_SIZE                                   384

/******************************************************************************
* Configuration Constants
*******************************************************************************/

#define LORA_MAC_TX                         "mac tx "
#define LORA_JOIN                           "mac join "
#define LORA_RADIO_TX                       "radio tx "
#define LORA_RADIO_RX                       "radio rx "

/******************************************************************************
* Typedefs
*******************************************************************************/

/**
 * @enum PL_t
 * @brief Payload Type
 *
 * Used by funuction
 *
 * @link lora_mac_tx @endlink
 */
typedef enum
{
    /**
     * Confirmed payload */
    PL_CNF,
    /**
     * Uncofirmed payload */
    PL_UNCNF

}PL_t;

/**
 * @enum JM_t
 * @brief Join Procedure Type
 *
 * @link lora_join @endlink
 */
typedef enum
{
    /**
     * Over the air activation */
    JM_OTAA,
    /**
     * Activation by personalization */
    JM_ABP

}JM_t;

/******************************************************************************
* Variables
*******************************************************************************/

static const char _payload[ 2 ][ 7 ] = {

    "cnf ", "uncnf "

};

static const char _join_mode[ 2 ][ 5 ] = {

    "otaa", "abp"

};

/******************************************************************************
* Function Prototypes
*******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif
/**
 * @brief Initialization
 *
 * Initialization of the module. Performs hardware reset of the module and
 * re-initialization of the all variables inside the library.
 *
 * @note Module response from with current firmware version after every hardware
 * restart - function ignores that, but response if needed can be cached with
 * callback.
 *
 * @param[in] - RTS_line - usage of RTS pin for flow control - firmware on the
 * module is not supporting hardware flow control
 *
 * @param[in] - CTS_line - usage of CTS pin for flow control - firmware on the
 * module is not supporting hardware flow control
 *
 * @param[in] - CB_default -
 *
 * @param[in] - response_p - pointer to user created function which is callback
 * in case that no function is called
 *
 */
void lora_init
(
        bool RTS_line,
        bool CTS_line,
        bool CB_default,
        void ( *response_p )( char *response )
);

/**
 * @brief Main Process
 *
 * Function should be placed inside the infinite while loop or run as thread.
 * That's a must in case of callbacks usage.
 *
 */
void lora_process( void );

/**
 * @brief Receiver
 *
 * Must be placed inside the user made UART interrupt routine.
 *
 * @param[in] rx_input - data from UART receive register
 */
void lora_rx_isr( char rx_input );

/**
 * @brief Timer Tick
 *
 * Used for host timing. Should be placed inside the previously made interrupt
 * routine made by user that occurs on every one milisecond. In case of host
 * watchdog usage that's a must.
 */
void lora_tick_isr( void );

/**
 * @brief Timer Configuration
 *
 * Used to configure host watchdog. When timeout occurs response with no data
 * will be parsed. If user provide 0 as argument timer will be turned off. By
 * default after the initialization timer limit is turned on and set to
 * @link TIMER_EXPIRED @endlink
 *
 * @note
 * RN2483 have it's own watchdog timer, this one should be longer than watchdog
 * and can be used to restart the module in case of when library does not receiving
 * responses from the module.
 *
 * @param[in] timer_limit ( 0 ~ 4294967296 )
 */
void lora_tick_conf
(
        uint32_t timer_limit
);

/**
 * @brief Command
 *
 * Send command to the module
 *
 * @param[in] cmd - well formated command string
 *
 * @param[in] args - in cases when there is no arguments empty string must be
 * provided
 *
 * @param[out] response
 *
 * @return 0 / error codes
 */
void lora_cmd
(
        char *cmd,
        char *args,
        char *response
);

/**
 * @brief MAC Transmission
 *
 * @param[in] payload - payload type @link PL_t @endlink
 *
 * @param[in] port_no - port number ( 1 ~ 223 )
 *
 * @param[in] buffer - data for transmission
 *
 * @param[out] response
 *
 * @return 0 / error codes
 */
int lora_mac_tx
(
        PL_t payload,
        uint8_t port_no,
        char *buffer,
        char *response
);

/**
 * @brief Join Procedure
 *
 * @param[in] join_mode - join procedure type @link JM_t @endlink
 *
 * @param[out] response
 *
 * @return 0 / error codes
 */
int lora_join
(
        JM_t join_mode,
        char *response
);

/**
 * @brief Radio Receive
 *
 * @note
 * MAC must be paused before any radio reception
 *
 * @note
 * Ensure the radio Watchdog Timer time out is higher than the receive
 * window size.
 *
 * @param[in] window_size - 0 represents continuous reception
 *
 * @param[out] response - in case of successful execution represents received
 * data
 *
 * @return 0 / error codes
 */
int lora_rx
(
        uint16_t window_size,
        char *response
);

/**
 * @brief Radio Transmission
 *
 * @note
 * Data must be provided as string where every character represents the
 * hexadecimal digit. Data length allowed is 0 to 255 bytes for LoRa modulation
 * and from 0 to 64 bytes for FSK modulation.
 *
 * @note
 * MAC must be paused before any radio transmission
 *
 * @param[in] buffer - data for transmission
 *
 * @return 0 / error codes
 */
int lora_tx
(
        char *buffer
);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LORA_H

/*** End of File **************************************************************/
