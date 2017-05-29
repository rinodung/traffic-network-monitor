<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="chrome=1">
    <title>Server Monitor</title>
    <link rel="stylesheet" type="text/css" href="css/bootstrap.min.css"/>
    <link rel="stylesheet" type="text/css" href="css/bootstrap-theme.min.css"/>
    <link rel="stylesheet" type="text/css" href="css/style.css"/>
    <script type="text/javascript" src="js/jquery-3.1.0.min.js"></script>
    <script type="text/javascript" src="js/bootstrap.min.js"></script>
</head>
<body>
<div id="panel">
    <canvas id="world">
        <p class="noCanvas">You need a <a href="http://www.google.com/chrome">modern browser</a> to view this.</p>
    </canvas>
        <div id="content">
            <div class="col-lg-4 hole">
                <i></i>
                <i></i>
                <i></i>
                <i></i>
                <img id="server1" style="display:none" src="img/server.png" />
            </div>
            <div class="col-lg-4"></div>
        </div>
</div>

<script type="text/javascript" src="js/keylight.js"></script>
</body>
</html>
