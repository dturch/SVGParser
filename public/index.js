// Put all onload AJAX calls here, and event listeners
$(document).ready(function () {
    // SVG file upload 
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/svg',   //The server endpoint we are connecting to

        success: function (data) {
            for (var i = 0; i < data.length; i++) {
                let json = JSON.parse(data[i]);
                $('#log-table').append("<tr><th scope=\"row\"><a download href=\"uploads/" + json["filename"] + "\"><img class='table-img' src=\"uploads/" + json["filename"] + "\">" + "</th><td>" + "<a href=\"uploads/" + json["filename"] + "\" download>" + json["filename"] + "</a><td>" + "1KB</td><td>" + json["numRect"] + "</td><td>" + json["numCirc"] + "</td><td>" + json["numPaths"] + "</td><td>" + json["numGroups"] + "</td>");
                $('#svg-dropdown').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
                alert('Successfully added ' + json["filename"] + ' to View Panel');
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
            alert('Could not add ' + json["filename"] + ' to View Panel');
            console.log(error);
        }
    });

    // SVG Image Component Summary
    $('#svg-dropdown').change(function () {
        let filename = $("#svg-dropdown option:selected").text();

        console.log(filename);

        $('#rectangle-summary tr').remove();
        $('#circle-summary tr').remove();
        $('#path-summary tr').remove();
        $('#group-summary tr').remove();
        $('#svg-display-img img').remove();
        $('#svg-display-img h5').remove();

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/components/' + filename,   //The server endpoint we are connecting to

            success: function (data) {
                console.log(data);
                for (let shapes in data) {
                    for (var r = 0; r < data[shapes].rectangle.length; r++) {
                        $("#rectangle-summary").append("<tr><td>Rectangle " + (r + 1) + "</td><td>Upper left corner: x = " + data[shapes].rectangle[r].x + data[shapes].rectangle[r].units + ", y = " + data[shapes].rectangle[r].y + data[shapes].rectangle[r].units + ", Width: " + data[shapes].rectangle[r].w + data[shapes].rectangle[r].units + ", Height: " + data[shapes].rectangle[r].h + data[shapes].rectangle[r].units + "</td><td>" + data[shapes].rectangle[r].numAttr + "</td></tr>");
                    }
                    for (var c = 0; c < data[shapes].circle.length; c++) {
                        $("#circle-summary").append("<tr><td>Circle " + (c + 1) + "</td><td>Centre: x = " + data[shapes].circle[c].cx + data[shapes].circle[c].units + ", y = " + data[shapes].circle[c].y + data[shapes].circle[c].units + ", radius: " + data[shapes].circle[c].r + data[shapes].circle[c].units + "</td><td>" + data[shapes].circle[c].numAttr + "</td></tr>");
                    }
                    for (var p = 0; p < data[shapes].path.length; p++) {
                        $("#path-summary").append("<tr><td>Path " + (p + 1) + "</td><td>path data = " + data[shapes].path[p].d + "</td><td>" + data[shapes].path[p].numAttr + "</td></tr>");
                    }
                    for (var g = 0; g < data[shapes].group.length; g++) {
                        $("#group-summary").append("<tr><td>Group " + (g + 1) + "</td><td>" + data[shapes].group[g].children + " child elements</td><td>" + data[shapes].group[g].numAttr + "</td></tr>");
                    }
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

    // SVG Image Properties
    /* 
        $('#component-dropdown').change(function() {
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
                    // Non-200 return, do something with error
                    alert("Could not display additional properites of " + component);
                    console.log(error);
                }
            });
        });
    */

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    // $('#someform').submit(function (e) {
    //     $('#blah').html("Form has data: " + $('#entryBox').val());
    //     e.preventDefault();
    //     //Pass data to the Ajax call, so it gets passed to the server
    //     $.ajax({
    //         //Create an object for connecting to another waypoint
    //     });
    // });
});