# CIS*2750 Assignment 3 - README
## Installation and init Start of Node App
### 1. npm install
- run 'npm install' command in the root of SVGApp/ directory

### 2. create libsvgparse.so
- go to parser/ directory
- run 'make' commmand
- make will create libsvgparse.so in root of SVGApp/

### 3. Running Server
- go to root of SVGApp/ directory
- run 'npm run dev xxxxx' command (where xxxxx is port number)

### Console Output
- will display if files in uploads/ directory are valid svg files are not

### svg.xsd and supporting schemas location
- vplace in root directory if removed otherwise they should be there upon extraction

### page load
- Alert user of valid svg files in uploads/ and display valid files in File Log Panel
- Fill corresponding Dropdown Lists with file names

### SVG File Upload 
- Upon attempting to upload file not ending with .svg extension redirects users to status page
- please refresh the page in order to get proper file summary

### SVG View Panel
- Select file from dropdown list to display its summary
- pre-populates Show Attributes dropdown list with all attributes in file selected in SVG View Panel

### Show Attributes
- front end only
- Doesn't work properly... Lists the components for selected SVG but fails in certain scenarios..
- If node app.js crashes upon selecting value from dropdown lists please restart server

### Edit Attributes
- file must be selected first in SVG View Panel then if it has components those are displays in the dropdown list

### Create SVG File
- works great if entering filename, title, and description altogether

### Edit SVG File
- works for editing both title/desc at same time.

### Scale Shapes & Adding shapes (rectangles & circles)
- front end only works with...
- alerts via js work for invalid inputs, no file selected