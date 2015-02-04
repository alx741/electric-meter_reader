/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <pic16f628a.h>

/*****************************************************************************
                            PIC16F628A CONFIGURATION
         '#pragma config' directive not supported for pic14  SDCC port
*****************************************************************************/

// Use internal oscillator, I/O functions on all the pins
// Watch dog timer disabled
// Power timer disabled
// MCLR pin disabled
// Brown out detection disabled
// Low level programming disabled
// Memory protection disabled
__code char __at(_CONFIG) CONFIG = _INTOSC_OSC_NOCLKOUT & _WDT_OFF &
                                   _PWRTE_OFF & _MCLRE_OFF & _BOREN_OFF &
                                   _LVP_OFF & DATA_CP_OFF & _CP_OFF;

/*****************************************************************************
*****************************************************************************/
