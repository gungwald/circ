#include <stdio.h>
#include <stdlib.h>     /* _heapadd */
#include <apple2enh.h>  /* videomode */
#include <tgi.h>

#ifdef TRACE
#define TRACE_MSG puts
#define TRACE_VAR printf
#else
#define TRACE_MSG
#define TRACE_VAR
#endif

#define STR_SIZ 256

static void checkError(const char *message);

/**
 * References first byte in static graphics driver.
 */
extern char a2e_hi;

const char *GRAPHICS_DRIVER = "a2e.hi.tgi";
char response[STR_SIZ];

int main(void)
{
    unsigned char tgiErrorCode = TGI_ERR_OK;

    /* Make more heap memory available: $803 - $1fff. */
    //TRACE_MSG("_heapadd");
    //_heapadd((void *) 0x0803, 0x17FD);

    /* Make sure second hi-res page is available by putting the text mode
       in 40 columns. */
    TRACE_MSG("videomode");
    videomode(VIDEOMODE_40COL);
    
#if DYNAMIC_GRAPHICS_DRIVER
    TRACE_VAR("tgi_load_driver: %s\n", tgi_stddrv);
    tgi_load_driver(tgi_stddrv);
    checkError("Failed to load graphics driver");
#else
    TRACE_MSG("tgi_install");
    tgi_install(tgi_static_stddrv); //a2e_hi
    TRACE_MSG("After tgi_install");
    checkError("Failed to install graphics driver");
#endif

    TRACE_MSG("tgi_init");
    tgi_init();
    TRACE_MSG("tgi_clear");
    tgi_clear();
    checkError("Failed to clear graphics screen");
    TRACE_MSG("tgi_setcolor");
    tgi_setcolor(TGI_COLOR_BLUE);
    TRACE_MSG("tgi_setviewpage");
    tgi_setviewpage(0);
    TRACE_MSG("tgi_setdrawpage");
    tgi_setdrawpage(1);
    TRACE_MSG("tgi_circle");
    tgi_circle(50, 50, 40);

    fgets(response, STR_SIZ, stdin);

    TRACE_MSG("tgi_done");
    tgi_done();

#if DYNAMIC_GRAPHICS_DRIVER
    TRACE_MSG("tgi_unload");
    tgi_unload();
#else
    TRACE_MSG("tgi_uninstall");
    tgi_uninstall();
#endif

    return EXIT_SUCCESS;
}

void checkError(const char* message)
{
    unsigned char errorCode;

    TRACE_MSG("Entering checkError");
    errorCode = tgi_geterror();
    TRACE_MSG("After tgi_geterror");
    if (errorCode != TGI_ERR_OK) {
        fprintf(stderr, "%s: %d\n", message, errorCode);
        exit(EXIT_FAILURE);
    }
    TRACE_MSG("Exiting checkError");
}

