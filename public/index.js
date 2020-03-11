// Put all onload AJAX calls here, and event listeners
$(document).ready(function () {
    refreshFileLogandDropdown();
    clearAllForms();

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

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/components/' + filename,   //The server endpoint we are connecting to

            success: function (data) {
                console.log(data);
                for (let svgStruct in data) {
                    for (var r = 0; r < data[svgStruct].rectangle.length; r++) {
                        $("#rectangle-summary").append("<tr><td>Rectangle " + (r + 1) + "</td><td>Upper left corner: x = " + data[svgStruct].rectangle[r].x + data[svgStruct].rectangle[r].units + ", y = " + data[svgStruct].rectangle[r].y + data[svgStruct].rectangle[r].units + ", Width: " + data[svgStruct].rectangle[r].w + data[svgStruct].rectangle[r].units + ", Height: " + data[svgStruct].rectangle[r].h + data[svgStruct].rectangle[r].units + "</td><td>" + data[svgStruct].rectangle[r].numAttr + "</td></tr>");
                    }
                    for (var c = 0; c < data[svgStruct].circle.length; c++) {
                        $("#circle-summary").append("<tr><td>Circle " + (c + 1) + "</td><td>Centre: x = " + data[svgStruct].circle[c].cx + data[svgStruct].circle[c].units + ", y = " + data[svgStruct].circle[c].cy + data[svgStruct].circle[c].units + ", radius: " + data[svgStruct].circle[c].r + data[svgStruct].circle[c].units + "</td><td>" + data[svgStruct].circle[c].numAttr + "</td></tr>");
                    }
                    for (var p = 0; p < data[svgStruct].path.length; p++) {
                        $("#path-summary").append("<tr><td>Path " + (p + 1) + "</td><td>path data = " + data[svgStruct].path[p].d + "</td><td>" + data[svgStruct].path[p].numAttr + "</td></tr>");
                    }
                    for (var g = 0; g < data[svgStruct].group.length; g++) {
                        $("#group-summary").append("<tr><td>Group " + (g + 1) + "</td><td>" + data[svgStruct].group[g].children + " child elements</td><td>" + data[svgStruct].group[g].numAttr + "</td></tr>");
                    }
                    $('#svg-title-desc').append("<tr><td>" + data[svgStruct].title + "</td><td>" + data[svgStruct].description + "</td></tr>");
                }
                $('#svg-display-img').append("<h5 class=\"blue\">" + filename + "</h5><img class=\"svg-display-img\" src=\"" + filename + "\">");
                // populate dropdownlist of components to diplay
                $('#component-dropdown').append("<option value=\"dummyCircle\"" + ">" + "dummy Circle 1" + "</option>");
                alert("Successfully displayed summary of " + filename + " to SVG View Panel");
            },
            fail: function (error) {
                // Non-200 return, do something with error
                alert("Could not display summary of " + filename + " to SVG View Panel");
                console.log(error);
            }
        });
    });

    $('#component-dropdown').change(function () {
        let component = $("#component-dropdown option:selected").text();

        console.log(component);

        $('#component-properties tr').remove();

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/components-properties/' + component,   //The server endpoint we are connecting to

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
            if (filename == currFiles.rows[i].cells[1].textContent) {
                alert('File exists already, please enter a unique file name');
                return;
            }
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
                    $('#log-table').append("<tr><th scope=\"row\"><a download href=\"uploads/" + filename + "\"><img class='table-img' src=\"uploads/" + filename + "\">" + "</th><td>" + "<a href=\"uploads/" + filename + "\" download>" + filename + "</a><td>" + "1KB</td><td>" + data["numRect"] + "</td><td>" + data["numCirc"] + "</td><td>" + data["numPaths"] + "</td><td>" + data["numGroups"] + "</td>");
                    $('#svg-dropdown').append("<option value=\"" + filename + "\"" + ">" + filename + "</option>");
            },
            fail: function (error) {
                // Non-200 return, do something with error
                alert("failed to create SVG file");
                console.log(error);
            }
        });
    });
});
/// ---- end of $(document).ready(function())

function refreshFileLogandDropdown() {
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/getFiles',   //The server endpoint we are connecting to
        success: function (data) {
            for (var i = 0; i < data.length; i++) {
                let json = JSON.parse(data[i]);
                $('#log-table').append("<tr><th scope=\"row\"><a download href=\"uploads/" + json["filename"] + "\"><img class='table-img' src=\"uploads/" + json["filename"] + "\">" + "</th><td>" + "<a href=\"uploads/" + json["filename"] + "\" download>" + json["filename"] + "</a><td>" + "1KB</td><td>" + json["numRect"] + "</td><td>" + json["numCirc"] + "</td><td>" + json["numPaths"] + "</td><td>" + json["numGroups"] + "</td>");
                // $('#log-table').append("<tr>");
                // $('#log-table').append("<th scope=\"row\"><a download href=\"uploads/" + json["filename"] + "\"><img class='table-img' src=\"uploads/" + json["filename"] + "\">" + "</th>");
                // $('#log-table').append("<td><a href=\"uploads/" + json["filename"] + "\" download>" + json["filename"] + "</a>");
                // $('#log-table').append("<td>" + json["sizeKB"] + "KB</td>");
                // $('#log-table').append("<td>" + json["numRect"] + "</td>");
                // $('#log-table').append("<td>" + json["numCirc"] + "</td>");
                // $('#log-table').append("<td>" + json["numPaths"] + "</td>");
                // $('#log-table').append("<td>" + json["numGroups"] + "</td>");
                // $('#log-table').append("</tr>");
                $('#svg-dropdown').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
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
            alert('Could not display ' + json["filename"] + ' to View Panel');
            console.log(error);
        }
    });
}

function clearAllForms() {
    $('#new-svg').trigger("reset");
}