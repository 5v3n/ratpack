require 'sinatra'
require 'faye'
require 'json'
require 'haml'
require 'rack/logger'

class RatPackServer < Sinatra::Application
  configure()             { set :activated, 0 }
  configure(:development) { set :logging, Logger::DEBUG }
  configure(:production)  { set :logging, Logger::INFO }
  configure(:test)        { disable :logging }
 
  get '/' do
    redirect '/status.html'
  end
  get '/status.html' do
    haml :index, :locals => {:activated => settings.activated, :indicator_class => indicator(settings.activated)}
  end
  get '/status.json' do
    render_status_response
  end
  put '/status.json' do
    request.body.rewind  # in case someone already read it
    data = JSON.parse request.body.read
    if settings.activated != data['activated']
      logger.info "change detected, activated = #{settings.activated}. sending update to faye subscribers..."
      settings.activated = data['activated']
      faye_client.publish('/status/realtime', {activated: settings.activated})
    end
    logger.debug "data    : #{data}"
    logger.debug "response: #{render_status_response}"
    render_status_response
  end
private
  def indicator(state)
    state == 0 ? :off : :on
  end
  def render_status_response
    {activated: settings.activated}.to_json
  end
  def faye_client
    env['faye.client'] || Faye::Client.new("http://#{request.host_with_port}/faye")
  end
end