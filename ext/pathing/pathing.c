#include "pathing.h"

VALUE Pathing = Qnil;

void Init_pathing(void)
{
  Pathing = rb_define_module("Pathing");
  define_graph_stuff();
}

