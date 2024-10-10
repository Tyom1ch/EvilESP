// main.h
#ifndef MAIN_H
#define MAIN_H


#define MAIN_WIDTH 128
#define MAIN_HEIGHT 48

static const unsigned char mainui[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x8, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x18, 0x10, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x28, 0x8, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x9, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x48, 0x4, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x12, 0x50, 0x0, 0x0, 0x0, 0x0, 0xf8, 0x0, 0x84, 0x4, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2a, 0x98, 0x0, 0x0, 0x0, 0x0, 0x87, 0xc1, 0x4, 0x2, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x66, 0xa4, 0x0, 0x0, 0x0, 0x0, 0x40, 0x3e, 0x4, 0x1, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x51, 0xa4, 0x0, 0x0, 0x0, 0x0, 0x44, 0x0, 0x4, 0x1, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5c, 0x7c, 0x0, 0x0, 0x0, 0x0, 0x23, 0x0, 0x2, 0x0, 0x90, 0x0, 0x0, 0x0, 0x2, 0x0, 0x47, 0x4, 0x0, 0x0, 0x0, 0x0, 0x20, 0xc0, 0x1, 0x0, 0x63, 0x0, 0x0, 0x0, 0x0, 0x0, 0x51, 0xfc, 0x0, 0x0, 0x1, 0x0, 0x10, 0x20, 0x0, 0x80, 0x4, 0x80, 0x0, 0x0, 0x8, 0x80, 0x2c, 0x8, 0x0, 0x0, 0x2, 0x80, 0x10, 0x40, 0x0, 0x80, 0x8, 0x80, 0x0, 0x10, 0x0, 0x0, 0x1b, 0x58, 0x30, 0x0, 0x4, 0x80, 0x8, 0x80, 0x4, 0x40, 0x8, 0x80, 0x0, 0x0, 0x2, 0x0, 0xa, 0xa4, 0xc8, 0x0, 0x5, 0x0, 0x8, 0x2, 0xe, 0x40, 0x9, 0x0, 0x0, 0x10, 0x0, 0x0, 0x7, 0xc3, 0x4, 0x0, 0x9, 0x0, 0x4, 0x7, 0xc, 0x40, 0x6, 0x0, 0x0, 0xaa, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x9, 0x0, 0x4, 0x6, 0xd, 0x40, 0x0, 0x0, 0x0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x2, 0x0, 0x9, 0x0, 0x4, 0x1e, 0x86, 0x28, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x2, 0x0, 0x9, 0x0, 0x4, 0x7, 0x0, 0x10, 0x0, 0x0, 0x0, 0x10, 0x1, 0x8, 0x0, 0x0, 0x0, 0x0, 0x8, 0x80, 0x4, 0x0, 0x0, 0x18, 0x0, 0x0, 0x4, 0x0, 0x1, 0x4, 0x70, 0x0, 0x0, 0x0, 0x4, 0x80, 0x2, 0x0, 0x0, 0x10, 0x0, 0x0, 0xa, 0x0, 0x0, 0x82, 0x88, 0x0, 0x3c, 0x0, 0x4, 0x40, 0x3, 0x0, 0x0, 0x20, 0x0, 0x0, 0x4, 0x0, 0x0, 0x41, 0x8, 0x3c, 0x42, 0x0, 0x2, 0x20, 0x1c, 0x0, 0x0, 0x40, 0x0, 0x0, 0x0, 0x0, 0x8, 0x41, 0x4, 0x42, 0x41, 0x0, 0x1, 0x10, 0xe0, 0x0, 0x1, 0x80, 0x0, 0x0, 0x0, 0x0, 0x25, 0xf0, 0x84, 0x41, 0x41, 0x0, 0x0, 0x8f, 0x0, 0x0, 0x1f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x12, 0x8, 0x82, 0x21, 0x81, 0x0, 0x0, 0x80, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0x4, 0x82, 0x21, 0x2, 0x0, 0x0, 0x40, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0, 0x1, 0x0, 0x30, 0x2, 0x82, 0x41, 0x2, 0x0, 0x0, 0x40, 0x0, 0x0, 0x10, 0x0, 0x0, 0x0, 0x1, 0xc0, 0x40, 0x3, 0x1, 0x42, 0x2, 0x0, 0x0, 0x20, 0x0, 0x0, 0x8, 0x0, 0x0, 0x0, 0x1, 0x30, 0x86, 0x1, 0x0, 0x80, 0x4, 0x8, 0x0, 0x20, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x1, 0xd, 0x1, 0x1, 0x0, 0x0, 0x4, 0x14, 0x0, 0x40, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x1, 0x2, 0x0, 0x1, 0x0, 0x0, 0x4, 0x12, 0x0, 0x40, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0, 0x8, 0x11, 0x0, 0x40, 0x1, 0xe0, 0xa1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0, 0x8, 0x9, 0x0, 0x80, 0x2, 0x10, 0x51, 0x0, 0x0, 0x1, 0x0, 0x80, 0x0, 0x20, 0x0, 0x0, 0x10, 0x9, 0x0, 0x80, 0x4, 0x10, 0x4e, 0x0, 0x0, 0x2, 0x80, 0x80, 0x0, 0x10, 0x0, 0x0, 0x8, 0x9, 0x0, 0x80, 0x4, 0x8, 0x40, 0x0, 0x0, 0x1, 0x0, 0x80, 0x0, 0x10, 0x0, 0x0, 0x8, 0x11, 0x0, 0x81, 0x8, 0x8, 0x80, 0x0, 0x0, 0x0, 0x0, 0x40, 0x0, 0x10, 0x0, 0x0, 0x4, 0x22, 0x1, 0x3, 0x8, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x24, 0x0, 0x0, 0x0, 0x0, 0x3, 0xc4, 0x1, 0x5, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x18, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x1, 0x4, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x0, 0x84, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x40, 0x0, 0x4, 0x0, 0x3, 0xc, 0x60, 0x0, 0x78, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x0, 0x8, 0x0, 0xc, 0x3, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x60, 0x0, 0x34, 0x1, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xc3, 0xfe, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

#endif // MAIN_H