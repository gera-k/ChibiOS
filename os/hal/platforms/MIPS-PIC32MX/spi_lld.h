/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    MIPS-PIC32MX/spi_lld.h
 * @brief   MIPS-PIC32MX low level SPI driver header.
 *
 * @addtogroup SPI
 * @{
 */

#ifndef _SPI_LLD_H_
#define _SPI_LLD_H_

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Width of single data transfer.
 */
enum spiDataWidth {
  SPI_DATA_MODE_8_BIT,
  SPI_DATA_MODE_16_BIT,
  SPI_DATA_MODE_32_BIT,
};

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an SPI driver.
 */
typedef struct SPIDriver SPIDriver;

/**
 * @brief   SPI notification callback type.
 *
 * @param[in] spid      pointer to the @p SPIDriver object triggering the
 *                      callback
 */
typedef void (*spicallback_t)(SPIDriver *spid);

/**
 * @brief   SPI chip select toggling callback type.
 *
 */
typedef void (*cscallback_t)(bool_t select);

/**
 * @brief   SPI clock polarity and phase configuration.
 *
 */
enum spiClkMode {
  SPI_CLK_MODE0, /* base(idle) value of clock is low, data is sampled on a leading edge */
  SPI_CLK_MODE1, /* base(idle) value of clock is low, data is sampled on a trailing edge */
  SPI_CLK_MODE2, /* base(idle) value of clock is high, data is sampled on a leading edge */
  SPI_CLK_MODE3, /* base(idle) value of clock is high, data is sampled on a trailing edge */
};

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief Operation complete callback or @p NULL.
   */
  spicallback_t         end_cb;
  /* End of the mandatory fields.*/

  struct {
    /**
     * @brief The chip select line port.
     */
    ioportid_t            port;
    /**
     * @brief The chip select line pad number.
     */
    uint16_t              pad;
  } cs;
  /**
   * @brief The width of single transaction.
   */
  enum spiDataWidth     width;
  /**
   * @brief Indicates master-slave relationship.
   */
  bool_t                master;
  /**
   * @brief Clock speed in Hz units.
   */
  uint32_t              clk;
  /**
   * @brief Clock polarity and phase.
   */
  enum spiClkMode       clk_mode;
  /**
   * @brief SPI RX interrupt.
   */
  uint8_t               rx_irq;
  /**
   * @brief SPIx port.
   */
  volatile void         *base;
} SPIConfig;

/**
 * @brief Structure representing a SPI driver.
 */
struct SPIDriver {
  /**
   * @brief Driver state.
   */
  spistate_t            state;
  /**
   * @brief Current configuration data.
   */
  const SPIConfig       *config;
#if SPI_USE_WAIT || defined(__DOXYGEN__)
  /**
   * @brief Waiting thread.
   */
  Thread                *thread;
#endif /* SPI_USE_WAIT */
#if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief Mutex protecting the bus.
   */
  Mutex                 mutex;
#elif CH_USE_SEMAPHORES
  Semaphore             semaphore;
#endif
#endif /* SPI_USE_MUTUAL_EXCLUSION */
#if defined(SPI_DRIVER_EXT_FIELDS)
  SPI_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/

  /**
   * @brief SPIx port.
   */
  volatile void         *base;
  /**
   * @brief SPI RX interrupt.
   */
  uint8_t               rx_irq;
  /**
   * @brief Number of bytes of the transaction.
   */
  uint16_t              cnt;
  /**
   * @brief Receive pointer or @p NULL.
   */
  uint8_t               *rxptr;
  /**
   * @brief Transmit pointer or @p NULL.
   */
  const uint8_t         *txptr;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void spi_lld_init(void);
  void spi_lld_start(SPIDriver *spid);
  void spi_lld_stop(SPIDriver *spid);
  void spi_lld_select(SPIDriver *spid);
  void spi_lld_unselect(SPIDriver *spid);
  void spi_lld_ignore(SPIDriver *spid, size_t n);
  void spi_lld_exchange(SPIDriver *spid, size_t n,
                        const void *txbuf, void *rxbuf);
  void spi_lld_send(SPIDriver *spid, size_t n, const void *txbuf);
  void spi_lld_receive(SPIDriver *spid, size_t n, void *rxbuf);
  uint32_t spi_lld_polled_exchange(SPIDriver *spid, uint32_t frame);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SPI */

#endif /* _SPI_LLD_H_ */

/** @} */