
# Test Commands

        # Command to request HTTP/1.0 GET from www.google.com
        printf "\x00\x0b\x02\x00\x01\x8e\xfb\x28\x84\x00\x50\x00\x17\x04\x00\x01GET / HTTP/1.0\r\n\r\n" | nc 127.0.0.1 8100

