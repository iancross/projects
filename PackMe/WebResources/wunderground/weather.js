var url = "http://api.wunderground.com/api/74c6d3c6bd09e814/geolookup/q/"


jQuery(document).ready(function($) {
  $("select").change(function(){
		var str = "";
		$("select option:selected").each(function () {
            str += $(this).val();
        });
        get_city(str);
    });
    var inp = $("msg").text();
	
});
	    

function get_lat_long(){
	lat = document.getElementById("lat").value;
	lon = document.getElementById("lon").value;
	
	url = url + lat + "," + lon + ".json";
	
	weather_coords(url);
	lat = "";
	lon = "";
	
}

function weather(url, loc){

	$.ajax({
		url: url,
		dataType: "jsonp",
		success: function(parsed_json){
			var res = parsed_json;
			var temp_f = res['current_observation']['temp_f'];
			str = "The current temperature in " + loc + " is " + temp_f + " degrees fahrenheit";
			console.log(tets);
			$("div").text(str);
		}
	});
}

function weather_coords(url){
	
	$.ajax({
		url: url,
		datType: "jsonp",
		success: function(parsed_json){
		    var result = parsed_json;
		    var city;
			var requrl = "http://api.wunderground.com/api/74c6d3c6bd09e814/conditions/q/";
			if(result['response']['error'] != null){
			if(result['response']['error']['type'] == 'querynotfound'){
				alert("Sorry, there are no weather stations in that area");
				return;
				}
			}
			
			requrl = requrl + result['location']['requesturl'];
			city = result['location']['city'];
			
			
			len = requrl.length;
			
			requrl = requrl.slice(0, len-5);
			requrl = requrl + ".json";
            
			weather(requrl, city);
			requrl = "";
			url = "";
		}
	});
}

