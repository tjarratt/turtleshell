Gem::Specification.new do |s|
  s.name = 'turtleshell'
  s.version = '0.1.0'
  s.date = '2013-11-05'
  s.summary = 'A ruby wrapper for librtlsdr'
  s.description = 'turtleshell is an interface for certain software defined radios'
  s.authors = ['Tim Jarratt']
  s.email = 'tjarratt@gmail.com'
  s.files = ['lib/turtleshell.rb']
  s.homepage = 'http://github.com/tjarratt/turtleshell'
  s.license = 'MIT'
  s.require_path = 'lib'

  s.add_development_dependency 'rake',  '~> 10.1'
  s.add_development_dependency 'rspec', '~> 2.14'

  ignores  = File.readlines('.gitignore').grep(/\S+/).map(&:chomp)
  dotfiles = %w[.gitignore]

  all_files_without_ignores = Dir['**/*'].reject { |f|
    File.directory?(f) || ignores.any? { |i| File.fnmatch(i, f) }
  }

  s.files = (all_files_without_ignores + dotfiles).sort
end
