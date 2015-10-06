/*
 * Copyright (C) 2007-2010 STMicroelectronics Ltd
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 */

/* --------------------------------------------------------------------
 * C3 character device driver
 *
 * ST - 2007-05-18
 * Alessandro Miglietti
 * ----------------------------------------------------------------- */

#ifndef __C3_CHAR_DEV_DRIVER_H__
#define __C3_CHAR_DEV_DRIVER_H__

/* --------------------------------------------------------------------
 * INCLUDES
 * ----------------------------------------------------------------  */

#include "c3_common.h"
#include "c3_types.h"

/* --------------------------------------------------------------------
 * FUNCTIONS
 * ----------------------------------------------------------------  */

unsigned int c3_cdd_init(void);

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

void c3_cdd_cleanup(void);

#endif /* __C3_CHAR_DEV_DRIVER_H__ */
