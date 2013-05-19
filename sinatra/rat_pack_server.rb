require 'sinatra'
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
    settings.activated = data['activated']
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
end