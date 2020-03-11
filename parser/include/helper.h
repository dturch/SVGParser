/**
 * file    		helper.h
 * author  		Dario Turchi
 * studentID 	0929012
 * lastEdit    	March 10, 2020
 */
#ifndef HELPER_H
#define HELPER_H

SVGimage* createSVGImageFromTree(xmlNode* root);
Group* createGroupFromTree(xmlNode* root);
bool validSVGStruct(SVGimage* image);
bool validAttrList(List* list);
void createTreeFromDoc(xmlNodePtr root, SVGimage* svgImage);
void createGroupFromDoc(xmlNodePtr root, Group* group);
SVGimage* newSVGImage(void);
Attribute* newAttribute(char* attrName, char* content);
Rectangle* newRectangle(void);
Circle* newCircle(void);
Path* newPath(void);
Group* newGroup(void);
void getRectsRecursive(void* data, bool isImage, List* list);
void getGroupsRecursive(void* data, bool isImage, List* list);
void getPathsRecursive(void* data, bool isImage, List* list);
void getCirclesRecursive(void* data, bool isImage, List* list);

//a2 module 3 bonus functions
SVGimage* JSONtoSVG(const char* svgString);
//Rectangle* JSONtoRect(const char* svgString);
//Circle* JSONtoCircle(const char* svgString);

char * svg_struct_to_html(char * filename);
char * shapes_struct_to_html(char * filename);
char * html_to_svg_struct(char * filename, char * svgJSON);

#endif