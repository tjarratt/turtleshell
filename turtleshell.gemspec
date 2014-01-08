Gem::Specification.new do |s|
  s.name = 'turtleshell'
  s.version = '1.0.6'
  s.date = '2013-11-05'
  s.summary = 'A ruby wrapper for librtlsdr'
  s.description = 'ruby bindings for librtlsdr -- realtek USB software defined radio devices'
  s.authors = ['Tim Jarratt']
  s.email = 'tjarratt@gmail.com'
  s.files = ['lib/turtleshell.rb']
  s.homepage = 'http://github.com/tjarratt/turtleshell'
  s.license = 'MIT'
  s.require_path = 'lib'

  s.extensions = ['ext/librtlsdr/extconf.rb']

  s.add_dependency 'rake',              '~> 10.1'
  s.add_dependency 'rake-compiler',     '~> 0.9'
  s.add_development_dependency 'rspec', '~> 2.14'

  ignores  = File.readlines('.gitignore').grep(/\S+/).map(&:chomp)
  dotfiles = %w[.gitignore]

  all_files_without_ignores = Dir['**/*'].reject { |f|
    File.directory?(f) || ignores.any? { |i| File.fnmatch(i, f) }
  }

  s.files = (all_files_without_ignores + dotfiles).sort
end
