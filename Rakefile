require 'rspec/core/rake_task'

RSpec::Core::RakeTask.new(:spec) do |t|
  t.pattern="./sinatra/spec{,/*/**}/*_spec.rb"
end

task :default => :spec
