#include <SDL/SDL.h>
#include <sys/param.h> /* for MAXPATHLEN */
#include <unistd.h>
#include <GL/glut.h>
#include "BrownianEngine.h"


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    const char *resourcePath = "./BrownianSDL";
    BrownianEngine* game = new BrownianEngine(resourcePath);
    game->init();
    int status = game->run();

    /* We're done, thank you for playing */
    delete game;
    return status;
}
