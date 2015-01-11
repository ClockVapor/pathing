#include "graph.h"

extern VALUE Pathing;
extern VALUE Node;
VALUE Graph = Qnil;

void define_graph_stuff(void)
{
  Graph = rb_define_class_under(Pathing, "Graph", rb_cObject);
  rb_define_method(Graph, "initialize", Graph_initialize, 1);
  rb_define_method(Graph, "path", Graph_path, 2);
}

/*
 * Creates a new Graph object with the given interface object.
 */
VALUE Graph_initialize(VALUE self, VALUE interface)
{
  if (!rb_respond_to(interface, rb_intern("neighbors_for")))
  {
    rb_raise(rb_eArgError, "interface must respond to \"neighbors_for\"");
    return Qnil;
  }
 
  if (!rb_respond_to(interface, rb_intern("edge_cost_between")))
  {
    rb_raise(rb_eArgError, "interface must respond to \"edge_cost_between\"");
    return Qnil;
  }

  rb_iv_set(self, "@interface", interface);

  return self;
}

/*
 * Returns the neighbors for a given node key as described by the interface.
 */
VALUE Graph_neighbors(VALUE interface, VALUE key)
{
  return rb_funcall(interface, rb_intern("neighbors_for"), 1, key);
}

/*
 * Returns the edge cost between the two given node keys as described by the
 * interface.
 */
VALUE Graph_edge_cost(VALUE interface, VALUE key1, VALUE key2)
{
  return rb_funcall(interface, rb_intern("edge_cost_between"), 2, key1, key2); 
}

/*
 * Calculates a path from the key1 Node to the key2 node using Dijksta's
 * algorithm. Returns an ordered array of the keys of each Node in the path.
 */
VALUE Graph_path(VALUE self, VALUE key1, VALUE key2)
{
  DPRINT("pathing from %s to %s\n", RSTRING_PTR(rb_inspect(key1)),
    RSTRING_PTR(rb_inspect(key2)));
  VALUE path = rb_ary_new();
  VALUE interface = rb_iv_get(self, "@interface");

  // beware: Dijksta's algorithm ahead

  VALUE settled = rb_ary_new();
  VALUE unsettled = rb_ary_new();
  VALUE prev_nodes = rb_hash_new();
  VALUE costs = rb_hash_new();
  VALUE neighbors;
  VALUE least;
  VALUE args;
  
  // add start node to unsettled
  rb_ary_push(unsettled, key1);
  rb_hash_aset(costs, key1, DBL2NUM(0.0));

  // pathfinding loop
  while (RARRAY_LEN(unsettled) > 0)
  {
    // find least-cost unsettled node and move it to settled
    least = rb_block_call(unsettled, rb_intern("min_by"), 0, NULL,
      value_from_key, costs);
    DPRINT("move %s to settled\n", RSTRING_PTR(rb_inspect(least)));
    rb_ary_delete(unsettled, least);
    if (least == key2) break;
    rb_ary_push(settled, least);

    // update cost for each neighbor of moved node if less than existing cost
   
   args = rb_ary_new();
    rb_ary_push(args, interface);
    rb_ary_push(args, least);
    rb_ary_push(args, costs);
    rb_ary_push(args, prev_nodes);
    rb_ary_push(args, unsettled);
    
    neighbors = Graph_neighbors(interface, least);
    if (neighbors == Qnil)
    {
      rb_raise(rb_eRuntimeError, "node %s neighbor collection is nil",
        RSTRING_PTR(rb_inspect(least)));
      return Qnil;
    }

    rb_block_call(neighbors,
      rb_intern("each"), 0, NULL, update_neighbor_cost, args);
  }

  // build path from end to start
  VALUE curr_node = key2;
  while (curr_node != Qnil)
  {
    rb_ary_unshift(path, curr_node);
    curr_node = rb_hash_aref(prev_nodes, curr_node);
  }

  return path;
}

/*
 * Updates the cost to reach the given node neighbor to the other given node.
 */
VALUE update_neighbor_cost(VALUE neighbor, VALUE args)
{
  VALUE interface = rb_ary_entry(args, 0);
  VALUE node = rb_ary_entry(args, 1);
  VALUE costs = rb_ary_entry(args, 2);
  VALUE prev_nodes = rb_ary_entry(args, 3);
  VALUE unsettled = rb_ary_entry(args, 4);

  float old_cost;
  VALUE rb_old_cost = rb_hash_aref(costs, neighbor);
  if (rb_old_cost != Qnil)
    old_cost = (float) NUM2DBL(rb_old_cost);

  VALUE rb_rel_cost = Graph_edge_cost(interface, node, neighbor);
  if (rb_rel_cost == Qnil)
  {
    rb_raise(rb_eRuntimeError, "cost between nodes %s and %s is nil",
      RSTRING_PTR(rb_inspect(node)), RSTRING_PTR(rb_inspect(neighbor)));
    return Qnil;
  }

  // cost to start node + cost between start and neighbor node
  float new_cost = (float) NUM2DBL(rb_hash_aref(costs, node)) +
    (float) NUM2DBL(Graph_edge_cost(interface, node, neighbor));

  if (rb_old_cost != Qnil)
  {
    old_cost = (float) NUM2DBL(rb_old_cost);
    if (new_cost < old_cost)
    {
      rb_hash_aset(costs, neighbor, DBL2NUM(new_cost));
      rb_hash_aset(prev_nodes, neighbor, node);
      rb_ary_push(unsettled, neighbor);
    }
  }
  else
  {
    rb_hash_aset(costs, neighbor, DBL2NUM(new_cost));
    rb_hash_aset(prev_nodes, neighbor, node);
    rb_ary_push(unsettled, neighbor);
  }

  return Qnil;
}

/*
 * Returns the value in the given hash with the given key. Used as a callback
 * for rb_block_call.
 */
VALUE value_from_key(VALUE key, VALUE hash)
{
  return rb_hash_aref(hash, key);
}
