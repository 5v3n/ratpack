var RatPack = RatPack || {};
$(document).ready(function() {
  RatPack.fayeClient = new Faye.Client(window.location.protocol + '//' + window.location.host + '/faye');
  var subscription = RatPack.fayeClient.subscribe('/status/realtime', function(message) {
    message.activated === 0 ? RatPack.turnOffButton() : RatPack.turnOnButton()
  });

 RatPack.turnOffButton = function(){
    var button = $('.btn.on');
    button.removeClass('on');
    button.addClass('off');
    button.html('off');
  };
  RatPack.turnOnButton = function(){
    var button = $('.btn.off');
    button.removeClass('off');
    button.addClass('on');
    button.html('on');
  };
});
