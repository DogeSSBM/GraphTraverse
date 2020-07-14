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
	printf("Drawing pos (%2d, %2d)... ", pos.x, pos.y);
	Coord wpos = graphUpscale(pos);
	setColor(GREY);
	fillSquareCoord(wpos, SCALE);
	printf("done\n");
	// wpos = coordShift2(wpos, DIR_D, DIR_R, 5);
	// setColor(BLUE);
	// fillSquareCoord(wpos, SCALE-10);
	//
	// setColor(GREY);
	// wpos = graphUpscale(pos);
	// wpos = coordShift2(wpos, DIR_D, DIR_R, SCALE/2);
	// fillCircleCoord(wpos, 5);
	//
	// for(uint i = 0; i < 4; i++){
	// 	if(n->arr[i])
	// 		drawLineCoords(wpos, coordShift(wpos, i, SCALE));
	// }
}

//Backup thingey
// Node *appendTile(Node *current, int direction)
// {
// 	current->arr[direction] = calloc(1, sizeof(Node));
// 	current->arr[direction]->arr[dirINV(direction)] = current;
// 	current = current->arr[direction];
// 	return current;
// }

typedef struct{
	SDL_Keycode wasd;
	SDL_Keycode arrow;
}SDLK_Dir;

void build(Node *origin)
{
	clear();
	if(!origin)
		origin = calloc(1, sizeof(Node));
	else{
		printf("just restart the damn thing\n");
		exit(0);
	}
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
					drawNode(current, pos);
					draw();
					pos = coordShift(pos, dir, 1);
					current->arr[dir] = calloc(1, sizeof(Node));
					current->arr[dir]->arr[dirINV(dir)] = current;
					current = current->arr[dir];
					break;
				case SDLK_RETURN:
					return;
					break;
				}
				break;
			}
			ticksLeft = frameEnd-getTicks();
		}
	}
	return;
}

uint traverseAdj(Node *n, const Coord pos, const bool toggleState)
{
	static uint totalNodes;
	if(sameCoord(pos, graphMid))
		totalNodes = 1;

	n->toggle = !n->toggle;
	drawNode(n, pos);
	draw();

	for(uint i = 0; i < 4; i++){
		if(
			n->arr[i] &&
			n->arr[i]->toggle == toggleState
		){
			totalNodes +=
			traverseAdj(n->arr[i], coordShift(pos, i, 1), toggleState);
		}
	}
	return totalNodes;
}

uint traverse(Node *origin)
{
	static bool toggleState = 0;	// Current value of toggle in all Nodes.

	return traverseAdj(origin, graphMid, toggleState);

	toggleState != toggleState;
}
