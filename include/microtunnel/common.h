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
    // 0x04
    REQ_BIND_UDP,
    REQ_SEND_UDP,
    REQ_CLOSE,
    // 7
    REQ_QUERY_DNS,
    // 8
    RESP_OPEN_TCP,
    // 0x09
    RESP_SEND_TCP,
    // 0x0a
    RESP_RECV_TCP,
    // 0x0b
    RESP_BIND_UDP,
    // 0x0d
    RECV_DATA,
    // 14 (0x0e)
    RESP_CLOSE,
    // 15
    RESP_QUERY_DNS
};

uint16_t a_htons(uint16_t a) {
#ifdef PICO_BUILD
        return (a & 0x00ff) << 8 | (a & 0xff00) >> 8;
#else
        return htons(a);
#endif
}

uint32_t a_htonl(uint32_t a) {
#ifdef PICO_BUILD
        return (a & 0x000000ff) << 24 | (a & 0x0000ff00) << 8 | (a & 0x00ff0000) >> 8 | (a & 0xff000000) >> 24;
#else
        return htonl(a);
#endif
}

uint16_t a_ntohs(uint16_t a) {
#ifdef PICO_BUILD
        return (a & 0x00ff) << 8 | (a & 0xff00) >> 8;
#else
        return htons(a);
#endif
}

uint32_t a_ntohl(uint32_t a) {
#ifdef PICO_BUILD
        return (a & 0x000000ff) << 24 | (a & 0x0000ff00) << 8 | (a & 0x00ff0000) >> 8 | (a & 0xff000000) >> 24;
#else
        return htonl(a);
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

class be_uint32_t {
public:
        be_uint32_t() : be_val_(0) {
        }
        // Transparently cast from uint16_t
        be_uint32_t(const uint32_t &val) : be_val_(a_htonl(val)) {
        }
        // Transparently cast to uint32_t
        operator uint32_t() const {
                return a_ntohl(be_val_);
        }

private:
        uint32_t be_val_;

} __attribute__((packed));


struct RequestOpenTCP {
    be_uint16_t len;
    be_uint16_t type;
    be_uint16_t clientId;
    be_uint32_t addr;
    be_uint16_t port;
};

struct RequestSendTCP {
    be_uint16_t len;
    be_uint16_t type;
    be_uint16_t clientId;
    // WARNING: Content will be of a different length!  This is just defining
    // the maximum area that can be used by the packet.
    uint8_t contentPlaceholder[2048];
};

struct RequestQueryDNS {
    be_uint16_t len;
    be_uint16_t type;
    char name[64];
};

struct RequestBindUDP {
    be_uint16_t len;
    be_uint16_t type;
    be_uint16_t id;
    be_uint16_t bindPort;
};

struct ResponseBindUDP {
    be_uint16_t len;
    be_uint16_t type;
    be_uint16_t id;
    be_uint16_t rc;
};

// Len = 12 + data
struct RequestSendUDP {
    be_uint16_t len;
    be_uint16_t type;
    be_uint16_t id;
    be_uint32_t addr;
    be_uint16_t port;
    // WARNING: Content will be of a different length!  This is just defining
    // the maximum area that can be used by the packet.
    uint8_t data[2048];
};

// Len = 12 + data
struct RecvData {
    be_uint16_t len;
    be_uint16_t type;
    be_uint16_t id;
    be_uint32_t addr;
    be_uint16_t port;
    // WARNING: Content will be of a different length!  This is just defining
    // the maximum area that can be used by the packet.
    uint8_t data[2048];
};

#endif
