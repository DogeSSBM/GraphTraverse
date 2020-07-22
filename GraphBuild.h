#pragma once

typedef struct NodeLink{
	Coord pos;
	Node *node;
	struct NodeLink *next;
}NodeLink;

void link(NodeLink *link, NodeLink *list)
{
	NodeLink *current = list;
	while(current != NULL){
		for(uint i = 0; i < 4; i++){
			if(sameCoord(coordShift(link->pos, i, 1), current->pos))
				link->node->arr[i] = current->node;
		}
		current = current->next;
	}
}

uint linkNodeList(NodeLink *list)
{
	NodeLink *current = list;
	uint totalNodes = 0;
	while(current != NULL){
		link(current, list);
		current = current->next;
		printf("linking node %u\n", totalNodes);
		totalNodes++;
	}
	printf("Finished linking %u node(s)\n", totalNodes);
	return totalNodes;
}

void freeNodeList(NodeLink *list, uint totalNodes)
{
	printf("freeing node list of %u nodes\n", totalNodes);
	while(list != NULL){
		printf("%u left\n", totalNodes--);
		NodeLink *n = list->next;
		free(list);
		list = n;
	}
}

void freeNodesAndNodeList(NodeLink *list, uint totalNodes)
{
	printf("freeing node list of %u nodes\n", totalNodes);
	while(list != NULL){
		printf("%u left\n", totalNodes--);
		NodeLink *n = list->next;
		free(list->node);
		free(list);
		list = n;
	}
}


Node* listToGraph(NodeLink *list)
{
	Node *graph = list->node;
	freeNodeList(list, linkNodeList(list));
	return graph;
}

void drawList(NodeLink *list)
{
	NodeLink *current = list;
	while(current != NULL){
		drawNode(current->node, current->pos);
		current = current->next;
	}
}

uint removeDupes(NodeLink *list, NodeLink *link)
{
	uint totalNodes = 1;
	NodeLink *current = list;
	while(current->next != NULL){
		totalNodes++;
		if(sameCoord(current->next->pos, link->pos) &&
		current->next != link){
			printf("unlinking and freeing dupe\n");
			NodeLink *n = current->next->next;
			free(current->next->node);
			free(current->next);
			current->next = n;
		}
		current = current->next;
	}
	return totalNodes;
}

Node* build(void)
{
	NodeLink *origin = calloc(1, sizeof(NodeLink));
	NodeLink *current = origin;
	uint totalNodes = 1;
	origin->pos = graphMid;
	origin->node = calloc(1, sizeof(Node));
	drawNode(origin->node, origin->pos);
	while(1){
		const Ticks frameEnd = getTicks() + TPF;
		int ticksLeft = frameEnd-getTicks();
		clear();
		drawList(origin);
		draw();
		while(ticksLeft > 0){
			Event event  = {0};
			SDL_WaitEventTimeout(&event, ticksLeft);
			switch(event.type){
			case SDL_QUIT:
				printf("Quitting now!\n");
				exit(0);
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					printf("Quitting now!\n");
					freeNodeList(origin, totalNodes);
					exit(0);
					break;
				case SDLK_w:
				case SDLK_UP:
				case SDLK_d:
				case SDLK_RIGHT:
				case SDLK_s:
				case SDLK_DOWN:
				case SDLK_a:
				case SDLK_LEFT:
					current->next = calloc(1, sizeof(NodeLink));
					current->next->node = calloc(1, sizeof(Node));
					current->next->pos = coordShift(
						current->pos,
						keyToDir(
							event.key.keysym.sym
						),
						1
					);
					current = current->next;
					totalNodes = removeDupes(origin, current);
					break;
				case SDLK_RETURN:
					return listToGraph(origin);
					break;
				}
				break;
			}
			ticksLeft = frameEnd-getTicks();
		}
	}
}
