/*
 * Copyright (C) 2011 Gautier Hattenberger <gautier.hattenberger@enac.fr>
 *               2013 Felix Ruess <felix.ruess@gmail.com>
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * @file peripherals/lsm303dlhc.h
 *
 * Driver for ST LSM303DLHC 3D accelerometer and magnetometer.
 */

#ifndef LSM303DLHC_H
#define LSM303DLHC_H

#include "std.h"
#include "mcu_periph/spi.h"
#include "math/pprz_algebra_int.h"
#include "peripherals/lsm303dlhc.h"

struct Lsm303dlhc_Spi {
  struct spi_periph *spi_p;
  struct spi_transaction spi_trans;
  bool initialized;                 ///< config done flag
  enum Lsm303dlhcTarget target;
  volatile uint8_t tx_buf[2];
  volatile uint8_t rx_buf[8];
  enum Lsm303dlhcConfStatus init_status;
  volatile bool data_available_acc;     ///< data ready flag accelero
  volatile bool data_available_mag;     ///< data ready flag magneto
  union {
    struct Int16Vect3 vect;           ///< data vector in acc coordinate system
    int16_t value[3];                 ///< data values accessible by channel index
  } data_accel;
  union {
    struct Int16Vect3 vect;           ///< data vector in mag coordinate system
    int16_t value[3];                 ///< data values accessible by channel index
  } data_mag;
  union {
    struct Lsm303dlhcAccConfig acc;
    struct Lsm303dlhcMagConfig mag;
  } config;
};

// TODO IRQ handling

// Functions
extern void lsm303dlhc_spi_init(struct Lsm303dlhc_Spi *lsm, struct spi_periph *spi_p, uint8_t slave_idx,
                                enum Lsm303dlhcTarget target);
extern void lsm303dlhc_spi_start_configure(struct Lsm303dlhc_Spi *lsm);
extern void lsm303dlhc_spi_read(struct Lsm303dlhc_Spi *lsm);
extern void lsm303dlhc_spi_event(struct Lsm303dlhc_Spi *lsm);

/// convenience function: read or start configuration if not already initialized
static inline void lsm303dlhc_spi_periodic(struct Lsm303dlhc_Spi *lsm)
{
  if (lsm->initialized) {
    lsm303dlhc_spi_read(lsm);
  } else {
    lsm303dlhc_spi_start_configure(lsm);
  }
}

#endif /* LSM303DLHC_H */
