/*
 * Copyright (c) 2016, M.Naruoka (fenrir)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the naruoka.org nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "telemeter.h"
#include "config.h"
#include "f38x_uart1.h"
#include "util.h"

void telemeter_send(char buf[SYLPHIDE_PAGESIZE]){
  static __xdata u16 sequence_num = 0;
  u16 crc = crc16(buf, SYLPHIDE_PAGESIZE,
      crc16((u8 *)&(++sequence_num), sizeof(sequence_num), 0));
  if(uart1_tx_margin() < (
      sizeof(sylphide_protocol_header) + sizeof(sequence_num)
        + SYLPHIDE_PAGESIZE + sizeof(crc))){
    return;
  }
  uart1_write(sylphide_protocol_header, sizeof(sylphide_protocol_header));
  uart1_write((u8 *)&sequence_num, sizeof(sequence_num));
  uart1_write(buf, SYLPHIDE_PAGESIZE);
  uart1_write((u8 *)&crc, sizeof(crc));
}

void telemeter_init(){
  uart1_bauding(config.baudrate.telemeter);

  data_hub_send_config("TLM.CFG", uart1_write);
}

static void make_packet(packet_t *packet){
  payload_t *dst = packet->current;
  *(dst++) = 'C';

  // read data and store it into packet
  uart1_read(dst, packet->buf_end - dst);
}

void telemeter_polling(){
  u8 buf_size = uart1_rx_size();
  for(; buf_size >= (SYLPHIDE_PAGESIZE - 1); buf_size -= (SYLPHIDE_PAGESIZE - 1)){
    if(!data_hub_assign_page(make_packet)){break;}
  }
}
