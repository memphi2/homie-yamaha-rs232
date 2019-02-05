/*
Yamaha AV-Receiver  MQTT <-> RS232 controller based on the Homie MQTT convention
Optimized for RX-V1600 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_MAIN_HPP_
#define INCLUDE_MAIN_HPP_

#define FW_NAME      "homie-yamaha-rs232"
#define FW_VERSION   "0.3.1"

#define SERIAL_BAUD  9600

#define BUFFER_SIZE 256

#define DEBUG_MODE 1

#define STX 0x02
#define ETX 0x03
#define DC1 0x11
#define DC2 0x12
#define DC3 0x13
#define DC4 0x14
#define ASCII_0 0x30
#define ASCII_1 0x31
#define ASCII_2 0x32
#define ASCII_3 0x33
#define ASCII_4 0x34
#define ASCII_5 0x35
#define ASCII_6 0x36
#define ASCII_7 0x37
#define ASCII_8 0x38
#define ASCII_9 0x39
#define ASCII_A 0x41
#define ASCII_B 0x42
#define ASCII_C 0x43
#define ASCII_D 0x44
#define ASCII_E 0x45
#define ASCII_F 0x46
#define DEL 0x7F

#endif  // INCLUDE_MAIN_HPP_
