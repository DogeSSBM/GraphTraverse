#pragma once
#define SCALE	30
const Length graphLen = {.x=32, .y=18};
const Coord graphMid = {.x=graphLen.x/2, .y=graphLen.y/2};
const Length winLen = {.x=graphLen.x*SCALE, .y=graphLen.y*SCALE};
const Coord winMid = {.x=winLen.x/2, .y=winLen.y/2};

Coord graphUpscale(const Coord pos)
{
	const Coord ret = {pos.x*SCALE, pos.y*SCALE};
	return ret;
}

typedef struct Node{
	bool toggle;
	union{
		struct Node *arr[4];
		struct{
			struct Node *u;
			struct Node *r;
			struct Node *d;
			struct Node *l;
		};
	};
}Node;

typedef struct NodeLink{
	Coord pos;
	Node *node;
	struct NodeLink *next;
}NodeLink;

void drawNode(const Node *n, const Coord pos)
{
	printf("Drawing pos (%2d, %2d)... ", pos.x, pos.y);
	Coord wpos = graphUpscale(pos);
	setColor(GREY);
	fillSquareCoord(wpos, SCALE);
	printf("done\n");
	wpos = coordShift2(wpos, DIR_D, DIR_R, 5);
	setColor(BLUE);
	fillSquareCoord(wpos, SCALE-10);

	setColor(GREY);
	wpos = graphUpscale(pos);
	wpos = coordShift2(wpos, DIR_D, DIR_R, SCALE/2);
	fillCircleCoord(wpos, 5);

	for(uint i = 0; i < 4; i++){
		if(n->arr[i])
			drawLineCoords(wpos, coordShift(wpos, i, SCALE));
	}
}

Node* build(void)
{
	clear();
	setColor(BLACK);
	fillScreen();
	Node *origin = calloc(1, sizeof(Node));
	Node *current = origin;
	Coord pos = graphMid;
	Direction dir = -1;
	drawNode(current, pos);
	while(1){
		const Ticks frameEnd = getTicks() + TPF;
		int ticksLeft = frameEnd-getTicks();
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
					dir = keyToDir(event.key.keysym.sym);
					printf("Dir: %d\n",dir);
					pos = coordShift(pos, dir, 1);
					current->arr[dir] = calloc(1, sizeof(Node));
					current->arr[dir]->arr[dirINV(dir)] = current;
					current = current->arr[dir];
					drawNode(current, pos);
					break;
				case SDLK_RETURN:
					return origin;
					break;
				}
				break;
			}
			ticksLeft = frameEnd-getTicks();
		}
		draw();
	}
	return origin;
}

uint traverseAdj(Node *n, const Coord pos)
{
	uint totalNodes = 1;

	n->toggle = !n->toggle;
	drawNode(n, pos);

	for(uint i = 0; i < 4; i++){
		if(
			n->arr[i] &&
			n->arr[i]->toggle != n->toggle
		){
			totalNodes+=
			traverseAdj(n->arr[i], coordShift(pos, i, 1));
		}
	}
	return totalNodes;
}

NodeLink* traverse(Node *origin)
{
	clear();
	NodeLink *list = calloc(1, sizeof(NodeLink));
	printf("NodeLink list created\n");
	list->node = origin;
	printf("Starting traversal\n");
	const uint totalNodes = traverseAdj(origin, graphMid);
	printf("Visited %d nodes\n",totalNodes);
	draw();
	while(1){
		const Ticks frameEnd = getTicks() + TPF;
		int ticksLeft = frameEnd-getTicks();
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
					exit(0);
					break;
				case SDLK_SPACE:
				case SDLK_RETURN:
					return list;
					break;
				}
				break;
			}
		}
	}
	return list;
}
