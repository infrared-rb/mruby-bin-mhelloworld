#ifndef BPF_TYPES_H
#define BPF_TYPES_H

#ifndef __BPF_HELPERS__
#include <linux/types.h>
#endif

#define MAX_SLOTS	15
struct hist {
  __u32 slots[MAX_SLOTS];
};

#endif
