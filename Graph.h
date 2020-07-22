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

void drawNode(const Node *n, const Coord pos)
{
	Coord wpos = graphUpscale(pos);
	setColor(GREY);
	fillSquareCoord(wpos, SCALE);
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

uint traverseAdj(Node *n, const Coord pos)
{
	uint totalNodes = 1;

	n->toggle = !n->toggle;
	printf("Drawing pos (%2d, %2d)... ", pos.x, pos.y);
	drawNode(n, pos);
	printf("done\n");

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

uint traverseFree(Node *n, const Coord pos)
{
	uint totalNodes = 1;
	for(uint i = 0; i < 4; i++){
		if(
			n->arr[i] &&
			n->arr[i]->toggle != n->toggle
		){
			totalNodes+=
			traverseFree(n->arr[i], coordShift(pos, i, 1));
		}
	}
	printf("freeing node %u at pos (%2d, %2d)\n",
		totalNodes, pos.x, pos.y);

	return totalNodes;
}

uint traverse(Node *origin)
{
	clear();
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
					return totalNodes;
					break;
				}
				break;
			}
		}
	}
	return totalNodes;
}
