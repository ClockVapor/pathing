# pathing v0.1.1

Dijkstra's algorithm pathfinding in Ruby

## Features

- **Quick** - written in C as a Ruby extension!
- **Flexible** - use your own graph interface!
- **Simple** - describe your graph, call one method, and you have your path!

## Sample Usage

The easiest way to download the gem is to use [Bundler](http://bundler.io/) with a Gemfile. In your Gemfile, include the line `gem 'pathing'`. Then you can run `bundle install` to automatically download and compile the gem for your system. To include the gem in a Ruby file, use the line `require 'pathing'`.

To do some pathfinding, first set up a graph interface object. This object defines the entire layout of your graph. The following example is one way of achieving a two-dimensional grid with neighbors only in the cardinal directions.
```
class Interface
  SIZE = 32

  # gets an array of neighbors for the given node key
  # for this example, the node key is passed in as an array like so: [x, y]
  def neighbors_for(node_key)
    x, y = *node_key
    neighbors = []
    
    neighbors << [x-1, y] if x > 0
    neighbors << [x, y-1] if y > 0
    neighbors << [x+1, y] if x < SIZE-1
    neighbors << [x, y+1] if y < SIZE-1
    
    neighbors
  end
  
  # gets the cost for moving from node1 to node2
  def edge_cost_between(node1_key, node2_key)
    1
  end
end
```

Now, create a `Graph` object referencing your interface object.
```
graph = Pathing::Graph.new(Interface.new)
```

You're all set. Simply call
```
graph.path([0,0], [15, 20])
```
to get your optimized path!

## Credits

Copyright (c) 2015 [Nick Lowery](https://github.com/ClockVapor)

View LICENSE for full license.
