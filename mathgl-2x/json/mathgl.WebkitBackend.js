/**
 * Webkit backend. Backend base on the object injected by QWebView.
 */

/** constructor */
mathgl.WebkitBackend = function() {}

/** inherit from mathgl.Backend interface */
mathgl.WebkitBackend.prototype = new mathgl.Backend();


/** return geometry */
mathgl.WebkitBackend.prototype.geometry = function(mgl) {
    var geometryData = globalBackend.geometry(mgl);
    console.log("geometryData size:",geometryData.length);

	/*
	var zlib = require('zlib');
	zlib.unzip(geometryData, function(err, buffer) {
		if (!err)
		{	geometryData = buffer;
			console.log("geometryData size:",geometryData.length);	}
	 });
	 */
	
    var obj = JSON.parse(geometryData);
    obj.pp = new Array();
    for(var i=0;i<obj.npnts;i++)	// copy original data for transformation
        obj.pp[i] = [obj.pnts[i][0],obj.pnts[i][1],obj.pnts[i][2]];
    return obj;
}
