require 'faye'
require File.expand_path('../rat_pack_server', __FILE__)

Faye::WebSocket.load_adapter('thin')

use Rack::ShowExceptions if ENV['RACK_ENV']=='development'
use Faye::RackAdapter, :mount      => '/faye',
                       :timeout    => 25

run RatPackServer
