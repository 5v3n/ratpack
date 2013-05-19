require_relative 'spec_helper'
require_relative '../rat_pack_server'
require 'rack/test'

describe RatPackServer do
  include Rack::Test::Methods
  def app
    RatPackServer
  end
  describe "/" do
    it "redirects to '/status.html'" do
      get '/'
      last_response.should be_redirect
      follow_redirect!
      last_request.path.should == '/status.html'
    end
  end
  describe "/status" do
    let(:request_params_activated){    {'activated' => 1}.to_json }
    let(:request_params_deactivated){  {'activated' => 0}.to_json }

    describe "PUT" do
      it "updates the state to 'on' for json request" do
        put '/status.json', request_params_activated
        last_response.status.should == 200
        last_response.body.should == request_params_activated.to_s
      end
      it "updates the state to 'off' for json request" do
        put '/status.json', request_params_deactivated
        last_response.status.should == 200
        last_response.body.should == request_params_deactivated.to_s
      end
    end
    describe "GET" do
      it "displays the state for json requests" do
        get '/status.json'
        last_response.status.should == 200
        last_response.body.should == request_params_deactivated.to_s
      end
      it "displays the state for html requests" do
        get '/status.html'
        last_response.status.should == 200
        last_response.body.should include 'off'
      end
    end
  end
end
