#include "Includes.h"

int main(int argc, char const *argv[])
{
	init(winLen.x, winLen.y);
	typedef enum{M_BUILD, M_TRAVERSE, M_EXIT, M_NUM}MenuSelect;
	MenuSelect select = M_BUILD;
	char *label[] = {"Build", "Traverse", "Exit"};
	Node *origin = NULL;
	while(1){
		const Ticks frameEnd = getTicks() + TPF;
		clear();
		drawTextCenteredCoord(winMid, label[select]);
		draw();
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
					select = wrap(select+1, 0, M_NUM);
					printf("select = %d\n", select);
					break;
				case SDLK_s:
				case SDLK_DOWN:
					select = wrap(select-1, 0, M_NUM);
					printf("select = %d\n", select);
					break;
				case SDLK_SPACE:
				case SDLK_RETURN:
					switch(select){
					case M_BUILD:
						build(origin);
						break;
					case M_TRAVERSE:
						if(origin)
							traverse(origin);
						else
							printf("You must construct a graph first!\n");
						break;
					case M_EXIT:
						printf("Quitting now!\n");
						exit(0);
						break;
					}
					break;
				}
				break;
			}
			ticksLeft = frameEnd-getTicks();
		}
	}
	return 0;
}
