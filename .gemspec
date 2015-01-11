Gem::Specification.new do |s|
  s.name = 'pathing'
  s.version = '0.1.1'
  s.licenses = ['MIT']
  s.summary = "Quick and flexible Dijkstra's algorithm pathfinding"
  s.description = s.summary
  s.authors = ['Nick Lowery']
  s.extensions << 'ext/pathing/extconf.rb'
  s.email = 'nick.a.lowery@gmail.com'
  s.files = Dir.glob(["ext/**/*", 'Gemfile*', 'Rakefile',
    'LICENSE', 'README.md'])
  s.homepage = ''
  s.add_development_dependency 'rake-compiler'
end

