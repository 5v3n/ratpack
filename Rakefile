if ['development','test'].include? ENV['RACK_ENV'] 
  require 'rspec/core/rake_task'
    RSpec::Core::RakeTask.new(:spec) do |t|
      t.pattern="./sinatra/spec{,/*/**}/*_spec.rb"
    end

    task :default => :spec
end

desc 'fake tasks needed by shelly'
namespace :db do
  task :migrate
  task :setup
end