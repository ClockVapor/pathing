#include "ruby.h"
#include "util.h"

void define_graph_stuff(void);
VALUE Graph_initialize(VALUE self, VALUE interface);
VALUE Graph_neighbors(VALUE interface, VALUE key);
VALUE Graph_edge_cost(VALUE interface, VALUE key1, VALUE key2);
VALUE Graph_path(VALUE self, VALUE key1, VALUE key2);
VALUE update_neighbor_cost(VALUE neighbor, VALUE args);
VALUE value_from_key(VALUE key, VALUE hash);
