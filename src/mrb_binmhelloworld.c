/*
** mrb_binmhelloworld.c - BinMhelloworld class
**
** Copyright (c) Uchio Kondo 2021
**
** See Copyright Notice in LICENSE
*/

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/error.h>
#include "mrb_binmhelloworld.h"
#include "helloworld.bpf.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

static void mrb_helloworld_bpf_free(mrb_state *mrb, void *p)
{
  struct helloworld_bpf *obj = p;
  helloworld_bpf__destroy(obj);
}

static const struct mrb_data_type mrb_helloworld_bpf_data_type = {
  "helloworld_bpf", mrb_helloworld_bpf_free,
};

static mrb_value mrb_helloworld_bpf_init(mrb_state *mrb, mrb_value self)
{
  struct helloworld_bpf *data;
  data = (struct helloworld_bpf *)DATA_PTR(self);
  if (data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Cannot initialize twice");
  }
  DATA_TYPE(self) = &mrb_helloworld_bpf_data_type;
  DATA_PTR(self) = NULL;

  data = helloworld_bpf__open();
  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_helloworld_bpf_load(mrb_state *mrb, mrb_value self)
{
  struct helloworld_bpf *data = DATA_PTR(self);
  return mrb_fixnum_value(helloworld_bpf__load(data));
}

static mrb_value mrb_helloworld_bpf_attach(mrb_state *mrb, mrb_value self)
{
  struct helloworld_bpf *data = DATA_PTR(self);
  return mrb_fixnum_value(helloworld_bpf__attach(data));
}

static mrb_value mrb_helloworld_bpf_get_hist(mrb_state *mrb, mrb_value self)
{
  struct helloworld_bpf *data = DATA_PTR(self);
  struct hist hist = {0};
  struct hist_key *init = NULL, prev = {0}, key = {0};
  mrb_value hist = mrb_obj_alloc(mrb, MRB_TT_OBJECT, mrb->object_class);

  data->maps.hists;
  return hists;
}

void mrb_mruby_bin_mhelloworld_gem_init(mrb_state *mrb)
{
  struct RClass *builder;
  builder = mrb_define_class(mrb, "HelloworldBuilder", mrb->object_class);
  mrb_define_method(mrb, builder, "initialize", mrb_helloworld_bpf_init, MRB_ARGS_NONE());
  mrb_define_method(mrb, builder, "load", mrb_helloworld_bpf_load, MRB_ARGS_NONE());
  mrb_define_method(mrb, builder, "attach", mrb_helloworld_bpf_attach, MRB_ARGS_NONE());
  DONE;
}

void mrb_mruby_bin_mhelloworld_gem_final(mrb_state *mrb)
{
}
