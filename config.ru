require './sinatra/rat_pack_server.rb'

ENV['LOG_LEVEL'] ||= ENV['RACK_ENV']=='development' ? Logger::DEBUG.to_s : Logger::INFO.to_s

use Rack::ShowExceptions if ENV['RACK_ENV']=='development'

run RatPackServer.new
