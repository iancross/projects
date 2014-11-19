var url = "http://api.wunderground.com/api/74c6d3c6bd09e814/conditions/q/"

jQuery(document).ready(function($) {
  $("select").change(function(){
		var str = "";
		$("select option:selected").each(function () {
            str += $(this).val();
        });
        get_city(str);
    });
    var inp = $("msg").text();
	console.log(inp);
	    
})


function get_city(str){

	if (str == "LAX"){
		url = url + "CA/Los_Angeles.json";
		loc = "Los Angeles"
		weather(url, loc);
				}	
	else if (str == "BOS"){
		url = url + "MA/Boston.json";
		loc = "Boston"
		weather(url, loc);
		}
	else if (str == "LHR"){
		url = url + "England/London.json";
		loc = "London"
		weather(url, loc);
	}
	else if (str == "SGN"){
		url = url + "Vietnam/Ho_Chi_Minh_City.json";
		loc = "Ho Chi Minh City"
		weather(url, loc);
	}
	else if (str == "ACC"){
		url = url + "Ghana/Accra.json";
		loc = "Accra"
		weather(url, loc);
	}
		
}

function weather(url, loc){
	$.ajax({
		url: url,
		dataType: "jsonp",
		success: function(parsed_json){
			var temp_f = parsed_json['current_observation']['temp_f'];
			str = "The current temperature in " + loc + " is " + temp_f + " degrees fahrenheit";
			$("div").text(str);
		}
	});
}

