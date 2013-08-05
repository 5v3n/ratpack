require 'rspec'
require 'capybara/rspec'
require 'capybara/webkit'
require 'simplecov'
SimpleCov.start
require_relative '../rat_pack_server'

set :environment, :test
ENV['RACK_ENV'] = 'test'
APP_PORT=4180

def app
  builder = Rack::Builder.parse_file(File.dirname(__FILE__) + '/../config.ru')
  builder.first
end

Thread.new do
  thin = Rack::Handler.get('thin')
  thin.run(app, :Port => APP_PORT)
end

Capybara.app_host = "http://0.0.0.0:#{APP_PORT}"

RSpec.configure do |config|
  config.include Capybara::DSL
  Capybara.javascript_driver = :webkit
end
