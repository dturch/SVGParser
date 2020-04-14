/**
 * file    		index.js
 * author  		Dario Turchi
 * studentID 	0929012
 * lastEdit     April 13, 2020s
 */
// Put all onload AJAX calls here, and event listeners
$(document).ready(function () {
    document.getElementById("db-actions").style.display = "none"; // hide the db actions until logged in
    refreshFileLogandDropdown(); // populate dropdownlists and filelog
    clearAllForms(); //triger reset to clear all fields in forms

    // svg Summary dropdown action 
    $('#svg-dropdown').change(function () {
        let filename = $("#svg-dropdown option:selected").text();

        console.log(filename);

        $('#svg-title-desc tr').remove();
        $('#rectangle-summary tr').remove();
        $('#circle-summary tr').remove();
        $('#path-summary tr').remove();
        $('#group-summary tr').remove();
        $('#svg-display-img img').remove();
        $('#svg-display-img h5').remove();
        $('#component-dropdown option').remove();
        $('#component-dropdown').append("<option selected value=\"-- No Component Selected --\"" + ">" + "-- No Component Selected --" + "</option>");
        $('#component-dropdown-edit').append("<option selected value=\"-- No Component Selected --\"" + ">" + "-- No Component Selected --" + "</option>");

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/components/' + filename,   //The server endpoint we are connecting to

            success: function (data) {
                console.log(data);
                for (let svgStruct in data) {
                    for (var r = 0; r < data[svgStruct].rectangle.length; r++) {
                        $("#rectangle-summary").append("<tr><td>Rectangle " + (r + 1) + "</td><td>Upper left corner: x = " + data[svgStruct].rectangle[r].x + data[svgStruct].rectangle[r].units + ", y = " + data[svgStruct].rectangle[r].y + data[svgStruct].rectangle[r].units + ", Width: " + data[svgStruct].rectangle[r].w + data[svgStruct].rectangle[r].units + ", Height: " + data[svgStruct].rectangle[r].h + data[svgStruct].rectangle[r].units + "</td><td>" + data[svgStruct].rectangle[r].numAttr + "</td></tr>");
                        $('#component-dropdown').append("<option value=\""+r+"\"" + ">" + "Rectangle " + (r + 1) + "</option>");
                        $('#component-dropdown-edit').append("<option value=\""+r+"\"" + ">" + "Rectangle " + (r + 1) + "</option>");
                    }
                    for (var c = 0; c < data[svgStruct].circle.length; c++) {
                        $("#circle-summary").append("<tr><td>Circle " + (c + 1) + "</td><td>Centre: x = " + data[svgStruct].circle[c].cx + data[svgStruct].circle[c].units + ", y = " + data[svgStruct].circle[c].cy + data[svgStruct].circle[c].units + ", radius: " + data[svgStruct].circle[c].r + data[svgStruct].circle[c].units + "</td><td>" + data[svgStruct].circle[c].numAttr + "</td></tr>");
                        $('#component-dropdown').append("<option value=\""+c+"\"" + ">" + "Circle " + (c + 1) + "</option>");
                        $('#component-dropdown-edit').append("<option value=\""+c+"\"" + ">" + "Circle " + (c + 1) + "</option>");
                    }
                    for (var p = 0; p < data[svgStruct].path.length; p++) {
                        $("#path-summary").append("<tr><td>Path " + (p + 1) + "</td><td>path data = " + data[svgStruct].path[p].d + "</td><td>" + data[svgStruct].path[p].numAttr + "</td></tr>");
                        $('#component-dropdown').append("<option value=\""+p+"\"" + ">" + "Path " + (p + 1) + "</option>");
                        $('#component-dropdown-edit').append("<option value=\""+p+"\"" + ">" + "Path " + (p + 1) + "</option>");
                    }
                    for (var g = 0; g < data[svgStruct].group.length; g++) {
                        $("#group-summary").append("<tr><td>Group " + (g + 1) + "</td><td>" + data[svgStruct].group[g].children + " child elements</td><td>" + data[svgStruct].group[g].numAttr + "</td></tr>");
                        $('#component-dropdown').append("<option value=\""+g+"\"" + ">" + "Group " + (g + 1) + "</option>");
                        $('#component-dropdown-edit').append("<option value=\""+g+"\"" + ">" + "Group " + (g + 1) + "</option>");
                    }
                    $('#svg-title-desc').append("<tr><td>" + data[svgStruct].title + "</td><td>" + data[svgStruct].description + "</td></tr>");
                }
                $('#svg-display-img').append("<h5 class=\"blue\">" + filename + "</h5><img class=\"svg-display-img\" src=\"" + filename + "\">");
                alert("Successfully displayed summary of " + filename + " to SVG View Panel");
            },
            fail: function (error) {
                // Non-200 return, do something with error
                alert("Could not display summary of " + filename + " to SVG View Panel");
                console.log(error);
            }
        });
    });

    // component properties dropdown action 
    $('#component-dropdown').change(function () {
        let component = $("#component-dropdown option:selected").text();
        let file = $("#svg-dropdown").children("option:selected").val();        
        console.log(component);

        $('#component-properties tr').remove();
        
        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/components-properties/' + file,   //The server endpoint we are connecting to

            success: function (data) {
                console.log(data);
                for (var i = 0; i < data.length; i++) {
                    console.log(data[i]);
                    $("#component-properties").append("<tr><td>" + data[i]["name"] + "</td><td>" + data[i]["value"] + "</td></tr>");
                }
                alert("Successfully displayed additional properites of " + component);
            },
            fail: function (error) {
                //Non-200 return, do something with error
                alert("Could not display additional properites of " + component);
                console.log(error);
            }
        });
    });

    // edit-svg dropdown action 
    $('#svg-dropdown-edit-svg').change(function (){
        let imageToEdit = $("#svg-dropdown-edit-svg option:selected").text();
        console.log(imageToEdit);        

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/components/' + imageToEdit,   //The server endpoint we are connecting to

            success: function (data) {
                console.log(data);
                for (let svgStruct in data) {
                    document.getElementsByName('editTitle')[0].value = data[svgStruct].title;
                    document.getElementsByName('editDescription')[0].value = data[svgStruct].description;
                    alert("Successfully displayed CURRENT title/desc of " + imageToEdit + " to Edit SVG form");
                }
            },
            fail: function (error) {
                // Non-200 return, do something with error
                alert("Could not display current title/desc of " + imageToEdit + " to Edit SVG form");
                console.log(error);
            }
        });
    });

    // create SVG btn.Click 
    $('#btn-create-svg').on('click', function (e) {
        e.preventDefault();
        let filename = $("#fname").val()
        let svgTitle = document.getElementById('newTitle').value;
        let svgDesc = document.getElementById('newDescription').value;

        if (filename.length == 0){
            alert("filename cannot be blank");
            return;
        }

        if (svgTitle.length > 256 || svgDesc.length > 256) {
            alert('title or description is too large. Keep under 256 characters');
            return;
        }

        var currFiles = document.getElementById('log-table');
        for (var i = 1; i < currFiles.rows.length; i++) {
            if (filename + '.svg' == currFiles.rows[i].cells[1].textContent || filename == currFiles.rows[i].cells[1].textContent) {
                alert('File exists already, please enter a unique file name');
                return;
            }
        }

        // temp server crash fix... this causes a non-empty description with nothing inside but a space character need to fix
        if(svgDesc.length == 0) {
            svgDesc = "";
        }

        if(svgTitle.length == 0) {
            svgTitle = "";
        }

        if(filename.slice(filename.length - 4) != ".svg"){
            filename += ".svg";
        }

        let svg = {
            'title' : svgTitle,
            'description' : svgDesc
        }
        
        let svgJSON = JSON.stringify(svg);

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/svgcreate',   //The server endpoint we are connecting to
            data: {
                filename: filename,
                svgJSON: svgJSON
            },            
            success: function (data) {
                    alert("successfully created new SVG file");
                    clearAllForms();
                    $('#log-table').append("<tr><th scope=\"row\"><a download href=\"uploads/" + filename + "\"><img class='table-img' src=\"uploads/" + filename + "\">" + "</th><td>" + "<a href=\"uploads/" + filename + "\" download>" + filename + "</a><td>" + data["sizeKB"] +"KB</td><td>" + data["numRect"] + "</td><td>" + data["numCirc"] + "</td><td>" + data["numPaths"] + "</td><td>" + data["numGroups"] + "</td>");
                    $('#svg-dropdown').append("<option value=\"" + filename + "\"" + ">" + filename + "</option>");
                    $('#svg-dropdown-add-rectangle').append("<option value=\"" + filename + "\"" + ">" + filename + "</option>");
                    $('#svg-dropdown-add-circle').append("<option value=\"" + filename + "\"" + ">" + filename + "</option>");
                    $('#svg-dropdown-edit-svg').append("<option value=\"" + filename + "\"" + ">" + filename + "</option>");
            },
            fail: function (error) {
                // Non-200 return, do something with error
                alert("failed to create SVG file");
                console.log(error);
            }
        });
    });

    // edit SVG btn.Click 
    $('#btn-edit-svg').on('click', function() {
        let editSVGFNToUpdate = $("#svg-dropdown-edit-svg").children("option:selected").val();
        let titleEdits = $('#editTitle').val();
        let descEdits = $('#editDescription').val();

        if(editSVGFNToUpdate == "-- None Selected --"){
            alert("please select a svg image to edit its title/description!");
            return;
        }

        let svg = {
            'title' : titleEdits,
            'description' : descEdits
        }
        
        let svgJSON = JSON.stringify(svg);

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/edit-svg/' + editSVGFNToUpdate,   //The server endpoint we are connecting to  
            data: {
                filename: editSVGFNToUpdate,
                svgJSON: svgJSON
            },   
            success: function (data) {
                alert("The file you want edit the title/desc is:  "+editSVGFNToUpdate+"\nTitle is now: "+titleEdits+", description is now: "+descEdits);
                clearEditSVGForm();
            },
            fail: function (error) {
                // Non-200 return, do something with error
                alert("failed to edit SVG file");
                console.log(error);
            }
        });
    });

    // edit SVG btn.Click 
    $('#btn-edit-attr').on('click', function() {
        let editAttrFNToUpdate = $("#component-dropdown-edit").children("option:selected").val();
        let nullDDL = $("#component-dropdown-edit").children("option:selected").val();
        let attrNameEdits = $('#editAttrName').val();
        let attrValueEdits = $('#editAttrValue').val();

        if(editAttrFNToUpdate == "-- No Component Selected --" || nullDDL == null){
            alert("please select an image component to edit its attributes!");
            return;
        }

        let attr = {
            'name' : attrNameEdits,
            'value' : attrNameEdits
        }
        
        let attrJSON = JSON.stringify(attr);

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/edit-attr/' + editAttrFNToUpdate,   //The server endpoint we are connecting to  
            data: {
                filename: editAttrFNToUpdate,
                attrJSON: attrJSON,
            },   
            success: function (data) {
                alert("dummy button click alert for edit attribute\n"+"component to load attributes from: "+editAttrFNToUpdate+"\nattrName: "+attrNameEdits+", attrValue is now: "+attrValueEdits);
                clearEditAttributeForm();
                alert("The Component you want edit the Attr is:  "+editAttrFNToUpdate+"\nTitle is now: "+attrNameEdits+", description is now: "+attrNameEdits);
                clearEditSVGForm();
            },
            fail: function (error) {
                // Non-200 return, do something with error
                alert("failed to edit SVG file");
                console.log(error);
            }
        });
    });

    // add rectangle btn.Click 
    $('#btn-add-rectangle').on('click', function() {
        let rectFNToUpdate = "uploads/" + $("#svg-dropdown-add-rectangle").children("option:selected").val();
        let x = $('#x').val();
        let y = $('#y').val();
        let height = $('#height').val();
        let width = $('#width').val();
        let rectUnits = $('#rectUnits').val();

        if(rectFNToUpdate == "uploads/-- None Selected --"){
            alert("please select a file to add rectangles to!");
            return;
        }
        
        if(width < 0 || height < 0) {
            alert("width / height cannot be negative!");
            return;
        }

        if(isNaN(width) == 1 || isNaN(height) == 1 || isNaN(y) == 1 || isNaN(x) == 1){
            alert("invalid input! only numbers are accepted for x, y, height, and width");
            return;
        }

        alert("The file you want add the new rectangle to is located: "+rectFNToUpdate+"\nNew Rectangle with values of { x: "+x+rectUnits+", y: "+y+rectUnits+", height: "+height+rectUnits+", width: "+width+rectUnits+" }");
        clearRectangleForm();
    });

    // add circle btn.Click 
    $('#btn-add-circle').on('click', function() {
        let circFNToUpdate = "uploads/" + $("#svg-dropdown-add-circle").children("option:selected").val();
        let cx = $('#cx').val();
        let cy = $('#cy').val();
        let radius = $('#radius').val();
        let circUnits = $('#circUnits').val();

        if(circFNToUpdate == "uploads/-- None Selected --"){
            alert("please select a file to add circles to!");
            return;
        }

        if(radius < 0) {
            alert("Radius cannot be negative!");
            return;
        }
        if(isNaN(cx) == 1 || isNaN(cy) == 1 || isNaN(radius) == 1){
            alert("invalid input! only numbers are accepted for cx, cy, radius");
            return;
        }

        alert("The file you want add the new circle to is located: "+circFNToUpdate+"\nNew Circle with values of { cx: "+cx+circUnits+", cy: "+cy+circUnits+", radius: "+radius+circUnits+" }");
        clearCircleForm();
    });

    // scale shape btn.Click 
    $('#btn-scale-shape').on('click', function() {
        let shapeToUpdate = "uploads/" + $("#svg-dropdown-scale-shapes").children("option:selected").val();
        let scaleFactor = $('#scale-factor').val();

        if(shapeToUpdate == "uploads/-- None Selected --"){
            alert("please select a file to scale!");
            return;
        }

        if(isNaN(scaleFactor) == 1){
            alert("invalid input! only numbers are accepted for scale factor");
            return;
        }
        alert("The shape you want to scale is "+shapeToUpdate+"\nScale Factor: "+scaleFactor);
        clearScaleForm();
    });

    //**********************A4 Functions **********************
    $('#db-login').submit(function(e){
        e.preventDefault();

        document.getElementById("db-actions").style.display = "block";

        let username = $("#username").val();
        let password = $("#password").val();
        let dbname = $("#db-name").val();

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/login/',
            data: {
                username: username,
                password: password,
                dbname: dbname,
            },
            success: function(data) {
                console.log(data);
                if(data === "Success")
                {
                    alert("Valid Credentials, Successfully Logged Into Database!");
                    document.getElementById("db-actions").style.display = "block"; // reveal db actions once logged in
                }
                else{
                    alert("Invalid Credentials! Please Try Again");
                }
            },
            fail: function(error) {
                console.log(error);
                alert("There was an error while logging in");
            }
        });
    })

});

function refreshFileLogandDropdown() {
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/getFiles',   //The server endpoint we are connecting to
        success: function (data) {
            for (var i = 0; i < data.length; i++) {
                let json = JSON.parse(data[i]);
                $('#log-table').append("<tr><th scope=\"row\"><a download href=\"uploads/" + json["filename"] + "\"><img class='table-img' src=\"uploads/" + json["filename"] + "\">" + "</th><td>" + "<a href=\"uploads/" + json["filename"] + "\" download>" + json["filename"] + "</a><td>" + json["sizeKB"] + "KB</td><td>" + json["numRect"] + "</td><td>" + json["numCirc"] + "</td><td>" + json["numPaths"] + "</td><td>" + json["numGroups"] + "</td>");
                $('#svg-dropdown').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
                $('#svg-dropdown-edit-svg').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
                $('#svg-dropdown-add-rectangle').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
                $('#svg-dropdown-add-circle').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
                $('#svg-dropdown-scale-shapes').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
                alert('Successfully added '+json["filename"]+' to the Panel');
            }
            // enable vertical scrolling if the file log panel contains more than 5 files
            if ($('#log-table tr').length > 6)
                $('#log-table').addClass('enable-scroll');
            // display this in file log panel if there are no files
            else if ($('#log-table tbody tr').length < 1)
                $('#log-table').append("<tr><td>No files</td></tr>");
        },
        fail: function (error) {
            // Non-200 return, do something with error
            alert('Could not display ' + json["filename"] + ' to the View Panel');
            console.log(error);
        }
    });
}

function clearAllForms() {
    $('#new-svg').trigger("reset");
    clearEditSVGForm();
    clearEditAttributeForm();
    clearCircleForm();
    clearRectangleForm();
    clearScaleForm();
}

function clearCircleForm() {
    $('#add-circle').trigger("reset");
}

function clearRectangleForm() {
    $('#add-rectangle').trigger("reset");
}

function clearScaleForm() {
    document.getElementsByName('scale-factor')[0].value = "";
}

function clearEditSVGForm() {
    document.getElementsByName('editTitle')[0].value = "";
    document.getElementsByName('editDescription')[0].value = "";
}

function clearEditAttributeForm() {
    document.getElementsByName('editAttrName')[0].value = "";
    document.getElementsByName('editAttrValue')[0].value = "";
}