require 'simplecov'
SimpleCov.start
require 'rspec'
require 'capybara/webkit'
require 'capybara/rspec'
require_relative '../rat_pack_server'

set :environment, :test
ENV['RACK_ENV'] = 'test'

def app
  builder = Rack::Builder.parse_file(File.dirname(__FILE__) + '/../config.ru')
  builder.first
  # builder = Rack::Builder.new do
  #   Faye::WebSocket.load_adapter('thin')
  #   use Faye::RackAdapter, :mount      => '/faye',
  #                      :timeout    => 25
  #   run RatPackServer
  # end
  # builder.to_app
end

#runs, no timeout, but no websockets:
# Capybara.server do |app, port|
#   require 'rack/handler/webrick'
#   Rack::Handler::WEBrick.run(app, :Port => port)
# end


RSpec.configure do |config|
  config.include Capybara::DSL
  Capybara.app = app #Rack::Builder.parse_file(File.dirname(__FILE__) + '/../config.ru').first
  Capybara.javascript_driver = :webkit
  #causes timeout, but would use thin & websockets if it worked
  # Capybara.server do |app, port|
  #   require 'rack/handler/thin'
  #   Rack::Handler::Thin.run(app, :Port => port)
  # end
end
