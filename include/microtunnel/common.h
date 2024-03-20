/**
 * Copyright (C) 2024, Bruce MacKinnon 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * NOT FOR COMMERCIAL USE WITHOUT PERMISSION.
 */
#ifndef _microtunnel_h
#define _microtunnel_h

#include <cstdint>

enum ClientFrameType {
    REQ_PING,
    REQ_RESET,
    // 2
    REQ_OPEN_TCP,
    // 3
    REQ_SEND_TCP,
    REQ_OPEN_UDP,
    REQ_SEND_UDP,
    REQ_CLOSE,
    // 7
    REQ_QUERY_DNS,
    // 8
    RESP_OPEN_TCP,
    RESP_SEND_TCP,
    RESP_RECV_TCP,
    RESP_OPEN_UDP,
    RESP_SEND_UDP,
    RESP_RECV_UDP,
    // 14
    RESP_CLOSE,
    // 15
    RESP_QUERY_DNS
};

uint16_t a_htons(uint16_t a) {
#ifdef PICO_BUILD
        return (a & 0x00ff) << 8 | (a & 0xff00) >> 8;
#else
        retrun htons(a);
#endif
}

uint16_t a_ntohs(uint16_t a) {
#ifdef PICO_BUILD
        return (a & 0x00ff) << 8 | (a & 0xff00) >> 8;
#else
        retrun htons(a);
#endif
}

class be_uint16_t {
public:
        be_uint16_t() : be_val_(0) {
        }
        // Transparently cast from uint16_t
        be_uint16_t(const uint16_t &val) : be_val_(a_htons(val)) {
        }
        // Transparently cast to uint16_t
        operator uint16_t() const {
                return a_ntohs(be_val_);
        }

private:
        uint16_t be_val_;

} __attribute__((packed));

struct RequestSendTCP {
    be_uint16_t len;
    be_uint16_t type;
    be_uint16_t clientId;
    uint8_t contentPlaceholder[2048];
};

#endif
