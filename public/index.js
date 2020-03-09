// Put all onload AJAX calls here, and event listeners
$(document).ready(function () {
    // On click upload AJAX Example
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
    $('#svg-dropdown').change(function() {
        let filename = $("#svg-dropdown option:selected").text();

        console.log(filename);

        $('#component-summary tr').remove();
        $('#svg-display-img img').remove();
        $('#svg-display-img h5').remove();

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/components/' + filename,   //The server endpoint we are connecting to

            success: function (data) {
                console.log(data);
                for (var i = 0; i < data.length; i++) {
                    console.log(data[i]);
                    $("#component-summary").append("<tr><td> Rectangle " + (i + 1) + "</td><td>" + data[i]["x"] + "</td><td>" + data[i]["numAttr"] + "</td></tr>");
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