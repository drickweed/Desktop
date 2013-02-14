var dataPath, settingsPath, savePath;

// Read data path from file in user's temp folder (<user home>/Local Settings/Temp)
var helperFile = new File(Folder.temp + "/imageimp_create_album.cfg");
helperFile.open('r');
settingsPath = helperFile.readln();
dataPath = helperFile.readln();
savePath = helperFile.readln();
helperFile.close();

// Default options
var options = Object();
options.grid = Object();

// Load settings exported from imageimp application
var settingsFile = new File(settingsPath);
settingsFile.open('r');
options.grid.width = settingsFile.readln();
options.grid.height = settingsFile.readln();
options.pictureTemplate = settingsFile.readln();
options.albumTemplate = settingsFile.readln();

// Open document template
var album = open(File(options.albumTemplate));

// Create layer groups.
// NOTE order of layer group creation is important because it decides what is visible over what.
var backLayerSet = album.layerSets.add();
backLayerSet.name = "Backgrounds";
var pictureLayerSet = album.layerSets.add();
pictureLayerSet.name = "Pictures";
var frontLayerSet = album.layerSets.add();
frontLayerSet.name = "Foregrounds";
var textLayerSet = album.layerSets.add();
textLayerSet.name = "Names";


// Load person and image list from text database
var dataFile = new File(dataPath);
dataFile.open('r');
var i = 0;
var people = Array();
var image_paths = Array();
while (!dataFile.eof) {
		people[i] = dataFile.readln();
		image_paths[i] = dataFile.readln();
		i++;
}
dataFile.close();

// Create template object
var template_image = open(File(options.pictureTemplate));
var template = Object();

try {
	// Get back layer
	template.back = template_image.artLayers.getByName("Back");
} catch (e) {
	//  layer is optional.
}

try {
	// Get front layer
	template.front = template_image.artLayers.getByName("Front");
} catch (e) {
	// Front layer is optional.
}

// Get text layer
template.text = template_image.artLayers.getByName("Text").textItem;

// Get picture layer
template.picture = template_image.artLayers.getByName("Picture");

// Get thumbnail dimentions for picture. correvts width and height to preserve aspect ratio if one of the dimentions is 0
// return [width, height]
function getThumbnailDimentions(image, width, height) {

	if (width == 0 && height == 0)
		return [0,0];
	var width, height;
	var aspect = image.width / image.height;
	if (width == 0) {
		width = height * aspect;
	} else if (height == 0) {
		height = width / aspect;
	}
	return [width, height];
}

function getMaxLayerWidth(layer) {

}

function getMaxLayerHeight(layer) {

}

// Get layet width and height.
// return [width, height]
function getLayerSize(layer) {
	var width = layer.bounds[2] - layer.bounds[0];
	var height = layer.bounds[3] - layer.bounds[1];
	return [width, height]
}

// Get document width and height
// return [width, height]
function getDocumentSize(document) {
	return [document.width, document.height];
}

// Copy text item properties from source to destination.
function copyTextItem(destination, source) {
	destination.alternateLigatures = source.alternateLigatures;
	destination.antiAliasMethod = source.antiAliasMethod;
	destination.font = source.font;
	destination.size = source.size;
	destination.color = source.color;
	destination.position = source.position;
	destination.underline = source.underline;
	destination.direction = source.direction;
	destination.justification = source.justification;
}

// Check if layer is empty
function layerIsEmpty(layer) {
	return (layer.kind == LayerKind.NORMAL && layer.bounds[2] == 0 && layer.bounds[3] == 0);
}

// Create person layer (image + name and decorations)
// return the picture layer
function createPersonLayer(album, name, image, options) {

	// Save active document
	var oldActiveDocument = activeDocument;

	// Add background layer
	try {
		if (!layerIsEmpty(template.back)) {
			activeDocument = album;
			var backLayer = backLayerSet.artLayers.add();
			backLayer.name = name + " (back)";
			activeDocument = template_image;
			template.back.copy();
			activeDocument = album;
			album.paste();
			backLayer.translate(-backLayer.bounds[0] + template.back.bounds[0], -backLayer.bounds[1] + template.back.bounds[1]);
		}
	} catch (e) {
		// Back layer is optional
	}

	// Add foreground layer
	try {
		if (!layerIsEmpty(template.front)) {
			activeDocument = album;
			var frontLayer = frontLayerSet.artLayers.add();
			frontLayer.name = name + " (front)";
			activeDocument = template_image;
			template.front.copy();
			activeDocument = album;
			album.paste();
			frontLayer.translate(-frontLayer.bounds[0] + template.front.bounds[0], -frontLayer.bounds[1] + template.front.bounds[1]);
		}
	} catch (e) {
		// Front layer is optional
	}

	// Load student image
	var person_image = open(File(image));

	var thumbnail_size = getLayerSize(template.picture);
	var thumbnail_dimentions = getThumbnailDimentions(person_image, thumbnail_size[0], 0);
	var width = thumbnail_dimentions[0];
	var height = thumbnail_dimentions[1];

	person_image.resizeImage(width, height, 72);
	person_image.selection.selectAll();
	person_image.selection.copy();

	// Add person picture layer
	activeDocument = album;
	var picLayer = pictureLayerSet.artLayers.add();
	picLayer.name = name + " (image)";
	album.paste();
	picLayer.translate(-picLayer.bounds[0] + template.picture.bounds[0], -picLayer.bounds[1] + template.picture.bounds[1]);
	person_image.close(SaveOptions.DONOTSAVECHANGES);

	// Add person name layer
	var nameLayer = textLayerSet.artLayers.add();
	nameLayer.kind = LayerKind.TEXT;
	nameLayer.name = name + " (name)";
	var txtRef = nameLayer.textItem;
	copyTextItem(txtRef, template.text);
	txtRef.contents = name;

	// Link all person related layers
	nameLayer.link(picLayer);
	try {
		nameLayer.link(backLayer);
	} catch (e) {}
	try {
		nameLayer.link(frontLayer);
	} catch (e) {}

	// Set old active document
	activeDocument = oldActiveDocument;

	// The background layer is the one which determines 
	return backLayer;
}

var i, j,layer_index;

// Create thumbnail layers
var layers =Array();
for(i=0; i<people.length; i++) {
	layers[i] = createPersonLayer(album, people[i], image_paths[i], options);
}

var picture_size = getDocumentSize(template_image);

template_image.close(SaveOptions.DONOTSAVECHANGES);

// Arrange in grid
layer_index = 0;
activeDocument = album;
for (i=0; i<options.grid.width; i++) {
	for (j=0; j<options.grid.height; j++) {
		if (layer_index < layers.length) {
			//var size = getLayerSize(layers[layer_index]);
			layers[layer_index].translate(j*picture_size[0], i*picture_size[1]);
		}
		layer_index++;
	}
}

// Save album file
var saveFile = new File(savePath);
album.saveAs(saveFile);
