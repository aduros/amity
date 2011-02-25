$amity.log("Hello world!");

var elapsed = 0;
$amity.onEnterFrame = function (dt) {
    elapsed += dt;

    $amity.log("Total elapsed: " + elapsed);

    //$amity.canvas.save();
    //$amity.canvas.rotate(0.1 * elapsed);
    //$amity.canvas.drawImage(image, imageX, imageY);
    //$amity.canvas.restore();
};
