require './sinatra/rat_pack_server.rb'

use Rack::ShowExceptions if ENV['RACK_ENV']=='development'

run RatPackServer.new
