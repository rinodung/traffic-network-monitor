<!DOCTYPE html>
<!-- saved from url=(0033)http://lab.hakim.se/keylight/03/# -->
<html lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
		

		<meta name="description" content="Traffic network monitor.">
		<meta name="keywords" content="CTF, Visualization, traffic, network, monitor, developer, flash, html5, canvas, web, experiments">
		<meta name="author" content="Hakim El Hattab">

		<meta http-equiv="X-UA-Compatible" content="chrome=1">
	    
        <title>CTF Visualization</title>
		

		<link href="./assets/styles.css" rel="stylesheet" media="screen">
    </head>
    <body>
		
	
<canvas id="world" width="100%" height="100%" style="position: absolute; top: 0px; float:left;">
	<p class="noCanvas">You need a <a href="http://www.google.com/chrome">modern browser</a> to view this.</p>
</canvas>
//css hover
<div id="ip" style="display:none; color:red; position: absolute; padding:20px; top: 100px; left: 0%; ">192.168.1.1</div>
<div id="panel" style="position: absolute; top: 0px;left: 0%; ">
			<h2 id="network">Network Monitor Version 1.1</h2>
			<p><b><a id="decreaseSpeed" href="/#">Decrease</a> / <a id="increaseSpeed" href="#">Increase</a> 
			</b> speed (<span id="speedDisplay">3/6</span>) <b><a id="traffic" href="#">Traffic</a></b></p>
			
			<div id="trafficData" style="display:none">
				<textarea type="textarea" cols='49' rows='10' id="log" style="color: #08de07; background: transparent;"></textarea>
			</div>
</div>
<script type="text/javascript" src="./assets/widgets.js"></script>
<script src="./assets/bower_components/jquery/dist/jquery.min.js"></script>
<script src="./assets/keylight.js"></script>
<script type="javascript">

</script>
</body>
    </html>
