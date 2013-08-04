require 'simplecov'
SimpleCov.start
require 'rspec'
require 'capybara/rspec'
require 'capybara-webkit'
require_relative '../rat_pack_server'

def app
  RatPackServer
end

RSpec.configure do |config|
  config.include Capybara::DSL
  Capybara.app = app
  Capybara.javascript_driver = :webkit
end