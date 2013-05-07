$(document).ready(function() {
  (function(){
    var ratpack = ratpack || {};
    ratpack.poll = function(){
      setTimeout(function(){
        $.ajax({
        url: "status.json",
        dataType: "json",
        success: function(data){ 
          data.activated === 0 ? ratpack.turnOffButton() : ratpack.turnOnButton()
        },
        complete: ratpack.poll(),
      });
      }, 2000)
    };
    ratpack.turnOffButton = function(){
      var button = $('.btn.on');
      button.removeClass('on');
      button.addClass('off');
      button.html('off');
    };
    ratpack.turnOnButton = function(){
      var button = $('.btn.off');
      button.removeClass('off');
      button.addClass('on'); 
      button.html('on');
    };
    ratpack.poll();
  })();
});