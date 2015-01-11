#include "node.h"

extern VALUE Pathing;
VALUE Node = Qnil;

void define_node_stuff(void)
{
  Node = rb_define_class_under(Pathing, "Node", rb_cObject);
  rb_define_method(Node, "initialize", Node_initialize, 1);
  rb_define_method(Node, "data", Node_get_data, 0);
  rb_define_method(Node, "add_neighbor", Node_add_neighbor, 2);
  rb_define_method(Node, "neighbors", Node_get_neighbors, 0);
  rb_define_method(Node, "neighbors_with_costs", Node_get_neighbors_with_costs,
    0);
}

/*
 * Creates a new empty Node.
 */
VALUE Node_initialize(VALUE self, VALUE data)
{
  rb_iv_set(self, "@data", data);
  rb_iv_set(self, "@neighbors", rb_ary_new());
  rb_iv_set(self, "@costs", rb_ary_new());
  return self;
}

/*
 * Gets the data object assigned to this Node.
 */
VALUE Node_get_data(VALUE self)
{
  return rb_iv_get(self, "@data");
}

/*
 * Adds the given Node as a neighbor to this Node with the given cost value.
 */
VALUE Node_add_neighbor(VALUE self, VALUE neighbor, VALUE cost)
{
  rb_ary_push(rb_iv_get(self, "@neighbors"), neighbor);
  rb_ary_push(rb_iv_get(self, "@costs"), cost);
  return neighbor;
}

/*
 * Returns an array of neighbor Nodes to this Node.
 */
VALUE Node_get_neighbors(VALUE self)
{
  return rb_iv_get(self, "@neighbors");
}

/*
 * Returns an array of neighbor Nodes to this node paired with their cost
 * values.
 */
VALUE Node_get_neighbors_with_costs(VALUE self)
{
  VALUE pair_list = rb_ary_new();
  VALUE neighbors = rb_iv_get(self, "@neighbors");
  VALUE costs = rb_iv_get(self, "@costs");
  VALUE pair = Qnil;
  int n = RARRAY_LEN(neighbors);
  int i;

  for (i=0; i<n; i++)
  {
    pair = rb_ary_new();
    rb_ary_push(pair, rb_ary_entry(neighbors, i));
    rb_ary_push(pair, rb_ary_entry(costs, i));
    rb_ary_push(pair_list, pair);
  }

  return pair_list;
}

