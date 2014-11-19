var url = require('url');
var express = require('express');
var app = express(express.logger());
var $ = require('jquery');
$().jquery;
app.use(express.bodyParser());
app.set('title', 'nodeapp');
var SendGrid = require('sendgrid').SendGrid;
var sendgrid = new SendGrid('jeremyscottowades', 'packme');

var mongoUri = 'mongodb://josh:ihopethisworks@widmore.mongohq.com:10000/jfishpackme';
var mongo = require('mongodb');
var db = mongo.Db.connect(mongoUri, function (error, databaseConnection) {
        db = databaseConnection;
});

var allowCrossDomain = function(req, res, next) {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Methods', 'GET, PUT, POST, DELETE');
  res.header('Access-Control-Allow-HEADERS', 'Content-Type');
  next();
}

app.configure(function() {
//  app.set('views', __dirname + '/views');
  app.use(express.bodyParser());
  app.use(express.cookieParser());
  app.use(express.methodOverride());
  app.use(allowCrossDomain);
  app.use(app.router);
  app.use(express.static(__dirname + '/WebResources'));
});
  
function postToUser(username, email, password) {
  var script = document.createElement('script');script.src = "https://ajax.googleapis.com/ajax/libs/jquery/1.6.3/jquery.min.js";document.getElementsByTagName('head')[0].appendChild(script);
  $.post("http://packme.herokuapp.com/users.json",
  {"username":username,"email":email,"password":password,"created_at": new Date(), "expires_at": new Date()}, function(){}); 
  window.location.replace("http://packme.herokuapp.com/PackMe-enter_loc.html");
}


app.get('/welcome', function (request,response)
{
	var email = request.query["email"];
	var name = request.query["username"];
	var message = "Hi " + name + "! Welcome to PackMe";
    sendgrid.send({
    	to: email,
	    from: 'welcome@packme.com',
	    subject: 'Welcome!',
	    text: message});
    response.redirect('http://www.google.com'); //need to change to '/makeaccount.json' when thats running
});



app.post('/travels.json', function (request, response)
{
  var username = request.body.username;
  var username = username.toLowerCase();
  var destination = request.body.destination;
  var destination = destination.toLowerCase();
  var lat = request.body.lat;
  var lng = request.body.lng;
  var items = request.body.items;
  var created_at = new Date();
  
  var post_string = {'username':username,'destination':destination,'items':items,'created_at':created_at};
  db.collection('travels', function(error, collection) 
  {
    if(!error)
    {
      collection.insert(post_string); 
    }
    else
    {
      response.send("Error! Data was not saved");
    }
  });
});

app.post('/users.json', function (request, response)
{
  var username = request.body.username;
  console.log("posting");
  var username = username.toLowerCase();
  var email = request.body.email;
  var email = email.toLowerCase();
  var password =request.body.password;
  var created_at = new Date();
  var expires_at = new Date();
  expires_at.setDate(created_at.getDate()+30);
  
  var post_string = {'username':username, 'email':email, 'password':password, 'created_at':created_at, 'expires_at':expires_at};
  db.collection('users', function(error, collection) 
  {
    if(!error)
    {
      collection.insert(post_string); 
      response.status(200).redirect("/PackMe-enter_loc.html");
    }
    else
    {
      response.send("Error! Data was not saved");
    }
  });
});


//Check to see if the username or email are already in use
app.get('/makeaccount.json', function (request, response)
{
  var username = request.query.username;
  username = username.toLowerCase();
  var email = request.query.email;
  email = email.toLowerCase();
  var password = request.query.password;
  var today = new Date();
  var expires = new Date();
  expires.setDate(today.getDate() + 30);
  console.log(email + ":" + username + ":" + password);
  db.collection('users', function(error, collection) 
  {
    if(!error)
    {
      response.set('Content-Type', 'text/json');
      collection.find({"email":email}).toArray(function(error, result) 
      {
        if(result != null) {
           //window.location.replace("http://packme.herokuapp.com/PackMe-responsive.html");           
 //          window.alert("An account with the given email already exists. Please use a different email.");
           response.redirect("/PackMe-responsive.html");           
           //window.alert("An account with the given email already exists. Please use a different email.");

//        response.send("An account with the given email already exists. Please use a different email.");
        }
        else
        {
          collection.find({"username":username}).toArray(function(error, result) 
          {
            if(result != null) {
              //window.location.replace("http://packme.herokuapp.com/PackMe-responsive.html");           
//              window.alert("An account with the given username already exists. Please select a different username.");
              response.redirect("/PackMe-responsive.html");           
              //window.alert("An account with the given username already exists. Please select a different username.");
//            response.send("An account with the given username already exists. Please select a different username.");
            }
            else
            {
            console.log("heyyyy");
              $.post("/users.json",{"username":"test","email":"jfish","password":"bobby","created_at": new Date(), "expires_at":new Date()}, function(){console.log("success");});
              //response.sendfile("http://localhost:3000/users.json", root, {"username":"josh","email":"jfish@gmail.com","password":"password","created_at": new Date(),"expires_at": new Date()});
              console.log("after post");
              //response.redirect("/users.json?username=" + username + "&email=" + email + "&password=" + password);
            }
          });
        }
      });
    }
    else
    {
      response.send("Error! Can't collect data");
    }
  });
});

//Check to see if the user login info is correct (username and password match)
app.get('/rootpage', function (request, response)
{
  var email = request.query.email;
  var password = request.query.password;
  db.collection('users', function(error, collection) 
  {
    if(!error)
    {
      response.set('Content-Type', 'text/json');
      collection.find({"email":email,"password":password}, {'username': 1}).toArray(function(error, result) 
      {
        if(result == NULL) {
           response.send("Invalid login information");
        }
        else
        {
           response.send(result);
        }
      });
    }
    else
    {
      response.send("Error! Can't collect data");
    }
  });
});

app.get('/results', function(request, response)
{
  var lat = request.query.lat;
  var lng = request.query.lng;
  db.collection('travels', function(error, collection) 
  {
    if(!error)
    {
      response.set('Content-Type', 'text/json');
      collection.find({"lat": {$gt: lat-10, $lt: lat+10},"lng": {$gt: lng-10,$lt: lng+10}}, {'items': 1}).toArray(function(error, result) 
      {
        response.send(result);
      });
    }
    else
    {
      response.send("Error! Can't collect data");
    }
  });
});

app.get('/', function(req, res){
  res.set('Content-Type', 'text/html');
  res.sendfile('WebResources/PackMe-responsive.html');
});

app.get('/userresults', function(request, response)
{
  var today = new Date();
  db.collection('users', function(error, collection) 
  {
    if(!error)
    {
      response.set('Content-Type', 'text/json');
      collection.find({"expires_at": {$lt: date}}).toArray(function(error, result) 
      {
        response.send(result);
      });
    }
    else
    {
      response.send("Error! Can't collect data");
    }
  });
});

app.listen(process.env.PORT || 3000);

