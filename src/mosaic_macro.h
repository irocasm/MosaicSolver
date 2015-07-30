#ifndef MOSAIC_MACRO
#define MOSAIC_MACRO

#define F_NOCOLOUR 0
#define F_BLACK 0x10
#define F_WHITE 0x20
#define F_NUMBER(i, field) ((field[i]) & 0xF)
#define F_GETCOLOUR(i, field) ((field[i]) & 0x30)
#define F_SETCOLOUR(i, field, colour) field[i] = (field[i] & ~0x30) | colour;
#define F_ISCOMPLETE(i, field) (((field[i]) & 0x80) == 0x80)
#define F_MARKCOMPLETE(i, field) field[i] |= 0x80;
#endif
