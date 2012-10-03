#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>
#include <string>
#include "tcp_stream.h"
#include "tcp.h"
#include "utils.h"

using namespace Tins;

class TCPStreamTest : public testing::Test {
public:
    static const size_t num_packets = 20;
    static EthernetII packets[];
    static const size_t indexes[][num_packets];
    static const std::string payload;
    
    static void end_handle(TCPStream& session);
    
    static size_t index;
    static bool processed_stream;
};

size_t TCPStreamTest::index;
bool TCPStreamTest::processed_stream;

EthernetII TCPStreamTest::packets[] = {
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x3c\xfd\x03\x40\x00\x40\x06\x3f\xb6\x7f\x00\x00\x01\x7f\x00\x00\x01\xa3\x78\x0b\xb8\xb1\xe6\x0f\x76\x00\x00\x00\x00\xa0\x02\x80\x18\xfe\x30\x00\x00\x02\x04\x40\x0c\x04\x02\x08\x0a\x2d\x1a\xf2\x75\x00\x00\x00\x00\x01\x03\x03\x04", 74),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x3c\x00\x00\x40\x00\x40\x06\x3c\xba\x7f\x00\x00\x01\x7f\x00\x00\x01\x0b\xb8\xa3\x78\x37\x67\x56\xd3\xb1\xe6\x0f\x77\xa0\x12\x80\x00\xfe\x30\x00\x00\x02\x04\x40\x0c\x04\x02\x08\x0a\x2d\x1a\xf2\x75\x2d\x1a\xf2\x75\x01\x03\x03\x04", 74),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x34\xfd\x04\x40\x00\x40\x06\x3f\xbd\x7f\x00\x00\x01\x7f\x00\x00\x01\xa3\x78\x0b\xb8\xb1\xe6\x0f\x77\x37\x67\x56\xd4\x80\x10\x08\x02\xfe\x28\x00\x00\x01\x01\x08\x0a\x2d\x1a\xf2\x75\x2d\x1a\xf2\x75", 66),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x01\xb1\xfd\x05\x40\x00\x40\x06\x3e\x3f\x7f\x00\x00\x01\x7f\x00\x00\x01\xa3\x78\x0b\xb8\xb1\xe6\x0f\x77\x37\x67\x56\xd4\x80\x18\x08\x02\xff\xa5\x00\x00\x01\x01\x08\x0a\x2d\x1a\xf4\x86\x2d\x1a\xf2\x75\x4c\x6f\x72\x65\x6d\x20\x69\x70\x73\x75\x6d\x20\x64\x6f\x6c\x6f\x72\x20\x73\x69\x74\x20\x61\x6d\x65\x74\x2c\x20\x63\x6f\x6e\x73\x65\x63\x74\x65\x74\x75\x72\x20\x61\x64\x69\x70\x69\x73\x63\x69\x6e\x67\x20\x65\x6c\x69\x74\x2e\x20\x53\x65\x64\x20\x61\x74\x20\x61\x6c\x69\x71\x75\x61\x6d\x20\x61\x72\x63\x75\x2e\x20\x53\x65\x64\x20\x61\x74\x20\x69\x61\x63\x75\x6c\x69\x73\x20\x6d\x61\x67\x6e\x61\x2e\x20\x4e\x61\x6d\x20\x75\x74\x20\x64\x6f\x6c\x6f\x72\x20\x65\x67\x65\x74\x20\x76\x65\x6c\x69\x74\x20\x6d\x61\x74\x74\x69\x73\x20\x70\x6f\x73\x75\x65\x72\x65\x20\x75\x74\x20\x6e\x6f\x6e\x20\x64\x75\x69\x2e\x20\x41\x6c\x69\x71\x75\x61\x6d\x20\x66\x61\x75\x63\x69\x62\x75\x73\x20\x65\x72\x61\x74\x20\x70\x72\x65\x74\x69\x75\x6d\x20\x6c\x69\x67\x75\x6c\x61\x20\x74\x69\x6e\x63\x69\x64\x75\x6e\x74\x20\x65\x67\x65\x74\x20\x74\x72\x69\x73\x74\x69\x71\x75\x65\x20\x6a\x75\x73\x74\x6f\x20\x70\x6c\x61\x63\x65\x72\x61\x74\x2e\x20\x50\x68\x61\x73\x65\x6c\x6c\x75\x73\x20\x74\x75\x72\x70\x69\x73\x20\x74\x65\x6c\x6c\x75\x73\x2c\x20\x6f\x72\x6e\x61\x72\x65\x20\x75\x6c\x74\x72\x69\x63\x69\x65\x73\x20\x65\x67\x65\x73\x74\x61\x73\x20\x76\x69\x74\x61\x65\x2c\x20\x6d\x6f\x6c\x6c\x69\x73\x20\x73\x65\x64\x20\x6e\x65\x71\x75\x65\x2e\x20\x53\x65\x64\x20\x65\x74\x20\x6c\x69\x62\x65\x72\x6f\x20\x69\x6e\x20\x6e\x75\x6e\x63\x20\x70\x68\x61\x72\x65\x74\x72\x61\x20\x61\x75\x63\x74\x6f\x72\x20\x75\x74\x20\x61\x20\x65\x72\x6f\x73\x2e\x20\x4d\x61\x75\x72\x69\x73\x20\x71\x75\x69\x73\x20\x66\x61\x75\x63\x69\x62\x75\x73\x20\x6e\x69\x62\x68\x2e\x20\x0a", 447),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x34\xa4\xa1\x40\x00\x40\x06\x98\x20\x7f\x00\x00\x01\x7f\x00\x00\x01\x0b\xb8\xa3\x78\x37\x67\x56\xd4\xb1\xe6\x10\xf4\x80\x10\x08\x43\xfe\x28\x00\x00\x01\x01\x08\x0a\x2d\x1a\xf4\x86\x2d\x1a\xf4\x86", 66),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x01\xb1\xfd\x06\x40\x00\x40\x06\x3e\x3e\x7f\x00\x00\x01\x7f\x00\x00\x01\xa3\x78\x0b\xb8\xb1\xe6\x10\xf4\x37\x67\x56\xd4\x80\x18\x08\x02\xff\xa5\x00\x00\x01\x01\x08\x0a\x2d\x1a\xf8\xba\x2d\x1a\xf4\x86\x4c\x6f\x72\x65\x6d\x20\x69\x70\x73\x75\x6d\x20\x64\x6f\x6c\x6f\x72\x20\x73\x69\x74\x20\x61\x6d\x65\x74\x2c\x20\x63\x6f\x6e\x73\x65\x63\x74\x65\x74\x75\x72\x20\x61\x64\x69\x70\x69\x73\x63\x69\x6e\x67\x20\x65\x6c\x69\x74\x2e\x20\x53\x65\x64\x20\x61\x74\x20\x61\x6c\x69\x71\x75\x61\x6d\x20\x61\x72\x63\x75\x2e\x20\x53\x65\x64\x20\x61\x74\x20\x69\x61\x63\x75\x6c\x69\x73\x20\x6d\x61\x67\x6e\x61\x2e\x20\x4e\x61\x6d\x20\x75\x74\x20\x64\x6f\x6c\x6f\x72\x20\x65\x67\x65\x74\x20\x76\x65\x6c\x69\x74\x20\x6d\x61\x74\x74\x69\x73\x20\x70\x6f\x73\x75\x65\x72\x65\x20\x75\x74\x20\x6e\x6f\x6e\x20\x64\x75\x69\x2e\x20\x41\x6c\x69\x71\x75\x61\x6d\x20\x66\x61\x75\x63\x69\x62\x75\x73\x20\x65\x72\x61\x74\x20\x70\x72\x65\x74\x69\x75\x6d\x20\x6c\x69\x67\x75\x6c\x61\x20\x74\x69\x6e\x63\x69\x64\x75\x6e\x74\x20\x65\x67\x65\x74\x20\x74\x72\x69\x73\x74\x69\x71\x75\x65\x20\x6a\x75\x73\x74\x6f\x20\x70\x6c\x61\x63\x65\x72\x61\x74\x2e\x20\x50\x68\x61\x73\x65\x6c\x6c\x75\x73\x20\x74\x75\x72\x70\x69\x73\x20\x74\x65\x6c\x6c\x75\x73\x2c\x20\x6f\x72\x6e\x61\x72\x65\x20\x75\x6c\x74\x72\x69\x63\x69\x65\x73\x20\x65\x67\x65\x73\x74\x61\x73\x20\x76\x69\x74\x61\x65\x2c\x20\x6d\x6f\x6c\x6c\x69\x73\x20\x73\x65\x64\x20\x6e\x65\x71\x75\x65\x2e\x20\x53\x65\x64\x20\x65\x74\x20\x6c\x69\x62\x65\x72\x6f\x20\x69\x6e\x20\x6e\x75\x6e\x63\x20\x70\x68\x61\x72\x65\x74\x72\x61\x20\x61\x75\x63\x74\x6f\x72\x20\x75\x74\x20\x61\x20\x65\x72\x6f\x73\x2e\x20\x4d\x61\x75\x72\x69\x73\x20\x71\x75\x69\x73\x20\x66\x61\x75\x63\x69\x62\x75\x73\x20\x6e\x69\x62\x68\x2e\x20\x0a", 447),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x34\xa4\xa2\x40\x00\x40\x06\x98\x1f\x7f\x00\x00\x01\x7f\x00\x00\x01\x0b\xb8\xa3\x78\x37\x67\x56\xd4\xb1\xe6\x12\x71\x80\x10\x08\x86\xfe\x28\x00\x00\x01\x01\x08\x0a\x2d\x1a\xf8\xba\x2d\x1a\xf8\xba", 66),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x35\xfd\x07\x40\x00\x40\x06\x3f\xb9\x7f\x00\x00\x01\x7f\x00\x00\x01\xa3\x78\x0b\xb8\xb1\xe6\x12\x71\x37\x67\x56\xd4\x80\x18\x08\x02\xfe\x29\x00\x00\x01\x01\x08\x0a\x2d\x1b\x00\x4a\x2d\x1a\xf8\xba\x0a", 67),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x34\xa4\xa3\x40\x00\x40\x06\x98\x1e\x7f\x00\x00\x01\x7f\x00\x00\x01\x0b\xb8\xa3\x78\x37\x67\x56\xd4\xb1\xe6\x12\x72\x80\x10\x08\x86\xfe\x28\x00\x00\x01\x01\x08\x0a\x2d\x1b\x00\x4a\x2d\x1b\x00\x4a", 66),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x35\xfd\x08\x40\x00\x40\x06\x3f\xb8\x7f\x00\x00\x01\x7f\x00\x00\x01\xa3\x78\x0b\xb8\xb1\xe6\x12\x72\x37\x67\x56\xd4\x80\x18\x08\x02\xfe\x29\x00\x00\x01\x01\x08\x0a\x2d\x1b\x00\x4a\x2d\x1b\x00\x4a\x0a", 67),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x34\xa4\xa4\x40\x00\x40\x06\x98\x1d\x7f\x00\x00\x01\x7f\x00\x00\x01\x0b\xb8\xa3\x78\x37\x67\x56\xd4\xb1\xe6\x12\x73\x80\x10\x08\x86\xfe\x28\x00\x00\x01\x01\x08\x0a\x2d\x1b\x00\x4a\x2d\x1b\x00\x4a", 66),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x01\xfa\xfd\x09\x40\x00\x40\x06\x3d\xf2\x7f\x00\x00\x01\x7f\x00\x00\x01\xa3\x78\x0b\xb8\xb1\xe6\x12\x73\x37\x67\x56\xd4\x80\x18\x08\x02\xff\xee\x00\x00\x01\x01\x08\x0a\x2d\x1b\x00\x96\x2d\x1b\x00\x4a\x43\x75\x72\x61\x62\x69\x74\x75\x72\x20\x73\x65\x6d\x20\x65\x72\x61\x74\x2c\x20\x62\x69\x62\x65\x6e\x64\x75\x6d\x20\x71\x75\x69\x73\x20\x63\x6f\x6e\x64\x69\x6d\x65\x6e\x74\x75\x6d\x20\x75\x74\x2c\x20\x69\x6d\x70\x65\x72\x64\x69\x65\x74\x20\x61\x74\x20\x65\x73\x74\x2e\x20\x44\x75\x69\x73\x20\x73\x61\x67\x69\x74\x74\x69\x73\x20\x72\x68\x6f\x6e\x63\x75\x73\x20\x66\x65\x6c\x69\x73\x20\x61\x74\x20\x75\x6c\x74\x72\x69\x63\x69\x65\x73\x2e\x20\x49\x6e\x20\x6c\x69\x62\x65\x72\x6f\x20\x75\x72\x6e\x61\x2c\x20\x64\x69\x67\x6e\x69\x73\x73\x69\x6d\x20\x65\x75\x20\x65\x6c\x65\x6d\x65\x6e\x74\x75\x6d\x20\x71\x75\x69\x73\x2c\x20\x63\x6f\x6e\x73\x65\x63\x74\x65\x74\x75\x72\x20\x61\x20\x6e\x65\x71\x75\x65\x2e\x20\x50\x72\x61\x65\x73\x65\x6e\x74\x20\x6c\x65\x6f\x20\x73\x65\x6d\x2c\x20\x63\x75\x72\x73\x75\x73\x20\x73\x65\x64\x20\x6c\x6f\x62\x6f\x72\x74\x69\x73\x20\x73\x69\x74\x20\x61\x6d\x65\x74\x2c\x20\x6f\x72\x6e\x61\x72\x65\x20\x61\x63\x20\x61\x75\x67\x75\x65\x2e\x20\x4d\x61\x75\x72\x69\x73\x20\x74\x72\x69\x73\x74\x69\x71\x75\x65\x20\x73\x65\x6d\x70\x65\x72\x20\x69\x70\x73\x75\x6d\x20\x61\x74\x20\x63\x6f\x6e\x73\x65\x71\x75\x61\x74\x2e\x20\x53\x65\x64\x20\x66\x72\x69\x6e\x67\x69\x6c\x6c\x61\x20\x64\x6f\x6c\x6f\x72\x20\x75\x74\x20\x6c\x61\x63\x75\x73\x20\x73\x61\x67\x69\x74\x74\x69\x73\x20\x71\x75\x69\x73\x20\x75\x6c\x74\x72\x69\x63\x69\x65\x73\x20\x6c\x65\x6f\x20\x76\x75\x6c\x70\x75\x74\x61\x74\x65\x2e\x20\x4d\x61\x65\x63\x65\x6e\x61\x73\x20\x64\x69\x67\x6e\x69\x73\x73\x69\x6d\x20\x69\x6d\x70\x65\x72\x64\x69\x65\x74\x20\x6a\x75\x73\x74\x6f\x2e\x20\x43\x72\x61\x73\x20\x6c\x69\x62\x65\x72\x6f\x20\x6f\x64\x69\x6f\x2c\x20\x76\x65\x68\x69\x63\x75\x6c\x61\x20\x65\x74\x20\x61\x64\x69\x70\x69\x73\x63\x69\x6e\x67\x20\x71\x75\x69\x73\x2c\x20\x6c\x75\x63\x74\x75\x73\x20\x76\x65\x6c\x20\x61\x6e\x74\x65\x2e\x20\x0a", 520),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x34\xa4\xa5\x40\x00\x40\x06\x98\x1c\x7f\x00\x00\x01\x7f\x00\x00\x01\x0b\xb8\xa3\x78\x37\x67\x56\xd4\xb1\xe6\x14\x39\x80\x10\x08\xc9\xfe\x28\x00\x00\x01\x01\x08\x0a\x2d\x1b\x00\x96\x2d\x1b\x00\x96", 66),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x02\x35\xfd\x0a\x40\x00\x40\x06\x3d\xb6\x7f\x00\x00\x01\x7f\x00\x00\x01\xa3\x78\x0b\xb8\xb1\xe6\x14\x39\x37\x67\x56\xd4\x80\x18\x08\x02\x00\x2a\x00\x00\x01\x01\x08\x0a\x2d\x1b\x04\xba\x2d\x1b\x00\x96\x41\x6c\x69\x71\x75\x61\x6d\x20\x69\x6d\x70\x65\x72\x64\x69\x65\x74\x20\x65\x73\x74\x20\x71\x75\x69\x73\x20\x6e\x75\x6e\x63\x20\x6d\x61\x6c\x65\x73\x75\x61\x64\x61\x20\x65\x67\x65\x74\x20\x63\x6f\x6e\x76\x61\x6c\x6c\x69\x73\x20\x74\x65\x6c\x6c\x75\x73\x20\x75\x6c\x6c\x61\x6d\x63\x6f\x72\x70\x65\x72\x2e\x20\x56\x69\x76\x61\x6d\x75\x73\x20\x75\x6c\x6c\x61\x6d\x63\x6f\x72\x70\x65\x72\x20\x65\x72\x6f\x73\x20\x73\x69\x74\x20\x61\x6d\x65\x74\x20\x6f\x64\x69\x6f\x20\x73\x6f\x6c\x6c\x69\x63\x69\x74\x75\x64\x69\x6e\x20\x72\x75\x74\x72\x75\x6d\x2e\x20\x44\x6f\x6e\x65\x63\x20\x70\x65\x6c\x6c\x65\x6e\x74\x65\x73\x71\x75\x65\x20\x66\x61\x75\x63\x69\x62\x75\x73\x20\x6e\x75\x6c\x6c\x61\x2c\x20\x75\x74\x20\x66\x72\x69\x6e\x67\x69\x6c\x6c\x61\x20\x72\x69\x73\x75\x73\x20\x61\x6c\x69\x71\x75\x61\x6d\x20\x65\x67\x65\x74\x2e\x20\x53\x65\x64\x20\x65\x74\x20\x61\x6e\x74\x65\x20\x6d\x69\x2e\x20\x4d\x6f\x72\x62\x69\x20\x61\x20\x74\x75\x72\x70\x69\x73\x20\x65\x74\x20\x74\x65\x6c\x6c\x75\x73\x20\x64\x61\x70\x69\x62\x75\x73\x20\x69\x61\x63\x75\x6c\x69\x73\x2e\x20\x45\x74\x69\x61\x6d\x20\x66\x61\x75\x63\x69\x62\x75\x73\x20\x74\x65\x6c\x6c\x75\x73\x20\x73\x65\x64\x20\x6d\x65\x74\x75\x73\x20\x63\x6f\x6e\x73\x65\x71\x75\x61\x74\x20\x72\x75\x74\x72\x75\x6d\x2e\x20\x46\x75\x73\x63\x65\x20\x73\x69\x74\x20\x61\x6d\x65\x74\x20\x6e\x75\x6c\x6c\x61\x20\x6d\x61\x73\x73\x61\x2c\x20\x74\x65\x6d\x70\x75\x73\x20\x76\x75\x6c\x70\x75\x74\x61\x74\x65\x20\x73\x65\x6d\x2e\x20\x43\x72\x61\x73\x20\x74\x69\x6e\x63\x69\x64\x75\x6e\x74\x20\x71\x75\x61\x6d\x20\x69\x6e\x20\x6c\x69\x62\x65\x72\x6f\x20\x72\x75\x74\x72\x75\x6d\x20\x69\x6e\x74\x65\x72\x64\x75\x6d\x2e\x20\x41\x6c\x69\x71\x75\x61\x6d\x20\x71\x75\x61\x6d\x20\x73\x61\x70\x69\x65\x6e\x2c\x20\x66\x61\x63\x69\x6c\x69\x73\x69\x73\x20\x61\x74\x20\x76\x65\x73\x74\x69\x62\x75\x6c\x75\x6d\x20\x65\x74\x2c\x20\x76\x65\x6e\x65\x6e\x61\x74\x69\x73\x20\x69\x64\x20\x6d\x61\x75\x72\x69\x73\x2e\x20\x4d\x6f\x72\x62\x69\x20\x72\x75\x74\x72\x75\x6d\x20\x67\x72\x61\x76\x69\x64\x61\x20\x75\x6c\x74\x72\x69\x63\x69\x65\x73\x2e\x20\x0a", 579),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x34\xa4\xa6\x40\x00\x40\x06\x98\x1b\x7f\x00\x00\x01\x7f\x00\x00\x01\x0b\xb8\xa3\x78\x37\x67\x56\xd4\xb1\xe6\x16\x3a\x80\x10\x0c\x00\xfe\x28\x00\x00\x01\x01\x08\x0a\x2d\x1b\x04\xba\x2d\x1b\x04\xba", 66),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x02\x6a\xfd\x0b\x40\x00\x40\x06\x3d\x80\x7f\x00\x00\x01\x7f\x00\x00\x01\xa3\x78\x0b\xb8\xb1\xe6\x16\x3a\x37\x67\x56\xd4\x80\x18\x08\x02\x00\x5f\x00\x00\x01\x01\x08\x0a\x2d\x1b\x09\xa2\x2d\x1b\x04\xba\x41\x65\x6e\x65\x61\x6e\x20\x65\x74\x20\x6a\x75\x73\x74\x6f\x20\x75\x74\x20\x6c\x69\x62\x65\x72\x6f\x20\x65\x75\x69\x73\x6d\x6f\x64\x20\x73\x6f\x6c\x6c\x69\x63\x69\x74\x75\x64\x69\x6e\x2e\x20\x4e\x75\x6c\x6c\x61\x6d\x20\x65\x6e\x69\x6d\x20\x64\x75\x69\x2c\x20\x69\x61\x63\x75\x6c\x69\x73\x20\x76\x69\x74\x61\x65\x20\x62\x69\x62\x65\x6e\x64\x75\x6d\x20\x65\x74\x2c\x20\x63\x6f\x6d\x6d\x6f\x64\x6f\x20\x69\x6e\x20\x74\x65\x6c\x6c\x75\x73\x2e\x20\x4e\x75\x6c\x6c\x61\x6d\x20\x65\x67\x65\x74\x20\x70\x75\x72\x75\x73\x20\x6d\x69\x2c\x20\x61\x20\x75\x6c\x6c\x61\x6d\x63\x6f\x72\x70\x65\x72\x20\x6c\x6f\x72\x65\x6d\x2e\x20\x53\x75\x73\x70\x65\x6e\x64\x69\x73\x73\x65\x20\x70\x6f\x74\x65\x6e\x74\x69\x2e\x20\x44\x75\x69\x73\x20\x61\x63\x20\x6a\x75\x73\x74\x6f\x20\x75\x74\x20\x6c\x65\x6f\x20\x65\x75\x69\x73\x6d\x6f\x64\x20\x67\x72\x61\x76\x69\x64\x61\x20\x73\x69\x74\x20\x61\x6d\x65\x74\x20\x61\x74\x20\x6c\x65\x63\x74\x75\x73\x2e\x20\x4c\x6f\x72\x65\x6d\x20\x69\x70\x73\x75\x6d\x20\x64\x6f\x6c\x6f\x72\x20\x73\x69\x74\x20\x61\x6d\x65\x74\x2c\x20\x63\x6f\x6e\x73\x65\x63\x74\x65\x74\x75\x72\x20\x61\x64\x69\x70\x69\x73\x63\x69\x6e\x67\x20\x65\x6c\x69\x74\x2e\x20\x4d\x61\x65\x63\x65\x6e\x61\x73\x20\x73\x65\x64\x20\x61\x72\x63\x75\x20\x76\x69\x74\x61\x65\x20\x6e\x69\x73\x69\x20\x73\x6f\x6c\x6c\x69\x63\x69\x74\x75\x64\x69\x6e\x20\x67\x72\x61\x76\x69\x64\x61\x2e\x20\x4e\x75\x6c\x6c\x61\x20\x66\x61\x63\x69\x6c\x69\x73\x69\x73\x20\x6e\x69\x62\x68\x20\x74\x75\x72\x70\x69\x73\x2e\x20\x4d\x61\x65\x63\x65\x6e\x61\x73\x20\x71\x75\x69\x73\x20\x69\x6d\x70\x65\x72\x64\x69\x65\x74\x20\x61\x72\x63\x75\x2e\x20\x53\x65\x64\x20\x73\x69\x74\x20\x61\x6d\x65\x74\x20\x6e\x75\x6c\x6c\x61\x20\x75\x72\x6e\x61\x2c\x20\x61\x74\x20\x76\x65\x73\x74\x69\x62\x75\x6c\x75\x6d\x20\x6d\x61\x75\x72\x69\x73\x2e\x20\x53\x75\x73\x70\x65\x6e\x64\x69\x73\x73\x65\x20\x71\x75\x69\x73\x20\x65\x6c\x69\x74\x20\x64\x75\x69\x2e\x20\x43\x6c\x61\x73\x73\x20\x61\x70\x74\x65\x6e\x74\x20\x74\x61\x63\x69\x74\x69\x20\x73\x6f\x63\x69\x6f\x73\x71\x75\x20\x61\x64\x20\x6c\x69\x74\x6f\x72\x61\x20\x74\x6f\x72\x71\x75\x65\x6e\x74\x20\x70\x65\x72\x20\x63\x6f\x6e\x75\x62\x69\x61\x20\x6e\x6f\x73\x74\x72\x61\x2c\x20\x70\x65\x72\x20\x69\x6e\x63\x65\x70\x74\x6f\x73\x20\x68\x69\x6d\x65\x6e\x61\x65\x6f\x73\x2e\x20\x0a", 632),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x34\xa4\xa7\x40\x00\x40\x06\x98\x1a\x7f\x00\x00\x01\x7f\x00\x00\x01\x0b\xb8\xa3\x78\x37\x67\x56\xd4\xb1\xe6\x18\x70\x80\x10\x0c\x00\xfe\x28\x00\x00\x01\x01\x08\x0a\x2d\x1b\x09\xa2\x2d\x1b\x09\xa2", 66),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x34\xfd\x0c\x40\x00\x40\x06\x3f\xb5\x7f\x00\x00\x01\x7f\x00\x00\x01\xa3\x78\x0b\xb8\xb1\xe6\x18\x70\x37\x67\x56\xd4\x80\x11\x08\x02\xfe\x28\x00\x00\x01\x01\x08\x0a\x2d\x1b\x0d\xd2\x2d\x1b\x09\xa2", 66),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x34\xa4\xa8\x40\x00\x40\x06\x98\x19\x7f\x00\x00\x01\x7f\x00\x00\x01\x0b\xb8\xa3\x78\x37\x67\x56\xd4\xb1\xe6\x18\x71\x80\x11\x0c\x00\xfe\x28\x00\x00\x01\x01\x08\x0a\x2d\x1b\x0d\xd2\x2d\x1b\x0d\xd2", 66),
    EthernetII((const uint8_t*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00\x00\x34\xfd\x0d\x40\x00\x40\x06\x3f\xb4\x7f\x00\x00\x01\x7f\x00\x00\x01\xa3\x78\x0b\xb8\xb1\xe6\x18\x71\x37\x67\x56\xd5\x80\x10\x08\x02\xfe\x28\x00\x00\x01\x01\x08\x0a\x2d\x1b\x0d\xd2\x2d\x1b\x0d\xd2", 66)
};

const size_t TCPStreamTest::indexes[][num_packets] = {
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19},
    {0,1,2,3,5,4,6,7,8,9,10,11,12,13,14,15,16,17,18,19},
    {0,1,2,5,3,4,6,7,8,9,10,11,12,13,14,15,16,17,18,19},
    {0,1,2,5,3,4,6,7,8,9,10,11,13,15,14,12,16,17,18,19},
    {0,1,2,5,3,4,6,7,8,9,10,15,13,11,14,12,16,17,18,19}
};

const std::string TCPStreamTest::payload = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed at aliquam arcu. Sed at iaculis magna. Nam ut dolor eget velit mattis posuere ut non dui. Aliquam faucibus erat pretium ligula tincidunt eget tristique justo placerat. Phasellus turpis tellus, ornare ultricies egestas vitae, mollis sed neque. Sed et libero in nunc pharetra auctor ut a eros. Mauris quis faucibus nibh. \nLorem ipsum dolor sit amet, consectetur adipiscing elit. Sed at aliquam arcu. Sed at iaculis magna. Nam ut dolor eget velit mattis posuere ut non dui. Aliquam faucibus erat pretium ligula tincidunt eget tristique justo placerat. Phasellus turpis tellus, ornare ultricies egestas vitae, mollis sed neque. Sed et libero in nunc pharetra auctor ut a eros. Mauris quis faucibus nibh. \n\n\nCurabitur sem erat, bibendum quis condimentum ut, imperdiet at est. Duis sagittis rhoncus felis at ultricies. In libero urna, dignissim eu elementum quis, consectetur a neque. Praesent leo sem, cursus sed lobortis sit amet, ornare ac augue. Mauris tristique semper ipsum at consequat. Sed fringilla dolor ut lacus sagittis quis ultricies leo vulputate. Maecenas dignissim imperdiet justo. Cras libero odio, vehicula et adipiscing quis, luctus vel ante. \nAliquam imperdiet est quis nunc malesuada eget convallis tellus ullamcorper. Vivamus ullamcorper eros sit amet odio sollicitudin rutrum. Donec pellentesque faucibus nulla, ut fringilla risus aliquam eget. Sed et ante mi. Morbi a turpis et tellus dapibus iaculis. Etiam faucibus tellus sed metus consequat rutrum. Fusce sit amet nulla massa, tempus vulputate sem. Cras tincidunt quam in libero rutrum interdum. Aliquam quam sapien, facilisis at vestibulum et, venenatis id mauris. Morbi rutrum gravida ultricies. \nAenean et justo ut libero euismod sollicitudin. Nullam enim dui, iaculis vitae bibendum et, commodo in tellus. Nullam eget purus mi, a ullamcorper lorem. Suspendisse potenti. Duis ac justo ut leo euismod gravida sit amet at lectus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas sed arcu vitae nisi sollicitudin gravida. Nulla facilisis nibh turpis. Maecenas quis imperdiet arcu. Sed sit amet nulla urna, at vestibulum mauris. Suspendisse quis elit dui. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. \n";

void data_handle(TCPStream&) {

}

void TCPStreamTest::end_handle(TCPStream& session) {
    processed_stream = true;
    ASSERT_EQ(session.client_payload().size(), payload.size()) << "Sizes differ for index " << index;
    EXPECT_TRUE(std::equal(payload.begin(), payload.end(), session.client_payload().begin())) << "Payload differs for index " << index;
}

TEST_F(TCPStreamTest, FollowStreams) {
    TCPStreamFollower follower;
    for(index = 0; index < (sizeof(indexes) / sizeof(indexes[0])); index++) {
        std::vector<EthernetII> pdus;
        for(size_t i = 0; i < num_packets; ++i)
            pdus.push_back(packets[indexes[index][i]]);
        processed_stream = false;
        follower.follow_streams(pdus.begin(), pdus.end(), data_handle, &TCPStreamTest::end_handle);
        EXPECT_TRUE(processed_stream);
    }
}
