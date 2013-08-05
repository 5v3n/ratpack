require 'simplecov'
SimpleCov.start
require 'rspec'
require 'capybara/rspec'
require 'capybara/poltergeist'
require_relative '../rat_pack_server'

set :environment, :test
ENV['RACK_ENV'] = 'test'

def app
  builder = Rack::Builder.parse_file(File.dirname(__FILE__) + '/../config.ru')
  builder.first
end

#runs, no timeout, but no websockets:
# Capybara.server do |app, port|
#   require 'rack/handler/webrick'
#   Rack::Handler::WEBrick.run(app, :Port => port)
# end


RSpec.configure do |config|
  config.include Capybara::DSL
  Capybara.app = app #Rack::Builder.parse_file(File.dirname(__FILE__) + '/../config.ru').first
  Capybara.javascript_driver = :poltergeist
  #causes timeout, but would use thin & websockets if it worked
  # Capybara.server do |app, port|
  #   require 'rack/handler/thin'
  #   #Thin::Logging.silent = true
  #   Rack::Handler::Thin.run(app, :Port => port)
  # end
end
