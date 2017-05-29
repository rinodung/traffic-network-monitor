// get the canvas element
var canvas = document.getElementById('monitor');
// get a context to draw in
var ctx = canvas.getContext('2d');
function tick() {
    // this is the only real tweak on the previous
    // example: instead of totally resetting the previous
    // frame, we only fade it out by drawing translucent
    // white over it
    ctx.fillStyle = 'rgba(255, 255, 255, 0.2)';
    // clear what was drawn in the previous frame
    ctx.fillRect(0, 0, 600, 100);
    // now draw an opaque circle
    ctx.fillStyle = '#ddd';
    // begin a new path: arc is a line instruction like lineTo
    ctx.beginPath();
    // define the circle: position according to time, 50px radius
    ctx.arc(100, 300, 20, 0, 2 * Math.PI);
    // draw the circle
    ctx.fill();
    // request a chance to draw the circle again as soon as possible
    requestAnimationFrame(tick);
}
tick();