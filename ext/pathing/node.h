#include "ruby.h"

void define_node_stuff(void);
VALUE Node_initialize(VALUE self, VALUE data);
VALUE Node_get_data(VALUE self);
VALUE Node_add_neighbor(VALUE self, VALUE neighbor, VALUE cost);
VALUE Node_get_neighbors(VALUE self);
VALUE Node_get_neighbors_with_costs(VALUE self);
