#include "pathing.h"

VALUE Pathing = Qnil;

void Init_pathing(void)
{
  Pathing = rb_define_module("Pathing");
  define_node_stuff();
  define_graph_stuff();
}

