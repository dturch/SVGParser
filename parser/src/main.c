#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlschemastypes.h>
#include "LinkedListAPI.h"
#include "SVGParser.h"
#include "helper.h"

int main(int argc, char *argv[]) 
{
    bool result1 = 0;
    bool result2 = 0;

    SVGimage *svgImage = createValidSVGimage(argv[1], argv[2]); // 1st argument is svg file to pass through

    
    result1 = validateSVGimage(svgImage, argv[2]); // 2nd argument is schema file

    Attribute* attr = malloc(sizeof(Attribute));
    attr->name = malloc(strlen("fill") + 1);
    attr->value = malloc(strlen("#ff666f") + 1);
    strcpy(attr->name, "fill");
    strcpy(attr->value, "#ff666f");
    setAttribute(svgImage, RECT, 1, attr);

    result2 = writeSVGimage(svgImage, argv[3]); // 3rd argument is name of new file 
    printf("validateSVGimage: %d\nwriteSVGimage: %d\n", result1, result2);




    // ListToJSON functions test -----------------------------------------
/*
    if(svgImage != NULL)
    {
        char* Jsvg = NULL;
        Jsvg = SVGtoJSON(svgImage);
        printf("SVGToJSON: %s\n", Jsvg);

        char* JattrList = NULL;
        char* JrectList = NULL;
        char* JcircList = NULL;
        char* JpathList = NULL;
        char* JgroupList = NULL;

        JattrList = attrListToJSON(svgImage->otherAttributes);

        List* rectList = getRects(svgImage);
        JrectList = rectListToJSON(rectList);
        
        List* circList = getCircles(svgImage);
        JcircList = circListToJSON(circList);
        
        List* pathList = getPaths(svgImage);
        JpathList = pathListToJSON(pathList);
        
        List* groupList = getGroups(svgImage);
        JgroupList = groupListToJSON(groupList);

        printf("attrListJSON: %s\n", JattrList);
        printf("rectListJSON: %s\n", JrectList);
        printf("circListJSON: %s\n", JcircList);
        printf("pathListJSON: %s\n", JpathList);
        printf("groupListJSON: %s\n", JgroupList);

        free(Jsvg);
        free(JattrList);
        free(JrectList);
        free(JcircList);
        free(JpathList);
        free(JgroupList);
        
        freeList(pathList);
        freeList(rectList);
        freeList(circList);
        freeList(groupList);
    

        // NEEED TO TEST FOR INVALIDITY AFTER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        Attribute* attr1 = malloc(sizeof(Attribute));
        attr1->name = malloc(strlen("fill") + 1);
        attr1->value = malloc(strlen("#ffff00") + 1);
        strcpy(attr1->name, "fill");
        strcpy(attr1->value, "#ffff00");
        setAttribute(svgImage, SVG_IMAGE, 0, attr1);

        result2 = writeSVGimage(svgImage, argv[3]); // 3rd argument is name of new file 
        printf("validateSVGimage: %d\nwriteSVGimage: %d\n", result1, result2);
    }
*/


    // toString functions test -----------------------------------------
/*
    char* string = NULL;
    string = SVGimageToString(svgImage);
    printf("print start: %s\n", string);
    free(string);  
*/

    deleteSVGimage(svgImage);
    
    return 0;
}