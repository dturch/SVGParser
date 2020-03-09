/**
 * file    		helper.h
 * author  		Dario Turchi
 * studentID 	0929012
 * lastEdit    	February 19, 2020
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

#endif