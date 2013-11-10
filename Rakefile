require 'bundler/setup'
require 'rspec/core/rake_task'
require 'rake/extensiontask'

task :default => 'compile:librtlsdr'
task :default => :spec
task :spec    => 'spec:progress'

Rake::ExtensionTask.new(:librtlsdr) do |ext|
  ext.lib_dir = "lib/librtlsdr"
end

namespace :spec do
  desc 'Run all specs in spec directory (format=progress)'
  RSpec::Core::RakeTask.new(:progress) do |t|
    t.pattern = 'spec/**/*_spec.rb'
    t.rspec_opts = %w{ --color --format=progress }
  end

  desc 'Run all specs in spec directory (format=documentation)'
  RSpec::Core::RakeTask.new(:documentation) do |t|
    t.pattern = 'spec/**/*_spec.rb'
    t.rspec_opts = %w{ --color --format=documentation }
  end
end
