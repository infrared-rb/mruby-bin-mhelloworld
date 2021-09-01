#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

#define MAX_ENTRIES	10240
#define MAX_SLOTS	15

// https://github.com/iovisor/bcc/blob/master/libbpf-tools/bits.bpf.h
static __always_inline u64 log2(u32 v)
{
  u32 shift, r;

  r = (v > 0xFFFF) << 4; v >>= r;
  shift = (v > 0xFF) << 3; v >>= shift; r |= shift;
  shift = (v > 0xF) << 2; v >>= shift; r |= shift;
  shift = (v > 0x3) << 1; v >>= shift; r |= shift;
  r |= (v >> 1);

  return r;
}

static __always_inline u64 log2l(u64 v)
{
  u32 hi = v >> 32;

  if (hi)
    return log2(hi) + 32;
  else
    return log2(v);
}

struct hist {
  __u32 slots[MAX_SLOTS];
};

const volatile pid_t targ_pid = -1;
static struct hist initial_hist = {0};

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __uint(max_entries, MAX_ENTRIES);
  __type(key, u32);
  __type(value, struct hist);
  __uint(map_flags, BPF_F_NO_PREALLOC);
} hists SEC(".maps");

SEC("kretprobe/vfs_read")
int BPF_KRETPROBE(vfs_read, ssize_t ret)
{
  if (ret < 0)
    goto cleanup;

  u32 pid;
  u64 slot;
  struct hist *histp;

  pid = bpf_get_current_pid_tgid();
  if(targ_pid != -1 && pid != (u32)targ_pid)
    goto cleanup;

  histp = bpf_map_lookup_elem(&hists, &pid);
  if (!histp) {
    bpf_map_update_elem(&hists, &pid, &initial_hist, 0);
    histp = bpf_map_lookup_elem(&hists, &pid);
    if (!histp)
      goto cleanup;
  }

  slot = log2l(ret);
  if (slot >= MAX_SLOTS)
    slot = MAX_SLOTS - 1;
  __sync_fetch_and_add(&histp->slots[slot], 1);
  //bpf_map_update_elem(&hists, &pid, histp, 0);

cleanup:
  return 0;
}

char LICENSE[] SEC("license") = "GPL";
