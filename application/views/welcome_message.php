<?php
defined('BASEPATH') OR exit('No direct script access allowed');
?><!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="utf-8">
	<title>Welcome to CodeIgniter</title>

	<style type="text/css">

	::selection { background-color: #E13300; color: white; }
	::-moz-selection { background-color: #E13300; color: white; }

	body {
		background-color: #fff;
		margin: 40px;
		font: 13px/20px normal Helvetica, Arial, sans-serif;
		color: #4F5155;
	}

	a {
		color: #003399;
		background-color: transparent;
		font-weight: normal;
	}

	h1 {
		color: #444;
		background-color: transparent;
		border-bottom: 1px solid #D0D0D0;
		font-size: 19px;
		font-weight: normal;
		margin: 0 0 14px 0;
		padding: 14px 15px 10px 15px;
	}

	code {
		font-family: Consolas, Monaco, Courier New, Courier, monospace;
		font-size: 12px;
		background-color: #f9f9f9;
		border: 1px solid #D0D0D0;
		color: #002166;
		display: block;
		margin: 14px 0 14px 0;
		padding: 12px 10px 12px 10px;
	}

	#body {
		margin: 0 15px 0 15px;
	}

	p.footer {
		text-align: right;
		font-size: 11px;
		border-top: 1px solid #D0D0D0;
		line-height: 32px;
		padding: 0 10px 0 10px;
		margin: 20px 0 0 0;
	}

	#container {
		margin: 10px;
		border: 1px solid #D0D0D0;
		box-shadow: 0 0 8px #D0D0D0;
	}
	</style>
</head>
<body>

<div id="container">
	<h1>Welcome to Traffic Monitor</h1>


	<canvas id="canvas1"></canvas>
	<canvas id="canvas2"></canvas>
	<p class="footer">Page rendered in <strong>{elapsed_time}</strong> seconds. <?php echo  (ENVIRONMENT === 'development') ?  'CodeIgniter Version <strong>' . CI_VERSION . '</strong>' : '' ?></p>
</div>

</body>
<script type="text/javascript">
var canvas = document.getElementById("canvas1");

var ctx = canvas.getContext("2d");
var p0 = {"x": 100, "y": 100};
var p1 = {"x": 350, "y": 50};
var p2 = {"x": 350, "y": 400};
var p3 = {"x": 100, "y": 350};
var t = 0;
var speed = 0.005;
canvas.height = 666;
canvas.width = 666;

setInterval(function () {

    var at = 1 - t;
	var green1x = p0.x * t + p1.x * at;
	var green1y = p0.y * t + p1.y * at;
	var green2x = p1.x * t + p2.x * at;
	var green2y = p1.y * t + p2.y * at;
	var green3x = p2.x * t + p3.x * at;
	var green3y = p2.y * t + p3.y * at;
	var blue1x = green1x * t + green2x * at;
	var blue1y = green1y * t + green2y * at;
	var blue2x = green2x * t + green3x * at;
	var blue2y = green2y * t + green3y * at;
	var finalx = blue1x * t + blue2x * at;
	var finaly = blue1y * t + blue2y * at;

	ctx.clearRect(0, 0, canvas.width, canvas.height);
	ctx.beginPath();
	ctx.arc(finalx, finaly, 5, 0, 2 * Math.PI, false);
	console.log(parseInt(finalx) + ":" + parseInt(finaly));
	ctx.fillStyle = 'red';
	ctx.fill();
	ctx.closePath();

	t += speed;
	if (t > 1 || t < 0) speed *= -1;
}, 40);


// Canvas 2
var canvas2 = document.getElementById("canvas2"),
    ctx2 = canvas2.getContext("2d"),
    painting = false,
    lastX = 0,
    lastY = 0;
var startPoint = {"x": 100, "y": 100};
var endPoint = {"x": 350, "y": 50};
canvas2.width = canvas2.height = 600;

canvas2.onclick = function (e) {
   
        ctx2.beginPath();
        ctx2.strokeStyle="#FF0000";
        ctx2.moveTo(startPoint.x, startPoint.y);
        ctx2.lineTo(endPoint.x, endPoint.y);
        ctx2.stroke();
}

function fadeOut() {
    ctx2.fillStyle = "rgba(255,255,255,0.3)";
    ctx2.fillRect(0, 0, canvas2.width, canvas2.height);
    setTimeout(fadeOut,100);
}

fadeOut();
</script>
</html>