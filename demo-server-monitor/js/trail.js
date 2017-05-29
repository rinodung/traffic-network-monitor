var SCREEN_WIDTH = window.innerWidth;
var SCREEN_HEIGHT = window.innerHeight;

var RADIUS = 110;

var RADIUS_SCALE = 1;
var RADIUS_SCALE_MAX = 1.5;

// The number of particles that are used to generate the trail
var QUANTITY = 1;

var canvas;
var context;
var particles;
var posX = document.getElementById("server1").getBoundingClientRect().right;
var posY = document.getElementById("server1").getBoundingClientRect().top;
var posX2 = document.getElementById("server2").getBoundingClientRect().right;
var posY2 = document.getElementById("server2").getBoundingClientRect().bottom;
init();

function init() {

    canvas = document.getElementById('world');

    if (canvas && canvas.getContext) {
        context = canvas.getContext('2d');

        createParticles();

        windowResizeHandler();

        setInterval(loop, 1000 / 60);
    }
}

function createParticles() {
    particles = [];

    for (var i = 0; i < QUANTITY; i++) {
        var particle = {
            position: {x: posX, y: posY},
            shift: {x: posX2, y: posY2},
            size: 5,
            angle: 0,
            speed: 0.1 + Math.random() * 0.04,
            targetSize: 1,
            fillColor: '#' + (Math.random() * 0x404040 + 0xaaaaaa | 0).toString(16),
            orbit: RADIUS * .5 + (RADIUS * .5 * Math.random())
        };
        particles.push(particle);
    }
}

function windowResizeHandler() {

    canvas.width = SCREEN_WIDTH;
    canvas.height = SCREEN_HEIGHT;

    canvas.style.position = 'absolute';
    canvas.style.left = (window.innerWidth - SCREEN_WIDTH) * .5 + 'px';
    canvas.style.top = (window.innerHeight - SCREEN_HEIGHT) * .5 + 'px';
}

function loop() {


    RADIUS_SCALE = Math.min(RADIUS_SCALE, RADIUS_SCALE_MAX);

    // Fade out the lines slowly by drawing a rectangle over the entire canvas
    context.fillStyle = 'rgba(0,0,0,0.08)';
    context.fillRect(0, 0, context.canvas.width, context.canvas.height);

    for (i = 0, len = particles.length; i < len; i++) {
        var particle = particles[i];

        var lp = {x: particle.position.x, y: particle.position.y};
// Follow mouse with some lag
        // Apply position
        particle.position.x = particle.shift.x + Math.cos(i + particle.angle) * (particle.orbit * RADIUS_SCALE);
        particle.position.y = particle.shift.y + Math.sin(i + particle.angle) * (particle.orbit * RADIUS_SCALE);

        // If we're at the target size, set a new one. Think of it like a regular day at work.
        if (Math.round(particle.size) == Math.round(particle.targetSize)) {
            particle.targetSize = 1 + Math.random() * 15;
        }

        context.beginPath();
        context.fillStyle = particle.fillColor;
        context.strokeStyle = particle.fillColor;
        context.lineWidth = particle.size;
        context.moveTo(lp.x, lp.y);
        context.lineTo(particle.position.x, particle.position.y);
        context.stroke();
        context.arc(particle.position.x, particle.position.y, particle.size / 2, 0, Math.PI * 2, true);
        context.fill();
    }
}
	
	
	
	