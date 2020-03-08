// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // enable vertical scrolling if the file log panel contains more than 5 files
    if ($('#log-table tr').length > 6)
    $('#log-table').addClass('enable-scroll');
    // -------------- Denis A3 stub --------------
        // // On page-load AJAX Example
        // $.ajax({
        //     type: 'get',            //Request type
        //     dataType: 'json',       //Data type - we will use JSON for almost everything 
        //     url: '/someendpoint',   //The server endpoint we are connecting to
        //     data: {
        //         name1: "Value 1",
        //         name2: "Value 2"
        //     },
        //     success: function (data) {
        //         /*  Do something with returned object
        //             Note that what we get is an object, not a string, 
        //             so we do not need to parse it on the server.
        //             JavaScript really does handle JSONs seamlessly
        //         */
        //         $('#blah').html("On page load, received string '"+data.foo+"' from server");
        //         //We write the object to the console to show that the request was successful
        //         console.log(data); 

        //     },
        //     fail: function(error) {
        //         // Non-200 return, do something with error
        //         $('#blah').html("On page load, received error from server");
        //         console.log(error); 
        //     }
        // });

        // On click upload AJAX Example
        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/svg',   //The server endpoint we are connecting to
            
            success: function (data) {
                for(var i = 0; i <data.length; i++)
                {
                    let json = JSON.parse(data[i]);
                    $('#log-table').append("<tr><th scope=\"row\"><img class='table-img' src=\"uploads/"+ json["filename"] + "\">" + "</th><td>" + "<a href=\"uploads/" + json["filename"] + "\">" + json["filename"] + "</a><td>" + json["fileSize"] + "</td><td>" + json["numRect"] + "</td><td>" + json["numCirc"] + "</td><td>" + json["numPaths"] + "</td><td>" + json["numGroups"] + "</td>");
				    $('#svg-dropdown').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
				    alert('Successfully added ' + json["filename"] + ' to View Panel');
                }

            },
            fail: function(error) {
                // Non-200 return, do something with error
                alert('Could not add ' + json["filename"] + ' to View Panel');
                console.log(error); 
            }
        });

        // Event listener form example , we can use this instead explicitly listening for events
        // No redirects if possible
        $('#someform').submit(function(e){
            $('#blah').html("Form has data: "+$('#entryBox').val());
            e.preventDefault();
            //Pass data to the Ajax call, so it gets passed to the server
            $.ajax({
                //Create an object for connecting to another waypoint
            });
        });
});