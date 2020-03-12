/**
 * file    		SVGParser.c
 * author  		Dario Turchi
 * studentID 	0929012
 * lastEdit     March 11, 2020
 */
#include "SVGParser.h"
#include "helper.h"
#include "LinkedListAPI.h"
#include <math.h>
#include <strings.h>

// ******************************* START: A1 - Module 1: createSVGimage, SVGimageToString, deleteSVGimage *******************************
	// Function to create an SVG object based on the contents of an SVG file
	SVGimage *createSVGimage(char *fileName)
	{
		if (!fileName || (strcmp(fileName, "") == 0))
			return NULL;

		/***************************************************************************
				*  tree1.c Example from XMLsoft.org, Written By Dodji Seketeli
				*  URL - http://www.xmlsoft.org/examples/tree1.c
				*  code with 'xml' in name are borrowed from this source cited above
				***************************************************************************/

		xmlDoc *doc = NULL;
		xmlNode *rootElement = NULL;

		// this initialize the library and check potential ABI mismatches between the version it was compiled for and the actual shared library used.
		LIBXML_TEST_VERSION;

		// parse the file and get the DOM
		doc = xmlReadFile(fileName, NULL, 0);

		if (!doc)
			return NULL;

		// get the root element node
		rootElement = xmlDocGetRootElement(doc);

		SVGimage *svgImage = createSVGImageFromTree(rootElement);

		/*free the document */
		xmlFreeDoc(doc);

		/*Free the global variables that mayhave been allocated by the parser.*/
		xmlCleanupParser();

		return svgImage;
	}

	// Function to create a string representation of an SVG object.
	char *SVGimageToString(SVGimage *img)
	{
		if (!img)
			return NULL;

		char *attrs = toString(img->otherAttributes);
		char *rects = toString(img->rectangles);
		char *circs = toString(img->circles);
		char *paths = toString(img->paths);
		char *groups = toString(img->groups);

		char *string = malloc(1000 + strlen(attrs) + strlen(rects) + strlen(circs) + strlen(paths) + strlen(groups));

		strcpy(string, "\n------START OF PRINT------\nnamespace: ");
		strcat(string, img->namespace);
		strcat(string, "\ntitle: ");
		strcat(string, img->title);
		strcat(string, "\ndescription: ");
		strcat(string, img->description);
		strcat(string, "\notherAttributes(");
		strcat(string, attrs);
		strcat(string, "\n)\n\nRectangles(");
		strcat(string, rects);
		strcat(string, "\n)\n\nCircles(");
		strcat(string, circs);
		strcat(string, "\n)\n\nPaths(");
		strcat(string, paths);
		strcat(string, "\n)\n\nGroup...");
		strcat(string, groups);
		strcat(string, "\n------END of PRINT------\n\n");

		// free strings created
		free(attrs);
		free(rects);
		free(circs);
		free(paths);
		free(groups);

		return string;
	}

	// Function to delete image content and free all the memory.
	void deleteSVGimage(SVGimage *img)
	{
		if (!img)
			return;
		// deallocate subcomponents first
		freeList(img->rectangles);
		freeList(img->circles);
		freeList(img->paths);
		freeList(img->groups);
		freeList(img->otherAttributes);
		// deallocate object
		free(img);
	}

// ******************************* START: A1 - Module 2: Accessor and search functions ***************************
	void dummy(void *data) {}

	List *getRects(SVGimage *img)
	{
		if (!img)
			return NULL;
		List *list = initializeList(rectangleToString, dummy, compareRectangles);
		getRectsRecursive(img, true, list);
		return list;
	}

	List *getCircles(SVGimage *img)
	{
		if (!img)
			return NULL;
		List *list = initializeList(circleToString, dummy, compareCircles);
		getCirclesRecursive(img, true, list);
		return list;
	}

	List *getPaths(SVGimage *img)
	{
		if (!img)
			return NULL;
		List *list = initializeList(pathToString, dummy, comparePaths);
		getPathsRecursive(img, true, list);
		return list;
	}

	List *getGroups(SVGimage *img)
	{
		if (!img)
			return NULL;
		List *list = initializeList(groupToString, dummy, compareGroups);
		getGroupsRecursive(img, true, list);
		return list;
	}

	int numRectsWithArea(SVGimage *img, float area)
	{
		if (!img || area < 0.0)
			return 0;

		int num = 0;
		float height = 0.0;
		float width = 0.0;
		float rectArea = 0.0;

		Rectangle *rect;
		List *list = getRects(img);
		ListIterator itr = createIterator(list);

		while ((rect = nextElement(&itr)) != NULL)
		{
			width = (rect->width);
			height = (rect->height);
			rectArea = height * width;
			if (ceil(rectArea) == ceil(area))
				num++;
		}
		return num;
	}

	int numCirclesWithArea(SVGimage *img, float area)
	{
		if (!img || area < 0.0)
			return 0;

		int num = 0;
		const float pi = 3.1415926535;
		float circArea = 0.0;
		float radius = 0.0;

		Circle *circ;
		List *list = getCircles(img);
		ListIterator itr = createIterator(list);

		while ((circ = nextElement(&itr)) != NULL)
		{
			radius = (circ->r);
			circArea = (pi * radius * radius);
			if (ceil(circArea) == ceil(area))
				num++;
		}
		return num;
	}

	int numPathsWithdata(SVGimage *img, char *data)
	{
		if (!img || !data)
			return 0;

		int num = 0;

		Path *pth;
		List *list = getPaths(img);
		ListIterator itr = createIterator(list);

		while ((pth = nextElement(&itr)) != NULL)
		{
			if ((strcmp(pth->data, data) == 0))
				num++;
		}

		return num;
	}

	int numGroupsWithLen(SVGimage *img, int len)
	{
		if (!img || len < 0)
			return 0;

		int num = 0;
		int grpLength = 0;

		Group *grp;
		List *list = getGroups(img);
		ListIterator itr = createIterator(list);

		while ((grp = nextElement(&itr)) != NULL)
		{
			grpLength = getLength(grp->circles);
			grpLength += getLength(grp->rectangles);
			grpLength += getLength(grp->paths);
			grpLength += getLength(grp->groups);
			if (grpLength == len)
				num++;
		}

		return num;
	}

	int numAttr(SVGimage *img)
	{
		if (!img)
			return 0;

		int numAttr = 0;

		numAttr += getLength(img->otherAttributes);

		List *list = getRects(img);
		ListIterator itr = createIterator(list);

		Rectangle *rect;
		while ((rect = nextElement(&itr)) != NULL)
		{
			numAttr += getLength(rect->otherAttributes);
		}

		freeList(list);
		list = getCircles(img);
		itr = createIterator(list);

		Circle *circ;
		while ((circ = nextElement(&itr)) != NULL)
		{
			numAttr += getLength(circ->otherAttributes);
		}

		freeList(list);
		list = getPaths(img);
		itr = createIterator(list);

		Path *path;
		while ((path = nextElement(&itr)) != NULL)
		{
			numAttr += getLength(path->otherAttributes);
		}

		freeList(list);
		list = getGroups(img);
		itr = createIterator(list);

		Group *grp;
		while ((grp = nextElement(&itr)) != NULL)
		{
			numAttr += getLength(grp->otherAttributes);
		}

		freeList(list);
		return numAttr;
	}

// ******************************* START: A2 - Module 1: validateSVG & writeSVG *******************************
	bool validateSVGimage(SVGimage *image, char *schemaFile)
	{
		bool valid = false;

		if (!image || !schemaFile || validSVGStruct(image) == false)
			return valid;

		/***************************************************************************
				*  tree2.c Example from XMLsoft.org, Author: Lucas Brasilino<brasilino@recife.pe.gov.br>
				*  URL - http://www.xmlsoft.org/examples/tree2.c
				*  code with 'xml' in name are borrowed from this source cited above
				***************************************************************************/

		xmlDocPtr doc = NULL;
		xmlNodePtr root = NULL;

		LIBXML_TEST_VERSION;

		doc = xmlNewDoc(BAD_CAST "1.0");
		root = xmlNewNode(NULL, BAD_CAST "svg");
		xmlDocSetRootElement(doc, root);

		createTreeFromDoc(root, image);

		xmlSchemaPtr schema = NULL;
		xmlSchemaParserCtxtPtr ctxt;

		xmlLineNumbersDefault(1);

		ctxt = xmlSchemaNewParserCtxt(schemaFile);

		xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc)fprintf, (xmlSchemaValidityWarningFunc)fprintf, stderr);
		schema = xmlSchemaParse(ctxt);
		xmlSchemaFreeParserCtxt(ctxt);

		if (!doc)
		{
			//fprintf(stderr, "Could not parse\n");
		}
		else
		{
			xmlSchemaValidCtxtPtr ctxt;
			int ret;

			ctxt = xmlSchemaNewValidCtxt(schema);
			xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc)fprintf, (xmlSchemaValidityWarningFunc)fprintf, stderr);
			ret = xmlSchemaValidateDoc(ctxt, doc);

			xmlSchemaFreeValidCtxt(ctxt);
			xmlFreeDoc(doc);

			if (ret == 0)
			{
				valid = true;
			}
			else
			{
				valid = false;
			}
		}

		// free the resource
		if (schema != NULL)
			xmlSchemaFree(schema);

		xmlSchemaCleanupTypes();
		xmlCleanupParser();
		xmlMemoryDump();

		return valid;
	}

	bool validSVGStruct(SVGimage *image) // validating the SVGimage aginst the header SVGparser.h
	{
		bool valid = false;

		if (!image)
			return valid;

		// check for empty namespace
		if (strlen(image->namespace) == 0)
			return valid;

		// check for NULL List
		if (image->rectangles == NULL || image->circles == NULL || image->paths == NULL || image->otherAttributes == NULL || image->groups == NULL)
			return valid;

		List *rectList = getRects(image);
		List *circList = getCircles(image);
		List *pathList = getPaths(image);
		List *grpList = getGroups(image);

		ListIterator itr = createIterator(rectList);
		Rectangle *rect = NULL;

		while ((rect = (Rectangle *)nextElement(&itr)) != NULL)
		{
			if (rect->height < 0 || rect->width < 0 || validAttrList(rect->otherAttributes) == false)
			{
				freeList(rectList);
				freeList(circList);
				freeList(pathList);
				freeList(grpList);
				return valid;
			}
		}

		itr = createIterator(circList);
		Circle *circ = NULL;

		while ((circ = (Circle *)nextElement(&itr)) != NULL)
		{
			if (circ->r < 0 || validAttrList(circ->otherAttributes) == false)
			{
				freeList(rectList);
				freeList(circList);
				freeList(pathList);
				freeList(grpList);
				return valid;
			}
		}

		itr = createIterator(pathList);
		Path *pth = NULL;

		while ((pth = (Path *)nextElement(&itr)) != NULL)
		{
			if (pth->data == NULL || validAttrList(pth->otherAttributes) == false)
			{
				freeList(rectList);
				freeList(circList);
				freeList(pathList);
				freeList(grpList);
				return valid;
			}
		}

		itr = createIterator(grpList);
		Group *grp = NULL;

		while ((grp = (Group *)nextElement(&itr)) != NULL)
		{
			if (grp->rectangles == NULL || grp->circles == NULL || grp->paths == NULL || grp->groups == NULL || validAttrList(grp->otherAttributes) == false)
			{
				freeList(rectList);
				freeList(circList);
				freeList(pathList);
				freeList(grpList);
				return valid;
			}
		}

		valid = validAttrList(image->otherAttributes);

		freeList(rectList);
		freeList(circList);
		freeList(pathList);
		freeList(grpList);

		return valid;
	}

	bool validAttrList(List *list) // check if attributeList is NULL or
	{
		bool valid = false;

		if (list == NULL)
			return valid;

		ListIterator itr = createIterator(list);
		Attribute *attr = NULL;

		while ((attr = (Attribute *)nextElement(&itr)) != NULL)
		{
			if (attr == NULL || attr->name == NULL || attr->value == NULL)
				return valid;
		}

		// upon success of checking
		valid = true;

		return valid;
	}

	SVGimage *createValidSVGimage(char *fileName, char *schemaFile)
	{
		if (!fileName || (strcmp(fileName, "") == 0) || !schemaFile)
			return NULL;

		/***************************************************************************
				*  tree1.c Example from XMLsoft.org, Written By Dodji Seketeli
				*  URL - http://www.xmlsoft.org/examples/tree1.c
				*  code with 'xml' in name are borrowed from this source cited above
				***************************************************************************/
		xmlDoc *doc = NULL;
		xmlNode *rootElement = NULL;

		// this initialize the library and check potential ABI mismatches between the version it was compiled for and the actual shared library used.
		LIBXML_TEST_VERSION;

		// parse the file and get the DOM
		doc = xmlReadFile(fileName, NULL, 0);

		if (!doc)
			return NULL;

		// get the root element node
		rootElement = xmlDocGetRootElement(doc);

		SVGimage *svgImage = createSVGImageFromTree(rootElement);

		// check if svgImage is valid
		if ((validateSVGimage(svgImage, schemaFile)) == 0)
			svgImage = NULL;

		/*free the document */
		xmlFreeDoc(doc);

		/*Free the global variables that mayhave been allocated by the parser.*/
		xmlCleanupParser();

		return svgImage;
	}

	bool writeSVGimage(SVGimage *image, char *fileName)
	{
		bool write = true;

		if (!image || !fileName)
			return write;

		xmlDocPtr doc = NULL;
		xmlNodePtr root = NULL;

		LIBXML_TEST_VERSION;

		doc = xmlNewDoc(BAD_CAST "1.0");
		root = xmlNewNode(NULL, BAD_CAST "svg");
		xmlDocSetRootElement(doc, root);

		createTreeFromDoc(root, image);

		if (xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 1) < 0)
		{
			write = false;
		}

		xmlFreeDoc(doc);
		xmlCleanupParser();

		return write;
	}

// ******************************* START: A2 - Module 2: setAttribute & addComponent *******************************
	void setAttribute(SVGimage *image, elementType elemType, int elemIndex, Attribute *newAttribute)
	{
		if (!image || !newAttribute || !newAttribute->name || !newAttribute->value)
		{
			if (newAttribute)
				deleteAttribute(newAttribute);
			return;
		}

		if (elemType == SVG_IMAGE)
		{
			ListIterator itr = createIterator(image->otherAttributes);

			Attribute *attr;
			while ((attr = nextElement(&itr)) != NULL)
			{
				if (strcmp(attr->name, newAttribute->name) == 0)
				{
					free(attr->value);
					attr->value = malloc(strlen(newAttribute->value) + 1);
					strcpy(attr->value, newAttribute->value);
					deleteAttribute(newAttribute);
					return;
				}
			}
			insertBack(image->otherAttributes, newAttribute);
			return;
		}

		if (elemType == CIRC)
		{
			ListIterator itr = createIterator(image->circles);

			if (elemIndex < 0 || elemIndex >= getLength(image->circles))
				return;

			Circle *circ = nextElement(&itr);
			while (circ != NULL && elemIndex-- != 0)
			{
				circ = nextElement(&itr);
			}

			if (strcmp(newAttribute->name, "cx") == 0)
			{
				circ->cx = strtof(newAttribute->value, NULL);
				deleteAttribute(newAttribute);
				return;
			}

			if (strcmp(newAttribute->name, "cy") == 0)
			{
				circ->cy = strtof(newAttribute->value, NULL);
				deleteAttribute(newAttribute);
				return;
			}

			if (strcmp(newAttribute->name, "r") == 0)
			{
				circ->r = strtof(newAttribute->value, NULL);
				deleteAttribute(newAttribute);
				return;
			}

			itr = createIterator(circ->otherAttributes);

			Attribute *attr;
			while ((attr = nextElement(&itr)) != NULL)
			{
				if (strcmp(attr->name, newAttribute->name) == 0)
				{
					free(attr->value);
					attr->value = malloc(strlen(newAttribute->value) + 1);
					strcpy(attr->value, newAttribute->value);
					deleteAttribute(newAttribute);
					return;
				}
			}
			insertBack(circ->otherAttributes, newAttribute);
			return;
		}

		if (elemType == RECT)
		{
			ListIterator itr = createIterator(image->rectangles);

			if (elemIndex < 0 || elemIndex >= getLength(image->rectangles))
				return;

			Rectangle *rect = nextElement(&itr);
			while (rect != NULL && elemIndex-- != 0)
			{
				rect = nextElement(&itr);
			}

			if (strcmp(newAttribute->name, "x") == 0)
			{
				rect->x = strtof(newAttribute->value, NULL);
				deleteAttribute(newAttribute);
				return;
			}

			if (strcmp(newAttribute->name, "y") == 0)
			{
				rect->y = strtof(newAttribute->value, NULL);
				deleteAttribute(newAttribute);
				return;
			}

			if (strcmp(newAttribute->name, "width") == 0)
			{
				rect->width = strtof(newAttribute->value, NULL);
				deleteAttribute(newAttribute);
				return;
			}

			if (strcmp(newAttribute->name, "height") == 0)
			{
				rect->height = strtof(newAttribute->value, NULL);
				deleteAttribute(newAttribute);
				return;
			}

			itr = createIterator(rect->otherAttributes);

			Attribute *attr;
			while ((attr = nextElement(&itr)) != NULL)
			{
				if (strcmp(attr->name, newAttribute->name) == 0)
				{
					free(attr->value);
					attr->value = malloc(strlen(newAttribute->value) + 1);
					strcpy(attr->value, newAttribute->value);
					deleteAttribute(newAttribute);
					return;
				}
			}
			insertBack(rect->otherAttributes, newAttribute);
			return;
		}

		if (elemType == PATH)
		{
			ListIterator itr = createIterator(image->paths);

			if (elemIndex < 0 || elemIndex >= getLength(image->paths))
				return;

			Path *path = nextElement(&itr);
			while (path != NULL && elemIndex-- != 0)
			{
				path = nextElement(&itr);
			}

			if (strcmp(newAttribute->name, "d") == 0)
			{
				free(path->data);
				path->data = malloc(strlen(newAttribute->value) + 1);
				strcpy(path->data, newAttribute->value);
				deleteAttribute(newAttribute);
				return;
			}

			itr = createIterator(path->otherAttributes);

			Attribute *attr;
			while ((attr = nextElement(&itr)) != NULL)
			{
				if (strcmp(attr->name, newAttribute->name) == 0)
				{
					free(attr->value);
					attr->value = malloc(strlen(newAttribute->value) + 1);
					strcpy(attr->value, newAttribute->value);
					deleteAttribute(newAttribute);
					return;
				}
			}
			insertBack(path->otherAttributes, newAttribute);
			return;
		}

		if (elemType == GROUP)
		{
			ListIterator itr = createIterator(image->groups);

			if (elemIndex < 0 || elemIndex >= getLength(image->groups))
				return;

			Group *grp = nextElement(&itr);
			while (grp != NULL && elemIndex-- != 0)
			{
				grp = nextElement(&itr);
			}

			itr = createIterator(grp->otherAttributes);

			Attribute *attr;
			while ((attr = nextElement(&itr)) != NULL)
			{
				if (strcmp(attr->name, newAttribute->name) == 0)
				{
					free(attr->value);
					attr->value = malloc(strlen(newAttribute->value) + 1);
					strcpy(attr->value, newAttribute->value);
					deleteAttribute(newAttribute);
					return;
				}
			}
			insertBack(grp->otherAttributes, newAttribute);
			return;
		}
		deleteAttribute(newAttribute);
	}

	void addComponent(SVGimage *image, elementType type, void *newElement)
	{
		if (!image || !newElement)
			return;

		if (type == CIRC)
		{
			insertBack(image->circles, (Circle *)newElement); // Dereference newElement as circle
			return;
		}

		if (type == RECT)
		{
			insertBack(image->rectangles, (Rectangle *)newElement); // Dereference newElement as rectangle
			return;
		}

		if (type == PATH)
		{
			insertBack(image->paths, (Path *)newElement); // Dereference newElement as path
			return;
		}
	}

// ******************************* START: A2 - Module 3: toJSON functions *******************************
	char *attrToJSON(const Attribute *a)
	{
		if (!a || !a->name || !a->value)
		{
			char *string = malloc(3);
			strcpy(string, "{}");
			return string;
		}

		char *string = malloc(32 + strlen(a->name) + strlen(a->value));
		strcpy(string, "{\"name\":\"");
		strcat(string, a->name);
		strcat(string, "\",\"value\":\"");
		strcat(string, a->value);
		strcat(string, "\"}");

		return string;
	}

	char *circleToJSON(const Circle *c)
	{
		if (!c)
		{
			char *string = malloc(3);
			strcpy(string, "{}");
			return string;
		}

		char *string = malloc(256);
		char str[50];

		strcpy(string, "{\"cx\":");
		snprintf(str, sizeof(str), "%.2f", c->cx);
		strcat(string, str);
		strcat(string, ",\"cy\":");
		snprintf(str, sizeof(str), "%.2f", c->cy);
		strcat(string, str);
		strcat(string, ",\"r\":");
		snprintf(str, sizeof(str), "%.2f", c->r);
		strcat(string, str);
		strcat(string, ",\"numAttr\":");
		snprintf(str, sizeof(str), "%d", getLength(c->otherAttributes));
		strcat(string, str);
		strcat(string, ",\"units\":\"");
		strcat(string, c->units);
		strcat(string, "\"}");

		return string;
	}

	char *rectToJSON(const Rectangle *r)
	{
		if (!r)
		{
			char *string = malloc(3);
			strcpy(string, "{}");
			return string;
		}

		char *string = malloc(256);
		char str[50];

		strcpy(string, "{\"x\":");
		snprintf(str, sizeof(str), "%.2f", r->x);
		strcat(string, str);
		strcat(string, ",\"y\":");
		snprintf(str, sizeof(str), "%.2f", r->y);
		strcat(string, str);
		strcat(string, ",\"w\":");
		snprintf(str, sizeof(str), "%.2f", r->width);
		strcat(string, str);
		strcat(string, ",\"h\":");
		snprintf(str, sizeof(str), "%.2f", r->height);
		strcat(string, str);
		strcat(string, ",\"numAttr\":");
		snprintf(str, sizeof(str), "%d", getLength(r->otherAttributes));
		strcat(string, str);
		strcat(string, ",\"units\":\"");
		strcat(string, r->units);
		strcat(string, "\"}");

		return string;
	}

	char *pathToJSON(const Path *p)
	{
		if (!p)
		{
			char *string = malloc(3);
			strcpy(string, "{}");
			return string;
		}

		char *string = malloc(256 + strlen(p->data));
		char str[50];

		strcpy(string, "{\"d\":\"");
		strncat(string, p->data, 64);
		strcat(string, "\",\"numAttr\":");
		snprintf(str, sizeof(str), "%d", getLength(p->otherAttributes));
		strcat(string, str);
		strcat(string, "}");

		return string;
	}

	char *groupToJSON(const Group *g)
	{
		if (!g)
		{
			char *string = malloc(3);
			strcpy(string, "{}");
			return string;
		}

		char *string = malloc(256);
		char str[50];

		strcpy(string, "{\"children\":");
		snprintf(str, sizeof(str), "%d", ((getLength(g->circles)) + (getLength(g->rectangles)) + (getLength(g->paths)) + (getLength(g->groups))));
		strcat(string, str);
		strcat(string, ",\"numAttr\":");
		snprintf(str, sizeof(str), "%d", getLength(g->otherAttributes));
		strcat(string, str);
		strcat(string, "}");

		return string;
	}

	char *attrListToJSON(const List *list)
	{
		if (!list || list->length == 0)
		{
			char *string = malloc(3);
			strcpy(string, "[]");
			return string;
		}
		int length = 0;

		ListIterator itr = createIterator((List *)list);
		Attribute *attr = nextElement(&itr);

		while (attr != NULL)
		{
			char *string = attrToJSON(attr);
			length += strlen(string) + 1;
			free(string);
			attr = nextElement(&itr);
		}

		char *string = malloc(256 + length);
		itr = createIterator((List *)list);

		attr = nextElement(&itr);

		strcpy(string, "[");
		while (attr != NULL)
		{
			char *JSONstring = attrToJSON(attr);
			strcat(string, JSONstring);
			free(JSONstring);
			if ((attr = nextElement(&itr)) != NULL)
				strcat(string, ",");
		}
		strcat(string, "]");

		return string;
	}

	char *circListToJSON(const List *list)
	{
		if (!list || list->length == 0)
		{
			char *string = malloc(3);
			strcpy(string, "[]");
			return string;
		}
		int length = 0;

		ListIterator itr = createIterator((List *)list);
		Circle *circ = nextElement(&itr);

		while (circ != NULL)
		{
			char *string = circleToJSON(circ);
			length += strlen(string) + 1;
			free(string);
			circ = nextElement(&itr);
		}

		char *string = malloc(256 + length);
		itr = createIterator((List *)list);

		circ = nextElement(&itr);

		strcpy(string, "[");
		while (circ != NULL)
		{
			char *JSONstring = circleToJSON(circ);
			strcat(string, JSONstring);
			free(JSONstring);
			if ((circ = nextElement(&itr)) != NULL)
				strcat(string, ",");
		}
		strcat(string, "]");

		return string;
	}

	char *rectListToJSON(const List *list)
	{
		if (!list || list->length == 0)
		{
			char *string = malloc(3);
			strcpy(string, "[]");
			return string;
		}
		int length = 0;

		ListIterator itr = createIterator((List *)list);
		Rectangle *rect = nextElement(&itr);

		while (rect != NULL)
		{
			char *string = rectToJSON(rect);
			length += strlen(string) + 1;
			free(string);
			rect = nextElement(&itr);
		}

		char *string = malloc(256 + length);
		itr = createIterator((List *)list);

		rect = nextElement(&itr);

		strcpy(string, "[");
		while (rect != NULL)
		{
			char *JSONstring = rectToJSON(rect);
			strcat(string, JSONstring);
			free(JSONstring);
			if ((rect = nextElement(&itr)) != NULL)
				strcat(string, ",");
		}
		strcat(string, "]");

		return string;
	}

	char *pathListToJSON(const List *list)
	{
		if (!list || list->length == 0)
		{
			char *string = malloc(3);
			strcpy(string, "[]");
			return string;
		}
		int length = 0;

		ListIterator itr = createIterator((List *)list);
		Path *path = nextElement(&itr);

		while (path != NULL)
		{
			char *string = pathToJSON(path);
			length += strlen(string) + 1;
			free(string);
			path = nextElement(&itr);
		}

		char *string = malloc(256 + length);
		itr = createIterator((List *)list);

		path = nextElement(&itr);

		strcpy(string, "[");
		while (path != NULL)
		{
			char *JSONstring = pathToJSON(path);
			strcat(string, JSONstring);
			free(JSONstring);
			if ((path = nextElement(&itr)) != NULL)
				strcat(string, ",");
		}
		strcat(string, "]");

		return string;
	}

	char *groupListToJSON(const List *list)
	{
		if (!list || list->length == 0)
		{
			char *string = malloc(3);
			strcpy(string, "[]");
			return string;
		}
		int length = 0;

		ListIterator itr = createIterator((List *)list);
		Group *grp = nextElement(&itr);

		while (grp != NULL)
		{
			char *string = groupToJSON(grp);
			length += strlen(string) + 1;
			free(string);
			grp = nextElement(&itr);
		}

		char *string = malloc(256 + length);
		itr = createIterator((List *)list);

		grp = nextElement(&itr);

		strcpy(string, "[");
		while (grp != NULL)
		{
			char *JSONstring = groupToJSON(grp);
			strcat(string, JSONstring);
			free(JSONstring);
			if ((grp = nextElement(&itr)) != NULL)
				strcat(string, ",");
		}
		strcat(string, "]");

		return string;
	}

	char *SVGtoJSON(const SVGimage *imge)
	{
		if (!imge)
		{
			char *string = malloc(3);
			strcpy(string, "{}");
			return string;
		}

		char *string = malloc(256);
		char str[50];

		List *rList = getRects((SVGimage *)imge);
		List *cList = getCircles((SVGimage *)imge);
		List *pList = getPaths((SVGimage *)imge);
		List *gpList = getGroups((SVGimage *)imge);

		strcpy(string, "{\"numRect\":");
		snprintf(str, sizeof(str), "%d", getLength(rList));
		strcat(string, str);
		strcat(string, ",\"numCirc\":");
		snprintf(str, sizeof(str), "%d", getLength(cList));
		strcat(string, str);
		strcat(string, ",\"numPaths\":");
		snprintf(str, sizeof(str), "%d", getLength(pList));
		strcat(string, str);
		strcat(string, ",\"numGroups\":");
		snprintf(str, sizeof(str), "%d", getLength(gpList));
		strcat(string, str);
		strcat(string, "}");

		freeList(rList);
		freeList(cList);
		freeList(pList);
		freeList(gpList);

		return string;
	}

// ******************************* START: A2 - Module 3: Bonus JSONto functions ***************************
	SVGimage* JSONtoSVG(const char *svgString)
	{
		if (!svgString)
			return NULL;

		ListIterator itr;
		List* properties = NULL;
		Attribute* attr = NULL;
		SVGimage* svgImage = newSVGImage();
		strcpy(svgImage->namespace, "http://www.w3.org/2000/svg");

		properties = parseJSONObj((char *)svgString);

		itr = createIterator(properties);
		while((attr = (Attribute*)nextElement(&itr)) != NULL) 
		{
			if(strcmp(attr->name, "title") == 0) 
			{
				strcpy(svgImage->title, attr->value);
				printf("the title:%s\n",svgImage->title);
        	}
			else if(strcmp(attr->name, "description") == 0) 
			{
				strcpy(svgImage->description, attr->value);
				printf("the desc:%s\n",svgImage->description);
        	}
  		}
		freeList(properties);

		return svgImage;
	}

	Rectangle* JSONtoRect(const char *svgString)
	{
		if (!svgString)
			return NULL;

		ListIterator itr;
		List* properties = NULL;
		Attribute* attr = NULL;
		Rectangle* rectangle = newRectangle();

		properties = parseJSONObj((char *)svgString);

		itr = createIterator(properties);
		while((attr = (Attribute*)nextElement(&itr)) != NULL) 
		{
			if(strcmp(attr->name, "x") == 0) 
			{
				rectangle->x = atof(attr->value);
        	}
			else if(strcmp(attr->name, "y") == 0) 
			{
				rectangle->y = atof(attr->value);
        	}
			else if(strcmp(attr->name, "width") == 0) 
			{
				rectangle->width = atof(attr->value);
        	}
			else if(strcmp(attr->name, "height") == 0) 
			{
				rectangle->height = atof(attr->value);
        	}
			else if(strcmp(attr->name, "units") == 0) 
			{
				strcpy(rectangle->units, attr->value);
        	}
  		}
		freeList(properties);

		return rectangle;
	}

	Circle* JSONtoCircle(const char *svgString)
	{
		if (!svgString)
			return NULL;

		ListIterator itr;
		List* properties = NULL;
		Attribute* attr = NULL;
		Circle* circle = newCircle();

		properties = parseJSONObj((char *)svgString);

		itr = createIterator(properties);
		while((attr = (Attribute*)nextElement(&itr)) != NULL) 
		{
			if(strcmp(attr->name, "cx") == 0) 
			{
				circle->cx = atof(attr->value);
        	}
			else if(strcmp(attr->name, "cy") == 0) 
			{
				circle->cy = atof(attr->value);
        	}
			else if(strcmp(attr->name, "r") == 0)
			{
				circle->r = atof(attr->value);
			}
  		}
		freeList(properties);

		return circle;
	}
// ******************************* A1 userCreated helper.h functions ***************************
	SVGimage *createSVGImageFromTree(xmlNode *root)
	{
		if (!root || (char *)root->ns == NULL)
			return NULL;

		xmlAttr *attr;
		SVGimage *svg = newSVGImage();
		xmlNode *currNode = root;

		// look for svg tag
		if (strcasecmp((char *)currNode->name, "svg") == 0)
		{
			// get namespace
			strncpy(svg->namespace, (char *)root->ns->href, 255);

			// svg attributes
			for (attr = currNode->properties; attr != NULL; attr = attr->next)
			{
				xmlNode *value = attr->children;
				char *attrName = (char *)attr->name;
				char *content = (char *)value->content;

				Attribute *oAttr = newAttribute(attrName, content);
				insertBack(svg->otherAttributes, oAttr);
			}
		}

		for (currNode = root->children; currNode != NULL; currNode = currNode->next)
		{
			// get title
			if (strcasecmp((char *)currNode->name, "title") == 0)
			{
				char *temp;
				temp = (char *)xmlNodeGetContent(currNode);
				strncpy(svg->title, temp, 255);
				xmlFree(temp);
			}

			// get description
			if (strcasecmp((char *)currNode->name, "desc") == 0)
			{
				char *temp;
				temp = (char *)xmlNodeGetContent(currNode);
				strncpy(svg->description, temp, 255);
				xmlFree(temp);
			}

			// look for rectangle tag
			if (strcasecmp((char *)currNode->name, "rect") == 0) // check if under parent node if not then allow
			{
				Rectangle *rect = newRectangle();
				char *buffer;

				for (attr = currNode->properties; attr != NULL; attr = attr->next)
				{
					xmlNode *value = attr->children;
					char *attrName = (char *)attr->name;
					char *content = (char *)value->content;

					if (strcasecmp(attrName, "x") == 0)
						rect->x = atof(content);
					else if (strcasecmp(attrName, "y") == 0)
						rect->y = atof(content);
					else if (strcasecmp(attrName, "width") == 0)
						rect->width = atof(content);
					else if (strcasecmp(attrName, "height") == 0)
					{
						rect->height = strtof(content, &buffer);
						strcpy(rect->units, buffer);
					}
					else
					{
						Attribute *rectAttr = newAttribute(attrName, content);
						insertBack(rect->otherAttributes, rectAttr);
					}
				}
				insertBack(svg->rectangles, rect);
			}

			// look for circle tag
			if (strcasecmp((char *)currNode->name, "circle") == 0)
			{
				Circle *circ = newCircle();
				char *buffer;

				for (attr = currNode->properties; attr != NULL; attr = attr->next)
				{
					xmlNode *value = attr->children;
					char *attrName = (char *)attr->name;
					char *content = (char *)value->content;
					if (strcasecmp(attrName, "cx") == 0)
						circ->cx = atof(content);
					else if (strcasecmp(attrName, "cy") == 0)
						circ->cy = atof(content);
					else if (strcasecmp(attrName, "r") == 0)
					{
						circ->r = strtof(content, &buffer);
						strcpy(circ->units, buffer);
					}
					else
					{
						Attribute *circAttr = newAttribute(attrName, content);
						insertBack(circ->otherAttributes, circAttr);
					}
				}
				insertBack(svg->circles, circ);
			}

			// look for path tag
			if (strcasecmp((char *)currNode->name, "path") == 0) // check if under parent node if not then allow
			{
				Path *path = newPath();

				for (attr = currNode->properties; attr != NULL; attr = attr->next)
				{
					xmlNode *value = attr->children;
					char *attrName = (char *)attr->name;
					char *content = (char *)value->content;

					if (strcasecmp(attrName, "d") == 0)
					{
						path->data = malloc(strlen(content) + 1);
						strcpy(path->data, content);
					}
					else
					{
						Attribute *pathAttr = newAttribute(attrName, content);
						insertBack(path->otherAttributes, pathAttr);
					}
				}
				insertBack(svg->paths, path);
			}

			// look for group tag
			if (strcasecmp((char *)currNode->name, "g") == 0)
			{
				insertBack(svg->groups, createGroupFromTree(currNode));
			}
		}
		return svg;
	}

	Group *createGroupFromTree(xmlNode *root)
	{
		if (!root)
			return NULL;

		xmlAttr *attr;
		Group *grp = newGroup();
		xmlNode *currNode = root;

		// look for group tag
		if (strcasecmp((char *)currNode->name, "g") == 0)
		{
			for (attr = currNode->properties; attr != NULL; attr = attr->next)
			{
				xmlNode *value = attr->children;
				char *attrName = (char *)attr->name;
				char *content = (char *)value->content;

				Attribute *oAttr = newAttribute(attrName, content);
				insertBack(grp->otherAttributes, oAttr);
			}
		}

		for (currNode = root->children; currNode != NULL; currNode = currNode->next)
		{
			// look for rectangle tag
			if (strcasecmp((char *)currNode->name, "rect") == 0) // check if under parent node if not then allow
			{
				Rectangle *rect = newRectangle();
				char *buffer;

				for (attr = currNode->properties; attr != NULL; attr = attr->next)
				{
					xmlNode *value = attr->children;
					char *attrName = (char *)attr->name;
					char *content = (char *)value->content;

					if (strcasecmp(attrName, "x") == 0)
						rect->x = atof(content);
					else if (strcasecmp(attrName, "y") == 0)
						rect->y = atof(content);
					else if (strcasecmp(attrName, "width") == 0)
						rect->width = atof(content);
					else if (strcasecmp(attrName, "height") == 0)
					{
						rect->height = strtof(content, &buffer);
						strcpy(rect->units, buffer);
					}
					else
					{
						Attribute *rectAttr = newAttribute(attrName, content);
						insertBack(rect->otherAttributes, rectAttr);
					}
				}
				insertBack(grp->rectangles, rect);
			}

			// look for path tag
			if (strcasecmp((char *)currNode->name, "path") == 0) // check if under parent node if not then allow
			{
				Path *path = newPath();

				for (attr = currNode->properties; attr != NULL; attr = attr->next)
				{
					xmlNode *value = attr->children;
					char *attrName = (char *)attr->name;
					char *content = (char *)value->content;

					if (strcasecmp(attrName, "d") == 0)
					{
						path->data = malloc(strlen(content) + 1);
						strcpy(path->data, content);
					}
					else
					{
						Attribute *pathAttr = newAttribute(attrName, content);
						insertBack(path->otherAttributes, pathAttr);
					}
				}
				insertBack(grp->paths, path);
			}

			// look for circle tag
			if (strcasecmp((char *)currNode->name, "circle") == 0)
			{
				Circle *circ = newCircle();
				char *buffer;

				for (attr = currNode->properties; attr != NULL; attr = attr->next)
				{
					xmlNode *value = attr->children;
					char *attrName = (char *)attr->name;
					char *content = (char *)value->content;

					if (strcasecmp(attrName, "cx") == 0)
						circ->cx = atof(content);
					else if (strcasecmp(attrName, "cy") == 0)
						circ->cy = atof(content);
					else if (strcasecmp(attrName, "r") == 0)
					{
						circ->r = strtof(content, &buffer);
						strcpy(circ->units, buffer);
					}
					else
					{
						Attribute *circAttr = newAttribute(attrName, content);
						insertBack(circ->otherAttributes, circAttr);
					}
				}
				insertBack(grp->circles, circ);
			}

			//look for group tag
			if (strcasecmp((char *)currNode->name, "g") == 0)
			{
				insertBack(grp->groups, createGroupFromTree(currNode));
			}
		}
		return grp;
	}

	SVGimage *newSVGImage(void)
	{
		SVGimage *svgImage = calloc(1, sizeof(SVGimage));

		svgImage->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
		svgImage->circles = initializeList(circleToString, deleteCircle, compareCircles);
		svgImage->paths = initializeList(pathToString, deletePath, comparePaths);
		svgImage->groups = initializeList(groupToString, deleteGroup, compareGroups);
		svgImage->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

		return svgImage;
	}

	Attribute *newAttribute(char *attrName, char *content)
	{
		Attribute *attr = malloc(sizeof(Attribute));
		attr->name = malloc(strlen(attrName) + 1);
		attr->value = malloc(strlen(content) + 1);

		strcpy(attr->name, attrName);
		strcpy(attr->value, content);

		return attr;
	}

	Rectangle *newRectangle(void)
	{
		Rectangle *rect = calloc(1, sizeof(Rectangle));
		rect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
		return rect;
	}

	Circle *newCircle(void)
	{
		Circle *circ = calloc(1, sizeof(Circle));
		circ->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
		return circ;
	}

	Path *newPath(void)
	{
		Path *path = malloc(sizeof(Path));
		path->data = NULL;
		path->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
		return path;
	}

	Group *newGroup(void)
	{
		Group *grp = malloc(sizeof(Group));

		grp->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
		grp->circles = initializeList(circleToString, deleteCircle, compareCircles);
		grp->paths = initializeList(pathToString, deletePath, comparePaths);
		grp->groups = initializeList(groupToString, deleteGroup, compareGroups);
		grp->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

		return grp;
	}

	void getRectsRecursive(void *data, bool isImage, List *list)
	{
		if (!data || !list)
			return;

		ListIterator itr;
		if (isImage)
		{
			itr = createIterator(((SVGimage *)data)->rectangles);
		}
		else
		{
			itr = createIterator(((Group *)data)->rectangles);
		}

		void *ptr = NULL;
		while ((ptr = nextElement(&itr)) != NULL)
		{
			insertBack(list, ptr);
		}

		if (isImage)
		{
			itr = createIterator(((SVGimage *)data)->groups);
		}
		else
		{
			itr = createIterator(((Group *)data)->groups);
		}

		while ((ptr = nextElement(&itr)) != NULL)
		{
			getRectsRecursive(ptr, false, list);
		}
	}

	void getCirclesRecursive(void *data, bool isImage, List *list)
	{
		if (!data || !list)
			return;

		ListIterator itr;
		if (isImage)
		{
			itr = createIterator(((SVGimage *)data)->circles);
		}
		else
		{
			itr = createIterator(((Group *)data)->circles);
		}

		void *ptr = NULL;
		while ((ptr = nextElement(&itr)) != NULL)
		{
			insertBack(list, ptr);
		}

		if (isImage)
		{
			itr = createIterator(((SVGimage *)data)->groups);
		}
		else
		{
			itr = createIterator(((Group *)data)->groups);
		}

		while ((ptr = nextElement(&itr)) != NULL)
		{
			getCirclesRecursive(ptr, false, list);
		}
	}

	void getPathsRecursive(void *data, bool isImage, List *list)
	{
		if (!data || !list)
			return;

		ListIterator itr;
		if (isImage)
		{
			itr = createIterator(((SVGimage *)data)->paths);
		}
		else
		{
			itr = createIterator(((Group *)data)->paths);
		}

		void *ptr = NULL;
		while ((ptr = nextElement(&itr)) != NULL)
		{
			insertBack(list, ptr);
		}

		if (isImage)
		{
			itr = createIterator(((SVGimage *)data)->groups);
		}
		else
		{
			itr = createIterator(((Group *)data)->groups);
		}

		while ((ptr = nextElement(&itr)) != NULL)
		{
			getPathsRecursive(ptr, false, list);
		}
	}

	void getGroupsRecursive(void *data, bool isImage, List *list)
	{
		if (!data || !list)
			return;

		ListIterator itr;
		if (isImage)
		{
			itr = createIterator(((SVGimage *)data)->groups);
		}
		else
		{
			itr = createIterator(((Group *)data)->groups);
		}

		void *ptr = NULL;
		while ((ptr = nextElement(&itr)) != NULL)
		{
			insertBack(list, ptr);
			getGroupsRecursive(ptr, false, list);
		}
	}

// ******************************* A2 userCreated helper.h functions ***************************
	void createTreeFromDoc(xmlNodePtr root, SVGimage *svgImage)
	{
		if (!root || !svgImage)
			return;

		xmlNodePtr rootNode = root;
		xmlNodePtr node = NULL;
		char floatStr[128];

		// set namespace
		xmlSetNs(rootNode, xmlNewNs(rootNode, BAD_CAST svgImage->namespace, NULL));

		// set SVG attributes
		if (getLength(svgImage->otherAttributes) != 0)
		{
			ListIterator itr = createIterator(svgImage->otherAttributes);

			Attribute *attr = nextElement(&itr);

			while (attr != NULL)
			{
				xmlNewProp(rootNode, BAD_CAST attr->name, BAD_CAST attr->value);
				attr = nextElement(&itr);
			}
		}

		// set title if exists
		if (strlen(svgImage->title) > 0)
		{
			xmlNewChild(rootNode, NULL, BAD_CAST "title", BAD_CAST svgImage->title);
		}

		// set description if exists
		if (strlen(svgImage->description) > 0)
		{
			xmlNewChild(rootNode, NULL, BAD_CAST "desc", BAD_CAST svgImage->description);
		}

		// creating the <rect> child node
		if (getLength(svgImage->rectangles) != 0)
		{
			ListIterator itr = createIterator(svgImage->rectangles);

			Rectangle *rect = nextElement(&itr);

			while (rect != NULL)
			{
				node = xmlNewChild(rootNode, NULL, BAD_CAST "rect", NULL);

				snprintf(floatStr, sizeof(floatStr), "%f%s", rect->x, rect->units);
				xmlNewProp(node, BAD_CAST "x", BAD_CAST floatStr);

				snprintf(floatStr, sizeof(floatStr), "%f%s", rect->y, rect->units);
				xmlNewProp(node, BAD_CAST "y", BAD_CAST floatStr);

				snprintf(floatStr, sizeof(floatStr), "%f%s", rect->height, rect->units);
				xmlNewProp(node, BAD_CAST "height", BAD_CAST floatStr);

				snprintf(floatStr, sizeof(floatStr), "%f%s", rect->width, rect->units);
				xmlNewProp(node, BAD_CAST "width", BAD_CAST floatStr);

				if (getLength(rect->otherAttributes) != 0)
				{
					ListIterator innerItr = createIterator(rect->otherAttributes);

					Attribute *attr = nextElement(&innerItr);

					while (attr != NULL)
					{
						xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
						attr = nextElement(&innerItr);
					}
				}
				rect = nextElement(&itr);
			}
		}

		// creating the <circle> child node
		if (getLength(svgImage->circles) != 0)
		{
			ListIterator itr = createIterator(svgImage->circles);

			Circle *circ = nextElement(&itr);

			while (circ != NULL)
			{
				node = xmlNewChild(rootNode, NULL, BAD_CAST "circle", NULL);

				snprintf(floatStr, sizeof(floatStr), "%f%s", circ->cx, circ->units);
				xmlNewProp(node, BAD_CAST "cx", BAD_CAST floatStr);

				snprintf(floatStr, sizeof(floatStr), "%f%s", circ->cy, circ->units);
				xmlNewProp(node, BAD_CAST "cy", BAD_CAST floatStr);

				snprintf(floatStr, sizeof(floatStr), "%f%s", circ->r, circ->units);
				xmlNewProp(node, BAD_CAST "r", BAD_CAST floatStr);

				if (getLength(circ->otherAttributes) != 0)
				{
					ListIterator innerItr = createIterator(circ->otherAttributes);

					Attribute *attr = nextElement(&innerItr);

					while (attr != NULL)
					{
						xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
						attr = nextElement(&innerItr);
					}
				}
				circ = nextElement(&itr);
			}
		}

		// creating the <path> child node
		if (getLength(svgImage->paths) != 0)
		{
			ListIterator itr = createIterator(svgImage->paths);

			Path *path = nextElement(&itr);

			while (path != NULL)
			{
				node = xmlNewChild(rootNode, NULL, BAD_CAST "path", NULL);

				xmlNewProp(node, BAD_CAST "d", BAD_CAST path->data);

				if (getLength(path->otherAttributes) != 0)
				{
					ListIterator innerItr = createIterator(path->otherAttributes);

					Attribute *attr = nextElement(&innerItr);

					while (attr != NULL)
					{
						xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
						attr = nextElement(&innerItr);
					}
				}
				path = nextElement(&itr);
			}
		}

		//creating the <g> child node
		if (getLength(svgImage->groups) != 0)
		{
			ListIterator itr = createIterator(svgImage->groups);
			Group *grp = nextElement(&itr);

			while (grp != NULL)
			{
				node = xmlNewChild(rootNode, NULL, BAD_CAST "g", NULL);
				createGroupFromDoc(node, grp);
				grp = nextElement(&itr);
			}
		}
	}

	void createGroupFromDoc(xmlNodePtr root, Group *group)
	{
		if (!root || !group)
			return;

		xmlNodePtr rootNode = root;
		xmlNodePtr node = NULL;
		char floatStr[128];

		if (getLength(group->otherAttributes) != 0)
		{
			ListIterator itr = createIterator(group->otherAttributes);

			Attribute *attr = nextElement(&itr);

			while (attr != NULL)
			{
				xmlNewProp(rootNode, BAD_CAST attr->name, BAD_CAST attr->value);
				attr = nextElement(&itr);
			}
		}

		// creating the <rect> child node
		if (getLength(group->rectangles) != 0)
		{
			ListIterator itr = createIterator(group->rectangles);

			Rectangle *rect = nextElement(&itr);

			while (rect != NULL)
			{
				node = xmlNewChild(rootNode, NULL, BAD_CAST "rect", NULL);

				snprintf(floatStr, sizeof(floatStr), "%f%s", rect->x, rect->units);
				xmlNewProp(node, BAD_CAST "x", BAD_CAST floatStr);

				snprintf(floatStr, sizeof(floatStr), "%f%s", rect->y, rect->units);
				xmlNewProp(node, BAD_CAST "y", BAD_CAST floatStr);

				snprintf(floatStr, sizeof(floatStr), "%f%s", rect->height, rect->units);
				xmlNewProp(node, BAD_CAST "height", BAD_CAST floatStr);

				snprintf(floatStr, sizeof(floatStr), "%f%s", rect->width, rect->units);
				xmlNewProp(node, BAD_CAST "width", BAD_CAST floatStr);

				if (getLength(rect->otherAttributes) != 0)
				{
					ListIterator innerItr = createIterator(rect->otherAttributes);

					Attribute *attr = nextElement(&innerItr);

					while (attr != NULL)
					{
						xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
						attr = nextElement(&innerItr);
					}
				}
				rect = nextElement(&itr);
			}
		}

		// creating the <circle> child node
		if (getLength(group->circles) != 0)
		{
			ListIterator itr = createIterator(group->circles);

			Circle *circ = nextElement(&itr);

			while (circ != NULL)
			{
				node = xmlNewChild(rootNode, NULL, BAD_CAST "circle", NULL);

				snprintf(floatStr, sizeof(floatStr), "%f%s", circ->cx, circ->units);
				xmlNewProp(node, BAD_CAST "cx", BAD_CAST floatStr);

				snprintf(floatStr, sizeof(floatStr), "%f%s", circ->cy, circ->units);
				xmlNewProp(node, BAD_CAST "cy", BAD_CAST floatStr);

				snprintf(floatStr, sizeof(floatStr), "%f%s", circ->r, circ->units);
				xmlNewProp(node, BAD_CAST "r", BAD_CAST floatStr);

				if (getLength(circ->otherAttributes) != 0)
				{
					ListIterator innerItr = createIterator(circ->otherAttributes);

					Attribute *attr = nextElement(&innerItr);

					while (attr != NULL)
					{
						xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
						attr = nextElement(&innerItr);
					}
				}
				circ = nextElement(&itr);
			}
		}

		// creating the <path> child node
		if (getLength(group->paths) != 0)
		{
			ListIterator itr = createIterator(group->paths);

			Path *path = nextElement(&itr);

			while (path != NULL)
			{
				node = xmlNewChild(rootNode, NULL, BAD_CAST "path", NULL);

				xmlNewProp(node, BAD_CAST "d", BAD_CAST path->data);

				if (getLength(path->otherAttributes) != 0)
				{
					ListIterator innerItr = createIterator(path->otherAttributes);

					Attribute *attr = nextElement(&innerItr);

					while (attr != NULL)
					{
						xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
						attr = nextElement(&innerItr);
					}
				}
				path = nextElement(&itr);
			}
		}

		//creating the <g> child node
		if (getLength(group->groups) != 0)
		{
			ListIterator itr = createIterator(group->groups);
			Group *grp = nextElement(&itr);

			while (grp != NULL)
			{
				node = xmlNewChild(rootNode, NULL, BAD_CAST "g", NULL);
				createGroupFromDoc(node, grp);
				grp = nextElement(&itr);
			}
		}
	}

// ******************************* A3 userCreated helper.h functions ***************************
	List* parseJSONObj(char* JSON) // functions that retuns name value pair of JSON object. delimit " { }
	{
		List* properties = NULL;
		Attribute* attribute = NULL;
		char* string = NULL;
		char* token = NULL;

		properties = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

		string = malloc(strlen(JSON)+3);
		strcpy(string,JSON);
		replaceChar(string, '\"');
		replaceChar(string, '{');
		replaceChar(string, '}');

		token = strtok(string, ":");
		attribute = malloc(sizeof(Attribute));
		attribute->name = malloc(strlen(token) + 1);
		strcpy(attribute->name, token);
		token = strtok(NULL, ",");
		attribute->value = malloc(strlen(token) + 1);
		strcpy(attribute->value, token);

		insertBack(properties, attribute);
		token = strtok(NULL, ":");

		while(token != NULL) 
		{        
			attribute = malloc(sizeof(Attribute));
			attribute->name = malloc(strlen(token) + 1);

			strcpy(attribute->name, token);
			token = strtok(NULL, ",");
			
			attribute->value = malloc(strlen(token) + 1);
			
			strcpy(attribute->value, token);
			
			insertBack(properties, attribute);
			
			token = strtok(NULL, ":");
			
			if(token == NULL) {
				break;
			}
		}

		free(string);

		return properties;
	}

	void replaceChar(char *str, char s) {
		int j = 0; 
		for (int i = j = 0;i < strlen(str);i++)
		{ 
			if (str[i] != s) {
				str[j++] = str[i]; 
			}
		}
		str[j] = '\0'; 
	}

	char* getSVGInfo(char* filename)
	{
		char *dir = malloc(256);
		strcpy(dir, "uploads/");
		strcat(dir, filename);

		SVGimage *svgImage = createSVGimage(dir);

		bool valid = validateSVGimage(svgImage, "svg.xsd");
		if (valid == false)
			return "Invalid file";

		char *str = SVGtoJSON(svgImage);

		deleteSVGimage(svgImage);
		free(dir);
		return str;
	}

	char* getShapesInfo(char* filename)
	{
		char *dir = malloc(256);
		strcpy(dir, "uploads/");
		strcat(dir, filename);

		SVGimage *svgImage = createSVGimage(dir);

		bool valid = validateSVGimage(svgImage, "svg.xsd");
		if (valid == false)
			return "Invalid SVGImage";

		char *rectStr = rectListToJSON(svgImage->rectangles);
		char *circStr = circListToJSON(svgImage->circles);
		char *pathStr = pathListToJSON(svgImage->paths);
		char *grpStr = groupListToJSON(svgImage->groups);

		char *bigStr = malloc(strlen(rectStr) + strlen(circStr) + strlen(pathStr) + strlen(grpStr) + strlen(svgImage->description) + strlen(svgImage->title) + 128);

		strcpy(bigStr, "{\"svgStruct\":{\"rectangle\":");
		strcat(bigStr, rectStr);
		strcat(bigStr, ",\"circle\":");
		strcat(bigStr, circStr);
		strcat(bigStr, ",\"path\":");
		strcat(bigStr, pathStr);
		strcat(bigStr, ",\"group\":");
		strcat(bigStr, grpStr);
		strcat(bigStr, ",\"title\":\"");
		strcat(bigStr, svgImage->title);
		strcat(bigStr, "\",\"description\":\"");
		strcat(bigStr, svgImage->description);
		strcat(bigStr, "\"}}");

		deleteSVGimage(svgImage);
		free(dir);
		free(rectStr);
		free(circStr);
		free(pathStr);
		free(grpStr);

		return bigStr;
	}

	char* createSVG(char* filename, char* svgJSON)
	{
		char *dir = malloc(256);
		strcpy(dir, "uploads/");
		strcat(dir, filename);

		SVGimage *svgImage = JSONtoSVG(svgJSON);

		printf("SVGimageToString: %s\n", SVGimageToString(svgImage));
		printf("Location saved to: %s\n", dir);

		bool write = writeSVGimage(svgImage, dir);
		if (write == false)
			return "Error writing to file";

		char *str = SVGtoJSON(svgImage);

		deleteSVGimage(svgImage);
		free(dir);

		return str;
	}

	bool isValid(char* filename)
	{
		SVGimage* img = NULL;
		bool isValid = false;
		img = createValidSVGimage(filename, "svg.xsd");
		isValid = validateSVGimage(img, "svg.xsd");
		deleteSVGimage(img);
		return isValid;
	}
	
// *******************************deleteFunction***************************
	void deleteAttribute(void *data)
	{
		Attribute *temp = (Attribute *)data;
		free(temp->name);
		free(temp->value);
		free(temp);
	}

	void deleteRectangle(void *data)
	{
		Rectangle *temp = (Rectangle *)data;
		freeList(temp->otherAttributes);
		free(temp);
	}

	void deleteCircle(void *data)
	{
		Circle *temp = (Circle *)data;
		freeList(temp->otherAttributes);
		free(temp);
	}

	void deletePath(void *data)
	{
		Path *temp = (Path *)data;
		free(temp->data);
		freeList(temp->otherAttributes);
		free(temp);
	}

	void deleteGroup(void *data)
	{
		Group *temp = (Group *)data;
		freeList(temp->rectangles);
		freeList(temp->circles);
		freeList(temp->paths);
		freeList(temp->groups);
		freeList(temp->otherAttributes);
		free(temp);
	}

// ************************toStringFunctions*********************************
	char *attributeToString(void *data)
	{
		if (!data)
			return NULL;

		Attribute *attr = (Attribute *)data;
		char *string = malloc(16 + strlen(attr->name) + strlen(attr->value));

		strcpy(string, "name: ");
		strcat(string, attr->name);
		strcat(string, ", ");
		strcat(string, "value: ");
		strcat(string, attr->value);

		return string;
	}

	char *rectangleToString(void *data)
	{
		if (!data)
			return NULL;

		Rectangle *rect = (Rectangle *)data;
		char *attrString = toString(rect->otherAttributes);
		char *string = malloc(256 + strlen(attrString));
		char str[50];

		strcpy(string, "x: ");
		snprintf(str, sizeof(str), "%.4f", rect->x);
		strcat(string, str);
		strcat(string, ", y: ");
		snprintf(str, sizeof(str), "%.4f", rect->y);
		strcat(string, str);
		strcat(string, ", width: ");
		snprintf(str, sizeof(str), "%.4f", rect->width);
		strcat(string, str);
		strcat(string, ", height: ");
		snprintf(str, sizeof(str), "%.4f", rect->height);
		strcat(string, str);
		strcat(string, ", units: ");
		strcat(string, rect->units);
		strcat(string, attrString);

		free(attrString);

		return string;
	}

	char *circleToString(void *data)
	{
		if (!data)
			return NULL;

		Circle *circle = (Circle *)data;
		char *attrString = toString(circle->otherAttributes);
		char *string = malloc(256 + strlen(attrString));
		char str[50];

		strcpy(string, "cx: ");
		snprintf(str, sizeof(str), "%.4f", circle->cx);
		strcat(string, str);
		strcat(string, ", cy: ");
		snprintf(str, sizeof(str), "%.4f", circle->cy);
		strcat(string, str);
		strcat(string, ", r: ");
		snprintf(str, sizeof(str), "%.4f", circle->r);
		strcat(string, str);
		strcat(string, ", units: ");
		strcat(string, circle->units);
		strcat(string, attrString);

		free(attrString);

		return string;
	}

	char *pathToString(void *data)
	{
		if (!data)
			return NULL;

		Path *path = (Path *)data;
		char *attrString = toString(path->otherAttributes);
		char *string = malloc(7 + strlen(path->data) + strlen(attrString));

		strcpy(string, "data: ");
		strcat(string, path->data);
		strcat(string, attrString);

		free(attrString);
		return string;
	}

	char *groupToString(void *data)
	{
		if (!data)
			return NULL;

		Group *group = (Group *)data;
		char *attrs = toString(group->otherAttributes);
		char *rects = toString(group->rectangles);
		char *circs = toString(group->circles);
		char *paths = toString(group->paths);
		char *groups = toString(group->groups);
		char *string = malloc(256 + strlen(attrs) + strlen(rects) + strlen(circs) + strlen(paths) + strlen(groups));

		strcpy(string, "\n\n\notherAttributes(");
		strcat(string, attrs);
		strcat(string, "\n)\n\nRectangles(");
		strcat(string, rects);
		strcat(string, "\n)\n\nCircles(");
		strcat(string, circs);
		strcat(string, "\n)\n\nPaths(");
		strcat(string, paths);
		strcat(string, "\n)\n\nGroups(");
		strcat(string, groups);

		free(attrs);
		free(rects);
		free(circs);
		free(paths);
		free(groups);

		return string;
	}

// ************************compareAttributesFunctions********************************
	int compareAttributes(const void *first, const void *second)
	{
		return 0;
	}

	int compareRectangles(const void *first, const void *second)
	{
		return 0;
	}

	int compareCircles(const void *first, const void *second)
	{
		return 0;
	}

	int compareGroups(const void *first, const void *second)
	{
		return 0;
	}

	int comparePaths(const void *first, const void *second)
	{
		return 0;
	}