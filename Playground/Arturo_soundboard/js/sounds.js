function sound1(input)
{
	document.getElementById(input).play();
}

document.addEventListener('scroll', function (event) {
    if (document.getElementById("all").scrollHeight == 
        document.getElementById("all").scrollTop +        
        window.innerHeight) {
        alert("Bottom!");
    }
});