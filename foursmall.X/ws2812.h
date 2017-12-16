
#ifndef WS2812_H
#define	WS2812_H

typedef struct {
    unsigned char   r;
    unsigned char   b;
    unsigned char   g;
    
} ws2812_ptr;

// transmit a single WS2812
void		ws2812_send();

#endif	/* WS2812_H */


