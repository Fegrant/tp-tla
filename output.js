const cytoscape = require('cytoscape');
const cytosnap = require('cytosnap');
cytosnap.use(['cytoscape-dagre', 'cytoscape-cose-bilkent']);
const cytosnapOptions = {format: 'png', width: 1000, height: 1000, background: 'white'}
var snap = cytosnap(cytoscape, cytosnapOptions);
var onlythis = cytoscape({ elements:	[]});
