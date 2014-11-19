jQuery(document).ready(function($) {
  $.ajax({
  url : "http://api.wunderground.com/api/74c6d3c6bd09e814/geolookup/conditions/q/CA/Los_Angeles.json",
  dataType : "jsonp",
  success : function(parsed_json) {
  var location = parsed_json['location']['city'];
  var temp_f = parsed_json['current_observation']['temp_f'];
  alert("Current temperature in " + location + " is: " + temp_f);
  }
  });
});