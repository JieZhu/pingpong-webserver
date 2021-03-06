#ifndef TIME_H_
#define TIME_H_

#include <sys/time.h>

void set_timestamp(char*, struct timeval*);
void get_timestamp(struct timeval*);
unsigned long get_latency(struct timeval*, struct timeval*);

#endif
