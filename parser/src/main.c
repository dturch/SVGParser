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

    // addComponent Rectangle, Circle, Path -----------------------------------------
/*    
    Rectangle* rect = malloc(sizeof(Rectangle));
    rect->height = 24; 
    rect->width = 97;
    rect->x = 9;
    rect->y = 0.77;
    strcpy(rect->units, "cm");
    rect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    addComponent(svgImage, RECT, rect);

    Circle* circ = malloc(sizeof(Circle));
    circ->r = 3.6; 
    circ->cx = 40.4;
    circ->cy = 34.3;
    strcpy(circ->units, "cm");
    circ->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    addComponent(svgImage, CIRC, circ);

    Path* path = malloc(sizeof(Path));
    path->data = malloc(strlen("213h4f87h13847fh178342hf8176934ghf89672g4") + 1);
    strcpy(path->data, "213h4f87h13847fh178342hf8176934ghf89672g4");
    path->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    addComponent(svgImage, PATH, path);
*/

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
    char* json = "{\"title\":\"stuff\",\"descr\":\"junk\"}";

    svgImage = JSONtoSVG(json);


    char* string = NULL;
    string = SVGimageToString(svgImage);
    printf("print start: %s\n", string);
    free(string);  

    result2 = writeSVGimage(svgImage, argv[3]); // 3rd argument is name of new file 

    printf("validateSVGimage: %d\nwriteSVGimage: %d\n", result1, result2);

    deleteSVGimage(svgImage);
    
    return 0;
}