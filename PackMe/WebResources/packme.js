var basic_pack_list = [];
num_textboxes = 1;
var name;
destLat = 0; // These two variables will be modified in initialize(), to hold the
destLng = 0; // latitude and longitude values of the user destination

//retrieves the login data and sends it to the database in order to find 
//associated data
function send_login(){
	name = $('#custom_user').val().toLowerCase();
	email = $('#login_password').val().toLowerCase();
	
	localStorage['username'] = name;
}

// this is a new user. Adds his name to the database and stores in local 
// storage to add the list later
function verify_name_email(){
	name = $("#name").val().toLowerCase();
	email = $('#email').val().toLowerCase();
    password = $('#password').val();
    localStorage['username'] = name;
	output = localStorage['username'];
  

  window.location.replace("http://packme.herokuapp.com/welcome?username=" + name + "&email=" + email);

  
	username = document.getElementById('name').value.toLowerCase();
	email = document.getElementById('email').value.toLowerCase();
  password = document.getElementById('password').value;
  var url = "/makeaccount.json?username=" + username + "&email=" + email + "&password=" + password;
  window.location.replace(url);
  var str = " " + {'username':username,'email':email,'password':password,'created_at': new Date(), 'expires_at': new Date()} + ""
  //post_to_url("/users.json", params, "post");
  //window.location.replace("/packme-enter_loc.html");
}

function post_to_url(path, params, method) {
console.log("posting to url");
    method = method || "post"; // Set method to post by default, if not specified.

    // The rest of this code assumes you are not using a library.
    // It can be made less wordy if you use one.
    var form = document.createElement("form");
    form.setAttribute("method", method);
    form.setAttribute("action", path);

    for(var key in params) {
        if(params.hasOwnProperty(key)) {
            var hiddenField = document.createElement("input");
            hiddenField.setAttribute("type", "hidden");
            hiddenField.setAttribute("name", key);
            hiddenField.setAttribute("value", params[key]);

            form.appendChild(hiddenField);
         }
    }

    document.body.appendChild(form);
    form.submit();

}

function send_loc(loc){
    geocoder = new google.maps.Geocoder();
	codeAddress(loc);	
}

//This function takes in an 'address' or any location (eg. Uganda, 389 Boston Ave, Boston,
// etc.) and turns it into a latitude and longitude that can be compared. 2 degrees of 
//latitude change would be roughly 100 mile radius.
function codeAddress(address){
   
	geocoder.geocode( { 'address': address}, function(results, status) {
	
	  if (status == google.maps.GeocoderStatus.OK) {
		  lat=results[0].geometry.location.lat().toFixed(6);
		  lng=results[0].geometry.location.lng().toFixed(6);
		  latlong = "(" + results[0].geometry.location.lat().toFixed(6) + " , " + results[0].geometry.location.lng().toFixed(6) + ")";
		  localStorage['lat'] = lat;
		  localStorage['lon'] = lng;
		  
		  //compare the result of this to locations found in the database		 
	  } 
	});
}
destLat = 0; // These two variables will be modified in initialize(), to hold the
destLng = 0; // latitude and longitude values of the user destination


function send_coords(){
/*          if(destLat == 0)   This test is helpful, but right now I'm not able to stop
          {                     it from going to the next page anyway after the alert. -Scott
            alert("Click to place marker on map before hitting continue.");
          }
 */         latlong = "(" + destLat + " , " + destLng + ")";
 		 
 		  localStorage['lat'] = destLat;
		  localStorage['lon'] = destLng;
          
}
var browserSupportsLocation =  new Boolean();
//var somerville = new google.maps.LatLng(42.3875, -71.1000);

function initializeHeat() 
{ 
    set_name();

    var mapOptions =
    {
        zoom: 2,
        mapTypeId: google.maps.MapTypeId.SATELLITE,
        panControl: false,
        scaleControl: true,
        streetViewControl: false,
    };
    var map = new google.maps.Map(document.getElementById("map_canvas"), mapOptions);
    marker = new google.maps.Marker({
       	//position: somerville, // Ian added to initialize the starting point - change to userLoc later.
      	map: map //
  });
 	// Browser supports GeoLocation
    if(navigator.geolocation)
    {
    	browserSupportsLocation = true;
    	navigator.geolocation.getCurrentPosition(function(position)
    	{
	      	centerWorld = new google.maps.LatLng(24.067865,-9.84375);
	     	map.setCenter(centerWorld);
	     	google.maps.event.addListener(map, 'click', function(event) {
                marker.setPosition(event.latLng);
                var theLoc = event.latLng;
                destLat = event.latLng.lat().toFixed(6);
                destLng = event.latLng.lng().toFixed(6);
//                document.getElementById('theLoc').value=theLoc.lat().toFixed(6);
//                document.getElementById('theLoc').value=theLoc.lng().toFixed(6);
            });
/*	        google.maps.event.addListener(map, "click", function(event) {
		        marker.setPosition(event.latLng); // Ian added this. I thought we could add a marker.

	            // populate yor box/field with lat, lng
	            console.log("Lat=" + lat + "; Lng=" + lng);
	        });*/
	    },
        	function()
        	{
        		handleNoGeolocation(browserSupportsLocation);
        	});
    }
     // Browser doesn't support Geolocation
    else
    {
    	browserSupportsLocation = false;
    	handleNoGeolocation(browserSupportsLocation);
  	}
  
  	function handleNoGeolocation(errorFlag) {
    	if (errorFlag == true) {
      		alert("Geolocation service failed.");
      		userLocation = somerville;
      	}
      	else {
        	alert("Your browser doesn't support geolocation. We've placed you in Somerville.");
        	userLocation = somerville;
        }
        map.setCenter(userLocation);
    }
}

function init_suggestions(){
        set_url();
         set_name();
		//get the array of items that we are going to be suggesting. Maybe have a hardcoded 
		//base list initially and add more.
	 basic_pack_list = ["Toothbrush and toothpaste", "Socks", "Underwear", "Pants", "Phone Charger", "Prophylactics", "Sunglasses", "Camera with Batteries"]


        var suggestions = document.getElementById('packing_suggestions');
        //suggestions.innerHTML = "";

    for (var option in basic_pack_list) {
        if (basic_pack_list.hasOwnProperty(option)) {
            var curr = basic_pack_list[option];
            var checkbox = document.createElement("input");
            checkbox.type = "checkbox";
            checkbox.name = curr;
            checkbox.value = curr;
            checkbox.id = curr;
            suggestions.appendChild(checkbox);
    
            var label = document.createElement('label')
            label.htmlFor = curr;
            label.appendChild(document.createTextNode(curr));

            suggestions.appendChild(label);
        }
    }
}

// stores the list generated by the user. This includes the checkboxes and the textboxes
function save_list(){
	myList = [];
	for(option in basic_pack_list){
		if(document.getElementById(basic_pack_list[option]).checked==true){
			console.log(basic_pack_list[option]);
			myList.push(basic_pack_list[option]);
	    }
    }
    
    for(iter = 0; iter<num_textboxes; iter++){
	    myList.push(document.getElementById("new_item" + iter).value);
    }
    localStorage["myList"] = JSON.stringify(myList);
    console.log(myList);
    console.log(localStorage["myList"]);
}

//adds a textbox so that the user can enter more items.
function add_textbox(){
	var container = document.getElementById('container');
	
	  var input = document.createElement('input'),
	  div = document.createElement('div');
	  input.type = "text";
	  input.id = ("new_item" + num_textboxes)
	  input.placeholder = 'NEW ITEM';
	  
	  div.insertBefore(input); 
	  container.insertBefore(div);
	  num_textboxes++;	
	  console.log(num_textboxes);
}
function update_packing_list(){
	set_name();
	var storedList = JSON.parse(localStorage["myList"]);

	var container = document.getElementById('print_list');
	for(i = 0; i < storedList.length; i++){
		  var li = document.createElement('li');
		  li.innerHTML = storedList[i];
		  container.appendChild(li, container.firstChild);
	}
}

//uses lat and lon to create a unique url for Wunderground API
function set_url(){
	var url = "http://api.wunderground.com/api/74c6d3c6bd09e814/geolookup/q/";
	console.log(localStorage['lat']);
	if(localStorage['lat'] != null && localStorage['lon'] != null){
		lat = localStorage['lat'];
		lon = localStorage['lon'];
	}
	
	else{
		return;
	}
	url = url + lat + "," + lon + ".json";
	weather_coords(url);
	lat = "";
	lon = "";
	
}

//gets URL needed for weather conditions request
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
				analyze_temp(300);
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

//gets current weather conditions 
function weather(url, loc){

	$.ajax({
		url: url,
		dataType: "jsonp",
		success: function(parsed_json){
			var res = parsed_json;
			var temp_f = res['current_observation']['temp_f'];
			console.log(res);
			var str = "It's " + temp_f + " degrees in " + loc;
			
			div = document.getElementById('current_temp');
			var input = document.createTextNode(str);
			div.appendChild(input);
			analyze_temp(temp_f);
		}
	});

}

function analyze_temp(temp)
{

	var message;
	if (temp == 300){
		message = "Sorry, there are no weather stations in that area";
	}
	if (temp <= -10){
		message = "Holy Cow, that's cold! Sure you wanna go?";
	}
	
	if (temp > -10 && temp <= 10){
		message = "I bet you're wishing Cancun wasn't sold out now!";
	}
	
	if (temp > 10 && temp <= 20){
		message = "Brrrrr I'm glad you're the one going and I get to stay warm here.";
	}
	if (temp > 20 && temp <= 34){
		message = "Hope you like snow, cause it's cold enough!";
	}
	if (temp > 34 && temp <= 50){
		message = "Meh, too warm to snow, too cold to be pleasant. Sorry!";
	}
	if (temp > 50 && temp <= 65){
		message = "Ahh, you're so close to spring weather: SWEATER WEATHER";
	}
	if(temp > 65 && temp <= 75){
		message = "You're never coming home, the weather is too nice";
	}
	if (temp > 75 && temp <= 90){
		message = "Bring your blanket and sunscreen, you're gonna want to lie out all day";
	}
	if (temp > 90 && temp <= 105){
		message = "Wowza! Hope your hotel has A/C";
	}
	if (temp > 105 && temp != 300){
		message = "Oh man, I though people didn't go to hell until the died!";
	}
 
	div = document.getElementById('suggestion');
	var input = document.createTextNode(message);
	div.appendChild(input);

}

function set_name(){
	
	name = localStorage['username'];
	if(name == null){
		return;
	}
    if(document.getElementById('custom_user')){
   		document.getElementById('custom_user').value = name;
   		return;
   	}
   	else {
   		div = document.getElementById('username');
   		var input = document.createTextNode(name);
   		div.appendChild(input);
   	}

}

