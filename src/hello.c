#include <asm/types.h>
#define MAX_SLOTS	15
struct hist {
  __u32 slots[MAX_SLOTS];
};

#include "helloworld.bpf.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>

int libbpf_print_fn(enum libbpf_print_level level,
        const char *format, va_list args)
{
  if (level == LIBBPF_DEBUG)
    return 0;
  return vfprintf(stderr, format, args);
}

int main(int argc, char **argv)
{
  libbpf_set_print(libbpf_print_fn);

  int err, i;
  struct helloworld_bpf *obj = helloworld_bpf__open();
  if (!obj) {
    fprintf(stderr, "failed to open BPF object\n");
    exit(1);
  }

  if(argc > 1)
    obj->rodata->targ_pid = (pid_t)strtol(argv[1], NULL, 10);

  err = helloworld_bpf__load(obj);
  if (err) {
    fprintf(stderr, "failed to load BPF programs\n");
    goto cleanup;
  }

  err = helloworld_bpf__attach(obj);
  if (err) {
    fprintf(stderr, "failed to attach BPF programs\n");
    goto cleanup;
  }

  printf("Tracing vfs_read in 3 secs.\n");
  for (i = 0; i < 3; i++) {
    printf(".\n");
    sleep(1);
  }
  printf("----\n");

  {
    struct bpf_map *hists = obj->maps.hists;
    int fd = bpf_map__fd(hists);
    __u32 lookup_key = -2, next_key;
    struct hist h;

    while (!bpf_map_get_next_key(fd, &lookup_key, &next_key)) {
      err = bpf_map_lookup_elem(fd, &next_key, &h);
      if (err < 0) {
        fprintf(stderr, "failed to lookup hist: %d\n", err);
        return -1;
      }
      printf("pid: %d\n", next_key);
      for(i = 0; i < MAX_SLOTS; i++)
        printf("\tvalue: hist[%d] = %d\n", i, h.slots[i]);

      lookup_key = next_key;
    }
  }

 cleanup:
  helloworld_bpf__destroy(obj);
  return err != 0;
}
